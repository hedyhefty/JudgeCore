#include "Runner.h"

#include <iostream>
#include <fstream>
#include <array>

#include <unistd.h>
#include <sys/resource.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <wait.h>
#include <string.h>
#include <stdio.h>

#include "SyscallChecker.h"

JudgeResult Runner::Process(Parameter par)
{
    pid_t processId = fork();
    _par = par;

    if(processId < 0)
    {
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }
    else if(processId == 0)
    {
        Run();
    }
    else
    {
        Watch(processId);
    }

    return _result;
}

void Runner::Run()
{
    std::cout << "start run target" << std::endl;

    IoRedirection();
    EnableSecurityMode();
    SetLimit();

    if(ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0)
    {
        std::cerr << "ptrace failed in run process" << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }

    auto runCommand = "./" + util::EXEC_NAME;
    execl(runCommand.c_str(), util::EXEC_NAME.c_str(), nullptr);

    //handle execl error
    std::cerr << "target execl error" << std::endl;
    std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
}

void Runner::Watch(pid_t childPid)
{
    int status = 0;

    for(;;)
    {
        if(wait4(childPid, &status, 0, &_rused) < 0)
        {
            std::cerr << "wait4 failed" << std::endl;
            std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
        }

        //run process exit by itself
        if(WIFEXITED(status))
        {
            if(WEXITSTATUS(status) == EXIT_SUCCESS)
            {
                std::cout << "run process finished successfully" << std::endl;
            }
            else
            {
                std::cout << "run process abort" << std::endl;
                _result = JudgeResult::RE;
            }
            break;
        }

        //run process stopped by signal
        if(WIFSIGNALED(status) || (WIFSTOPPED(status) && WSTOPSIG(status) != SIGTRAP))
        {
            int sig = 0;
            if(WIFSIGNALED(status))
            {
                sig = WTERMSIG(status);
                std::cout << "run process signaled by: " << strsignal(sig) << std::endl;
            }
            else
            {
                sig = WSTOPSIG(status);
                std::cout << "run process stopped by: " << strsignal(sig) << std::endl;
            }

            switch(sig)
            {
                case SIGALRM:
                case SIGXCPU:
                case SIGVTALRM:
                case SIGKILL:
                    std::cout << "time limit exceed" << std::endl;
                    _result = JudgeResult::TLE;
                    break;
                case SIGXFSZ:
                    std::cout << "file size limit exceed" << std::endl;
                    _result = JudgeResult::OLE;
                    break;
                default:
                    _result = JudgeResult::RE;
                    break;
            }

            ptrace(PTRACE_KILL, childPid, nullptr, nullptr);
            break;
        }

        //getpagesize() return size of one page(bytes)
        auto memUsage = _rused.ru_minflt * (getpagesize() >> 10);
        if(memUsage > util::MEMORY_LIMIT)
        {
            std::cout << "memory limit exceed" << std::endl;
            _result = JudgeResult::MLE;

            ptrace(PTRACE_KILL, childPid, nullptr, nullptr);
            break;
        }

        user_regs_struct regs;
        //get register of run process
        if(ptrace(PTRACE_GETREGS, childPid, nullptr, &regs) < 0)
        {
            std::cerr << "ptrace get args failed in watch process" << std::endl;
            std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
        }

        auto syscallId = regs.orig_rax;
        SyscallChecker checker;
        if(syscallId > 0 && !checker.IsSyscallValid(_par.lang, syscallId, childPid, regs))
        {
            std::cout << "invalid syscall occurred: " << syscallId << std::endl;
            _result = JudgeResult::RE;

            ptrace(PTRACE_KILL, childPid, nullptr, nullptr);
            break;
        }

        if(ptrace(PTRACE_SYSCALL, childPid, nullptr, nullptr) < 0)
        {
            std::cerr << "ptrace syscall failed" << std::endl;
            std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
        }
    }
}

void Runner::EnableSecurityMode()
{
    if(chdir(_par.runDir.c_str()) != EXIT_SUCCESS)
    {
        std::cerr << "chdir failed" << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }

    std::array<char, 1024> cwd;
    if(getcwd(cwd.data(), cwd.size()) == nullptr)
    {
        std::cerr << "getcwd failed" << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }

    if(chroot(cwd.data()) != EXIT_SUCCESS)
    {
        std::cerr << "chroot failed" << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }
}

void Runner::SetLimit()
{
    //cpu time limit
    rlimit lim;
    lim.rlim_max = (_par.timeLimit - _usedTime + 999) / 1000 + 1;
    lim.rlim_cur = lim.rlim_max;
    if(setrlimit(RLIMIT_CPU, &lim) < 0)
    {
        std::cerr << "setrlimit cpu failed" << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }

    //stack size limit
    lim.rlim_max = util::STD_MB << 8;
    lim.rlim_cur = lim.rlim_max;
    if(setrlimit(RLIMIT_STACK, &lim) < 0)
    {
        std::cerr << "setrlimit stack failed" << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }

    lim.rlim_max = (util::STD_MB << 5) + util::STD_MB;
    lim.rlim_cur = util::STD_MB << 5;
    if(setrlimit(RLIMIT_FSIZE, &lim) < 0)
    {
        std::cerr << "setrlimit file size failed" << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }
}

void Runner::IoRedirection()
{
    std::cout << "Redirect IO of run process" <<std::endl;

    auto inFilePath = _par.runDir + "/" + util::INPUT_FILE_NAME;
    auto outFilePath = _par.runDir + "/" + util::OUTPUT_FILE_NAME;

    stdin = freopen(inFilePath.c_str(),"r",stdin);
    stdout = freopen(outFilePath.c_str(),"w",stdout);

    if(!stdin || !stdout)
    {
        std::cerr<<"error while redirecting io: "<<stdin<<", "<<stdout<<std::endl;
        std::exit(exitcode::EXIT_UNKNOWN);
    }
}

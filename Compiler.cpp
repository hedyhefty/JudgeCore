#include "Compiler.h"

#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

void Compiler::Process(Parameter par)
{
    pid_t processId = fork();

    if(processId < 0)
    {
        std::cerr << "error fork child" << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }
    else if(processId == 0)
    {
        std::cout << "compile process start the job." << std::endl;
        Compile(par);
    }
    else
    {
        int status;
        pid_t w = waitpid(processId, &status, WUNTRACED);
        if(w == -1)
        {
            std::cerr << "waitpid error" << std::endl;
            std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
        }

        std::cout << "compile process finished" << std::endl;
        if(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
        {
            std::cout << "compile succeed" << std::endl;
        }
        else
        {
            std::cerr << "compile error" << std::endl;
            std::exit(exitcode::EXIT_COMPILE);
        }
    }
}

void Compiler::Compile(Parameter par)
{
    std::string execFilePath = par.runDir + "/" + util::EXEC_NAME;
    switch (par.lang)
    {
        case Language::C:
            execlp("gcc", "gcc", par.srcPath.c_str(), "-o", execFilePath.c_str(),
                   "-static", "-w", "-lm", "-std=c99", "-O2", "-DONLINE_JUDGE", nullptr);
            break;
        case Language::Cpp:
            execlp("g++", "g++", par.srcPath.c_str(), "-o", execFilePath.c_str(),
                   "-static", "-w", "-lm", "-std=c++11", "-O2", "-DONLINE_JUDGE", nullptr);
            break;
        default:
            break;
    }

    //execute error occur:
    std::cerr << "error occured while exec compile commands" << std::endl;
    std::exit(exitcode::EXIT_COMPILE);
}

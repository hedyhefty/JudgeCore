#include "JudgeCore.h"

#include <unistd.h>

JudgeCore::JudgeCore()
{

}

void JudgeCore::Process(const int argc, const char* const* argv)
{
    _parameters = _parser.ParseArgs(argc, argv);

    std::cout << "get parameter succeed, \nsrc path: " << _parameters.srcPath
              << ", \nrun dir: " << _parameters.runDir
              << ", \nlanguage(enum): " << static_cast<int>(_parameters.lang) << std::endl;

    PrepareWorkspace();

    _compiler.Process(_parameters);
    std::cout << "compiler job done, return to Core." << std::endl;

    auto res = _runner.Process(_parameters);
    std::cout << "Runner finished, return to Core." << std::endl;
    if(res != JudgeResult::None)
    {
        std::cout << "Judge finished after run, judge result: " << static_cast<int>(res) << std::endl;
        return;
    }

    res = _judger.Judge(_parameters);
    if(res == JudgeResult::AC)
    {
        std::cout << "congratulations, your answer got AC" << std::endl;
    }
    else if(res == JudgeResult::WA)
    {
        std::cout << "Wrong answer" << std::endl;
    }
}

void JudgeCore::PrepareWorkspace()
{
    std::string command = "cp " + util::INPUT_FILE_PATH + "/" + _parameters.inputFileName + " " + _parameters.runDir;
    int status = std::system(command.c_str());

    if(status < 0)
    {
        std::cerr << "system call error, cmd: " << command << std::endl;
        std::exit(exitcode::EXIT_SYSTEMCALL_ERROR);
    }
    else
    {
        if(WIFEXITED(status))
        {
            std::cout << "cmd returned normally, exit code " << WEXITSTATUS(status) << std::endl;
        }
        else
        {
            std::cout << "cmd exited abnormaly" << std::endl;
        }
    }
}

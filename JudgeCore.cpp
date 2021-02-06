#include "JudgeCore.h"

JudgeCore::JudgeCore()
{

}

void JudgeCore::Process(const int argc, const char* const* argv)
{
    _parameters = _parser.ParseArgs(argc, argv);

    std::cout << "get parameter succeed, \nsrc path: " << _parameters.srcPath
              << ", \nrun dir: " << _parameters.runDir
              << ", \nlanguage(enum): " << static_cast<int>(_parameters.lang) << std::endl;

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

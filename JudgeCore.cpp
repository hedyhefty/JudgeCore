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
}

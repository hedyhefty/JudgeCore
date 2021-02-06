#include "Parser.h"

#include <vector>
#include <string>

Parameter Parser::ParseArgs(const int argc, const char* const* argv)
{
    Parameter p;

    for(int i = 0; i < argc - 1; ++i)
    {
        std::string flag(argv[i]);
        std::string parameter(argv[i + 1]);

        if(flag == "-c")
        {
            p.srcPath = parameter;
        }
        else if(flag == "-d")
        {
            p.runDir = parameter;
        }
        else if(flag == "-l")
        {
            p.lang = static_cast<Language>(std::stoi(parameter));
        }
        else if(flag == "-i")
        {
            p.inputFileName = parameter;
        }
        else if(flag == "-a")
        {
            p.groundTrueFileName = parameter;
        }
    }

    if(p.srcPath.empty() || p.runDir.empty() || p.lang == Language::UnSet)
    {
        std::cerr << "Parser error" << std::endl;
        exit(exitcode::EXIT_BAD_PARAM);
    }

    return p;
}

#pragma once

#include "Parser.h"
#include "Compiler.h"
#include "Runner.h"

class JudgeCore
{
public:
    JudgeCore();

    void Process(const int argc, const char* const* argv);

private:
    Parser _parser;
    Parameter _parameters;
    Compiler _compiler;
    Runner _runner;
};

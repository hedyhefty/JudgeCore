#pragma once

#include <iostream>
#include <utility>

#include "Utils.h"

class Parser
{
public:
    Parameter ParseArgs(const int argc, const char* const* argv);
};

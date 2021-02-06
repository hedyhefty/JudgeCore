#pragma once

#include "Utils.h"

class Judger
{
public:
    Judger();

    JudgeResult Judge(Parameter par);

private:
    bool IsAnswerIdentical(const std::string& ansPath, const std::string& groundTruePath);
};

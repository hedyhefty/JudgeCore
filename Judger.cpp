#include "Judger.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <iostream>

Judger::Judger()
{

}

JudgeResult Judger::Judge(Parameter par)
{
    std::string ansPath = par.runDir + "/" + util::OUTPUT_FILE_NAME;
    if(IsAnswerIdentical(ansPath, "ans/answer"))
    {
        return JudgeResult::AC;
    }

    return JudgeResult::WA;
}

bool Judger::IsAnswerIdentical(const std::string& ansPath, const std::string& groundTruePath)
{
    std::ifstream ansFile(ansPath);
    std::ifstream gtFile(groundTruePath);

    std::istreambuf_iterator<char> ansItor(ansFile);
    std::istreambuf_iterator<char> gtItor(gtFile);

    std::istreambuf_iterator<char> end;

    while(ansItor != end && gtItor != end)
    {
        if(*ansItor != *gtItor)
        {
            return false;
        }

        ++ansItor;
        ++gtItor;
    }

    return (ansItor == end) && (gtItor == end);
}

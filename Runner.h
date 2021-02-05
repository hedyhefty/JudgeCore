#pragma once
#include "Utils.h"
#include <sys/resource.h>

class Runner
{
public:
    JudgeResult Process(Parameter par);

private:
    void Run();

    void Watch(pid_t processId);

    void EnableSecurityMode();

    void SetLimit();

    void IoRedirection();

private:
    Parameter _par;
    rusage _rused;
    int _usedTime = 0;
    JudgeResult _result = JudgeResult::None;
};

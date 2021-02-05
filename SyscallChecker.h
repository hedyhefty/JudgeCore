#pragma once

#include <map>

#include <sys/user.h>

#include "Utils.h"

class SyscallChecker
{
public:
    SyscallChecker();

    bool IsSyscallValid(Language l, int syscallId, pid_t child, user_regs_struct regs);

private:
    void InitSyscallMap();

private:
    std::map<Language, std::map<int, int>> _syscallLangDirectory;
    bool _inSyscall = true;
};

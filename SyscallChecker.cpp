#include "SyscallChecker.h"

#include <iostream>
#include <syscall.h>

SyscallChecker::SyscallChecker()
{
    InitSyscallMap();
}

bool SyscallChecker::IsSyscallValid(Language l, int syscallId, pid_t child, user_regs_struct regs)
{
    _inSyscall = !_inSyscall;
    auto& table = _syscallLangDirectory[l];

    if(table.count(syscallId) == 0)
    {
        std::cout<<"Invalid syscall: "<<syscallId<<std::endl;
        return false;
    }
    else if(table[syscallId] > 0 && !_inSyscall)
    {
        std::cout<<"limited syscall: "<<syscallId<<std::endl;
        --table[syscallId];
        return true;
    }

    std::cout<<"permitted syscall: "<<syscallId<<std::endl;
    return true;
}

void SyscallChecker::InitSyscallMap()
{
    std::map<int, int> cppMap =
    {
        {SYS_access,         -1},
        {SYS_arch_prctl,     -1},
        {SYS_brk,            -1},
        {SYS_close,          -1},
        {SYS_execve,          1},
        {SYS_exit_group,     -1},
        {SYS_fstat,          -1},
        {SYS_futex,          -1},
        {SYS_gettimeofday,   -1},
        {SYS_mmap,           -1},
        {SYS_mremap,         -1},
        {SYS_mprotect,       -1},
        {SYS_munmap,         -1},
        {SYS_lseek,          -1},
        {SYS_read,           -1},
        {SYS_set_thread_area, -1},
        {SYS_uname,          -1},
        {SYS_write,          -1},
        {SYS_writev,         -1},
        {SYS_time,           -1},
        {SYS_readlink,       -1},
        //temp
        {SYS_open,           -1}
    };

    _syscallLangDirectory[Language::Cpp] = cppMap;
    _syscallLangDirectory[Language::C] = cppMap;
}

#pragma once
#include <string>

namespace exitcode
{
const int EXIT_OK               = 0;  //正常退出
const int EXIT_UNPRIVILEGED     = 1;  //缺乏权限退出
const int EXIT_BAD_PARAM        = 3;  //参数错误退出
const int EXIT_SET_LIMIT        = 15;  //设置时间限制错误退出
const int EXIT_SET_SECURITY     = 17;  //设置安全限制错误退出
const int EXIT_JUDGE            = 21;  //程序调用错误退出
const int EXIT_COMPARE          = 27;
const int EXIT_TIMEOUT          = 36;  //超时退出
const int EXIT_SYSTEMCALL_ERROR = 37;
const int EXIT_COMPILE          = 39;
const int EXIT_UNKNOWN          = 127;  //不详
}

namespace util
{
const std::string EXEC_NAME = "a.out";
const std::string INPUT_FILE_NAME = "in.in";
const std::string OUTPUT_FILE_NAME = "out.out";

const unsigned long STD_MB = 1048576;
const unsigned int MEMORY_LIMIT = 65535; //kb
}

enum class JudgeResult
{
    AC = 0,
    RE,
    TLE,
    OLE,
    MLE,
    WA,

    None = 100
};

enum class Language
{
    C = 0,
    Cpp,

    UnSet = 100
};

struct Parameter
{
    std::string srcPath;
    std::string runDir;
    Language lang = Language::UnSet;
    int timeLimit = 1000;
};

/*****************************************************************//**
 * \file   tools.h
 * \brief  工具文件，提供一些常用的函数
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef TOOLS_H
#define TOOLS_H
#include "standard.h"

/* 宏工具 */
#define CONCAT(a, b) a##b

/* 调试日志输出宏 */
#define CERR std::cerr

/* 断言宏 配合log使用 */
#define ASSERT(x) do{\
        CERR << " Assertion " << x << "failed" << std::endl; \
        std::abort(); \
    } while(0);

namespace Util {
/* 字符串格式化实现 V1 */
    inline std::string formatMessage(const std::string& format) {
        return format;
    }
    /* 字符串格式化实现 V2 */
    template <typename T, typename... Args>
    std::string formatMessage(const std::string& format, T&& first, Args&&... args) {
        size_t pos = format.find("{}");
        if (pos == std::string::npos) {
            return format;
        }
        std::ostringstream stream;
        stream << format.substr(0, pos) << std::forward<T>(first);
        return stream.str() + formatMessage(format.substr(pos + 2), std::forward<Args>(args)...);
    }

    /* 获取最后一个cpp或h文件 */
    static std::string getLastCppOrHFile(const std::string& path) {
        size_t lastSlash = path.find_last_of("/\\");
        std::string fileName = (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);
        size_t dotPos = fileName.find_last_of('.');
        if (dotPos != std::string::npos) {
            std::string ext = fileName.substr(dotPos);
            if (ext == ".cpp" || ext == ".h") {
                return fileName;
            }
        }
        return "";
    }
}

#endif // TOOLS_H
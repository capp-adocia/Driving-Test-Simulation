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

/* 断言宏 配合log使用 */
#define ASSERT \
        std::cerr << "Assertion failed, in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        std::abort(); \

/* 字符串格式化实现 V1 */
inline std::string formatMessage(const std::string& format) {
    return format;
}
/* 字符串格式化实现 V2 */
template <typename T, typename... Args>
inline std::string formatMessage(const std::string& format, T&& value, Args&&... args) {
    size_t pos = format.find("{}");
    if (pos == std::string::npos) return format;

    std::ostringstream oss;
    oss << value;
    return format.substr(0, pos) + oss.str() +
        formatMessage(format.substr(pos + 2), std::forward<Args>(args)...);
}

#endif // TOOLS_H
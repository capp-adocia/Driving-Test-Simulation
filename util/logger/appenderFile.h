/*****************************************************************//**
 * \file   appenderFile.h
 * \brief  文件输出 在输出文件夹中创建一个以日期命名的文件，并将日志消息写入该文件中
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef APPENDERFILE_H
#define APPENDERFILE_H
#include "appender.h"
class AppenderFile : public Appender {
public:
    AppenderFile(const std::string& folderName);
    ~AppenderFile();
    void append(LogLevel level, const std::string& logMessage);

private:
    std::string getCurrentDate() const;
    bool createDirectory(const std::string& path) const;
    std::string getLogFilePath() const;
private:
    std::ofstream m_file;
    std::string m_folderName;
    std::mutex m_mutex;
};
#endif // APPENDERFILE_H
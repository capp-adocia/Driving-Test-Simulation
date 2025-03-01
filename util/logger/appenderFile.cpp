/*****************************************************************//**
 * \file   appenderFile.cpp
 * \brief  文件输出
 *
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "appenderFile.h"
#include "../common/preprocessor.h"
#include "../logger/log.h"

AppenderFile::AppenderFile(const std::string& folderName) {
    m_folderName = folderName;
    if (!createDirectory(m_folderName)) {
        LOG_ERROR("Failed to create directory: " + m_folderName);
    }

    std::string dateFolder = m_folderName + "/" + getCurrentDate();
    if (!createDirectory(dateFolder)) {
        LOG_ERROR("Failed to create directory: " + dateFolder);
    }

    std::string logFilePath = getLogFilePath();
    m_file.open(logFilePath, std::ios::app);
    if (!m_file.is_open()) {
        LOG_ERROR("Failed to open log file: " + logFilePath);
    }
}

AppenderFile::~AppenderFile() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_file.is_open()) {
        m_file << std::endl;
        m_file.close();
        printColor(LOG_LEVEL_INFO, "Log file saved and closed!");
        return;
    }
    printColor(LOG_LEVEL_WARN, "Failed to close log file!");
}

void AppenderFile::append(LogLevel level, const std::string& logMessage) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_file.is_open()) {
        switch (level) {
        case LogLevel::LOG_LEVEL_DEBUG:
            m_file << DEBUG_S << logMessage << std::endl;
            break;
        case LogLevel::LOG_LEVEL_INFO:
            m_file << INFO_S << logMessage << std::endl;
            break;
        case LogLevel::LOG_LEVEL_WARN:
            m_file << WARN_S << logMessage << std::endl;
            break;
        case LogLevel::LOG_LEVEL_ERROR:
            m_file << ERROR_S << logMessage << std::endl;
            ASSERT(logMessage);
            break;
        default:
            m_file << "Invalid log level" << std::endl;
            LOG_ERROR("Invalid log level");
            break;
        }
    }
}

std::string AppenderFile::getCurrentDate() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

bool AppenderFile::createDirectory(const std::string& path) const {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
#ifdef _WIN32
        return _mkdir(path.c_str()) == 0;
#else
        return mkdir(path.c_str(), 0733) == 0;
#endif
    }
    else if (info.st_mode & S_IFDIR) {
        return true;
    }
    else {
        return false;
    }
}

std::string AppenderFile::getLogFilePath() const {
    std::string dateFolder = m_folderName + "/" + getCurrentDate();
    return dateFolder + "/log.txt";
}
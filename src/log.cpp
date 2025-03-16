#include "log.h"
#include <iostream>
#include <fstream>

#define FILE_LOG "log.txt"

void writeLog(const std::string &message)
{
    std::ofstream logFile(FILE_LOG, std::ios::app);
    if (!logFile.is_open())
    {
        std::cerr << "Không thể mở file log: " << FILE_LOG << std::endl;
        return;
    }
    logFile << message << std::endl;
    logFile.close();
}

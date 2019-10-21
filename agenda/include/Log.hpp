#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <time.h>

class Log {
public:
    Log() {}
    void LogToFile(std::string str);
};

#endif

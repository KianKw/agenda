#include "../include/Log.hpp"
// #include "../include/Path.hpp"

#include <fstream>
#include <time.h>

void Log::LogToFile(std::string s) {
    time_t t = time(0);
    char str[100];
    std::ofstream out("/tmp/log_data.txt", std::ofstream::app);
    strftime(str, sizeof(str), "%Y-%m-%d/%I:%M:%S", localtime(&t));
    std::string temp(str);
    out << " * " << temp << " * --- " << s << std::endl;
    out.close();
}

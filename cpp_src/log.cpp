#include "include/log.h"
#include <fstream>
#include <ctime>

std::ofstream log_file;

void write_log(const std::string &event_type, const std::string &file_name) {
    std::ofstream log_file;
    log_file.open("log.txt", std::ios_base::app);

    std::time_t current_time = std::time(0);
    char buffer[80];
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));

    log_file << "O arquivo " << file_name << " foi " << event_type << " em " << buffer << ".\n";
    log_file.close();
}
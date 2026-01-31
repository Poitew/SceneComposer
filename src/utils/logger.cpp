#include "logger.hpp"

std::vector<std::string> Logger::logs{};

void Logger::log(std::string entry) { logs.push_back(entry); }

std::vector<std::string>& Logger::get_logs() { return logs; }

#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <vector>

class Logger {
 public:
  static void log(std::string log);
  static std::vector<std::string>& get_logs();

 private:
  static std::vector<std::string> logs;
};

#endif
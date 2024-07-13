#include "Util.h"
#include <chrono>
#include <iomanip>

std::string getDate(int hourOffset) {
  auto now = std::chrono::system_clock::now();

  auto tomorrow = now + std::chrono::hours(hourOffset);

  std::time_t tt = std::chrono::system_clock::to_time_t(tomorrow);
  std::tm local_tm = *std::localtime(&tt);

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "%Y-%m-%d");

  return oss.str();
}

int getDay(std::string date) { return std::stoi(date.substr(8, 2)); }

int getMonth(std::string date) { return std::stoi(date.substr(5, 2)); }

int getYear(std::string date) { return std::stoi(date.substr(0, 4)); }

std::string getPreviousDate(int day, int month, int year) {
  std::ostringstream oss;
  oss << year << "-" << std::setw(2) << std::setfill('0') << month << "-"
      << std::setw(2) << std::setfill('0') << day - 1;
  std::string date = oss.str();
  return date;
}
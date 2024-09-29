#pragma once

#include <string>
#include <cstdint>

std::string getDate(int hourOffset);

int getDay(std::string date);
int getMonth(std::string date);
int getYear(std::string date);

std::string getPreviousDate(int day, int month, int year);

uint64_t hton64(uint64_t host64);
uint64_t ntoh64(uint64_t net64);
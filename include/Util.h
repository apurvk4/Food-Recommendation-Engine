#pragma once

#include <string>

std::string getDate(int hourOffset);

int getDay(std::string date);
int getMonth(std::string date);
int getYear(std::string date);

std::string getPreviousDate(int day, int month, int year);
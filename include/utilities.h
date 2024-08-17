#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>

std::vector<std::string> splitInput(const std::string& line);
std::vector<char> extractFlags(std::vector<std::string> args);

// checks if command was used with flag
bool containsFlag(const std::vector<char> &flags, const char &flag);

#endif
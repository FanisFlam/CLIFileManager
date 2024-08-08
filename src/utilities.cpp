#include <iostream>
#include "utilities.h"

#include <sstream>

bool exitCli = false;

std::vector<std::string> splitInput(const std::string& line){
    std::stringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;

    // extracting each word from the line in a vector
    while(iss >> token){
        tokens.push_back(token);
    }

    return tokens;
}
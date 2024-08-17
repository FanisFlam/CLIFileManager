#include "utilities.h"

#include <iostream>
#include <sstream>
#include <algorithm>

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

std::vector<char> extractFlags(std::vector<std::string> args){
    std::vector<char> flags;

    for(std::string& arg : args){
        if(arg[0] == '-'){
            arg.erase(0, 1);
            flags.insert(flags.end(), arg.begin(), arg.end());
        }
    }

    return flags;
}

bool containsFlag(const std::vector<char> &flags, const char &flag){
    return std::find(flags.begin(), flags.end(), flag) != flags.end();
}
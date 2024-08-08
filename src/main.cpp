#include <iostream>
#include <filesystem>
#include <string>

#include "utilities.h"
#include "commands.h"

int main(){
    std::string line;

    // main loop
    while(!exitCli){
        std::cout << std::filesystem::current_path() << ">";
        std::getline(std::cin, line);

        std::vector<std::string> args = splitInput(line);

        // extracting the first word - the command
        std::string command = args[0];

        handleCommand(decodeCommand(command), args);
    }

    return 0;
}
#include <iostream>
#include <filesystem>
#include <string>

#include "utilities.h"
#include "commands.h"

int main(){
    std::string line;

    // Start message
    std::cout << std::endl;
    std::cout << "***************************************" << std::endl;
    std::cout << "*  CLI FILE MANAGER - DEVELOPED BY    *" << std::endl;
    std::cout << "*              FANIS FLAM             *" << std::endl;
    std::cout << "***************************************" << std::endl << std::endl;

    // Main loop
    while(!exitCli){
        std::cout << std::filesystem::current_path().string() << ">";
        std::getline(std::cin, line);

        std::vector<std::string> args = splitInput(line);

        // extracting the first word - the command
        std::string command = args[0];

        args.erase(args.begin());
        handleCommand(command, args);
    }

    return 0;
}
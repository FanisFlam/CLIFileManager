#include "commands.h"

#include <iostream>
#include <sstream>
#include <fstream>

// ~~~~ handling command functions ~~~~
// executes code based on command
void handleCommand(const Command& command, const std::vector<std::string>& args){
    switch(command){
        case exit_cli:
            exitCli = true;
            break;
        case cd:
            changeDir(args[1]);
            break;
        case ls:
            if(args.size() > 1 && std::filesystem::exists(args[1]) && args[1] != "")
                listFiles(args[1]);
            else
                listFiles(std::filesystem::current_path());
            break;
        case pwd:
            std::cout << std::filesystem::current_path() << std::endl;
            break;
        case clear:
            clearScreen();
            break;
        case echo:
            printToScreen(args);
            break;
        case touch:
            createFile(args);
            break;
        case rm:
            removeFile(args);
            break;
        case undefined:
            std::cout << "Please provide a valid command." << std::endl;
            break;
    }
}

// returns an enum form of the command for the switch statement in the handleCommand()
Command decodeCommand(const std::string& command){
    if(command == "exit")
        return exit_cli;
    if(command == "cd")
        return cd;
    if(command == "ls")
        return ls;
    if(command == "pwd")
        return pwd;
    if(command == "echo")
        return echo;
    if(command == "touch")
        return touch;
    if(command == "rm")
        return rm;
    if(command == "clear")
        return clear;
    return undefined;
}

// ~~~~ execution functions ~~~~
//cd command
void changeDir(const std::filesystem::path& path){
    if(std::filesystem::exists(path)){
        std::filesystem::current_path(path);
    }
}

// ls command
void listFiles(const std::filesystem::path& path){
    for (const auto &entry : std::filesystem::directory_iterator(path)){
        std::cout << entry.path().filename().string() << "  ";
    }
    std::cout << std::endl;
}

// echo command
void printToScreen(const std::vector<std::string>& args){
    if(args.size() <= 1){
        std::cout << std::endl;
        return;
    }

    std::ostringstream text;
    for (size_t i = 1; i < args.size(); i++)
    {
        if(i > 1){
            text << " ";
        }
        text << args[i];
    }
    std::cout << text.str() << std::endl;
}

void createFile(const std::vector<std::string>& filenames){
    for(const std::string& filename : filenames){
        std::ofstream file(filename);

        file.close();
    }
}

void removeFile(const std::vector<std::string>& filenames){
    if(filenames.size() > 0){
        for(const std::string& filename : filenames){
            std::filesystem::remove(filename);
        }
    } else {
        std::cout << "rm: missing operand" << std::endl;
    }
}

// clear command
void clearScreen(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
#endif
}
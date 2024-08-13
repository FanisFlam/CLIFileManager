#include "commands.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

// ~~~~ handling command functions ~~~~
// executes code based on command
void handleCommand(const Command &command, const std::vector<std::string> &args){
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
            createFile(command, args);
            break;
        case rm:
            removeFile(command, args);
            break;
        case mkdir:
            makeDir(args);
            break;
        case rmdir:
            removeDir(command, args);
            break;
        case undefined:
            std::cout << "Please provide a valid command." << std::endl;
            break;
    }
}

// returns an enum form of the command for the switch statement in the handleCommand()
Command decodeCommand(const std::string &command){
    static const std::unordered_map<std::string, Command> commandList = {
        {"exit", exit_cli},
        {"cd", cd},
        {"ls", ls},
        {"pwd", pwd},
        {"echo", echo},
        {"touch", touch},
        {"rm", rm},
        {"mkdir", mkdir},
        {"rmdir", rmdir},
        {"clear", clear}
    };

    auto it = commandList.find(command);
    if(it != commandList.end()){
        return it->second;
    }

    return undefined;
}

// ~~~~ execution functions ~~~~
//cd command
void changeDir(const std::filesystem::path &path){
    if(std::filesystem::exists(path)){
        std::filesystem::current_path(path);
    }
}

// ls command
void listFiles(const std::filesystem::path &path){
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

// touch command
void createFile(const Command &command, const std::vector<std::string> &filenames){
    if(filenames.size() > 0){
        for(const std::string& filename : filenames){
            std::ofstream file(filename);

            file.close();
        }
    } else {
        throwError(command, "missing file operand");
    }
}

// rm command
void removeFile(const Command& command, const std::vector<std::string>& filenames){
    if(filenames.size() > 0){
        for(const std::string& filename : filenames){
            std::filesystem::remove(filename);
        }
    } else {
        throwError(command, "missing operand");
    }
}

// mkdir command
void makeDir(const std::vector<std::string> &filenames){
    for(const std::string& filename : filenames){
        std::filesystem::create_directory(filename);
    }
}

// rmdir command
void removeDir(const Command& command, const std::vector<std::string> &filenames){
    if(filenames.size() <= 0){
        throwError(command, "missing operand");
        return;
    }

    for(const std::string &filename : filenames){
        std::filesystem::remove(filename);
    }
}

// throwing errors
void throwError(const Command& command, const std::string& err){
    std::cerr << command << ": " << err << std::endl;
}

// clear command
void clearScreen(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
#endif
}
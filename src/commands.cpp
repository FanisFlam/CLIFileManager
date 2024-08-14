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
            changeDir(args[0]);
            break;
        case ls:
            if(args.size() > 1 && std::filesystem::exists(args[0]) && args[0] != "")
                listFiles(args[0]);
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
            makeDir(command, args);
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
    if(args.size() == 0){
        std::cout << std::endl;
        return;
    }

    std::ostringstream text;
    for (size_t i = 0; i < args.size(); i++)
    {
        if(i > 0){
            text << " ";
        }
        text << args[i];
    }
    std::cout << text.str() << std::endl;
}

// touch command
void createFile(const Command &command, const std::vector<std::string> &filenames){
    // if user doesn't provide files to be created
    if(filenames.size() <= 0){
        throwError(command, "missing file operand");
        return;
    }

    for(const std::string& filename : filenames){
        std::ofstream file(filename);

        file.close();
    }
}

// rm command
void removeFile(const Command& command, const std::vector<std::string>& filenames){
    // if user doesn't provide files to be deleted
    if(filenames.size() <= 0){
        throwError(command, "missing operand");
    }

    // delete each file if exists
    for(const std::string& filename : filenames){
        try {
            if(!std::filesystem::exists(filename)){
                throwError(command, "cannot remove '" + filename + "': No such file or directory");
                continue;
            }
            
            if(std::filesystem::is_directory(filename)){
                throwError(command, "cannot remove '" + filename + "': Is a directory");
                continue;
            }

            std::filesystem::remove(filename);
        } catch(const std::filesystem::filesystem_error& e){
            throwError(command, "Failed to remove " + filename + ": " + e.what());
        }
    }
}

// mkdir command
void makeDir(const Command& command, const std::vector<std::string> &dirnames){
    // if user doesn't provide directories to be created
    if(dirnames.size() <= 0){
        throwError(command, "missing operand");
    }

    for(const std::string& dirname : dirnames){
        if(!std::filesystem::exists(dirname)){
            throwError(command, "failed to remove '" + dirname + "': No such file or directory");
            continue;
        }
        
        std::filesystem::create_directory(dirname);
    }
}

// rmdir command
void removeDir(const Command& command, const std::vector<std::string> &dirnames){
    // if user doesn't provide directories to be deleted
    if(dirnames.size() <= 0){
        throwError(command, "missing operand");
        return;
    }

    for(const std::string &dirname : dirnames){
        if(!std::filesystem::is_directory(dirname)){
            throwError(command, "failed to remove '" + dirname + "': Not a directory");
            continue;
        }

        std::filesystem::remove(dirname);
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
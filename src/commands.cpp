#include "commands.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

#include "utilities.h"

// ~~~~ handling command functions ~~~~
// executes code based on command
void handleCommand(const std::string &command, const std::vector<std::string> &args, const std::vector<char> &flags){
    switch(decodeCommand(command)){
        case exit_cli:
            exitCli = true;
            break;
        case cd:
            changeDir(command, args);
            break;
        case ls:
            if(args.size() > 1 && std::filesystem::exists(args[0]) && args[0] != "")
                listFiles(args[0], flags);
            else
                listFiles(std::filesystem::current_path(), flags);
            break;
        case pwd:
            std::cout << std::filesystem::current_path().string() << std::endl;
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
        case mv:
            move(command, args);
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
        {"mv", mv},
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
void changeDir(const std::string &command, const std::vector<std::string> &args){
    if(args.size() > 1){
        throwError(command, "too many arguments");
        return;
    }

    const std::filesystem::path path = args[0];
    if(std::filesystem::exists(path)){
        std::filesystem::current_path(path);
    }
}

// ls command
void listFiles(const std::filesystem::path &path, const std::vector<char> &flags){
    for (const auto &entry : std::filesystem::directory_iterator(path)){
        if (entry.path().filename().string()[0] != '.' || containsFlag(flags, 'a')){
           std::cout << entry.path().filename().string() << "  ";        
        }
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
void createFile(const std::string &command, const std::vector<std::string> &filenames){
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
void removeFile(const std::string &command, const std::vector<std::string>& filenames){
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
void makeDir(const std::string &command, const std::vector<std::string> &dirnames){
    // if user doesn't provide directories to be created
    if(dirnames.size() <= 0){
        throwError(command, "missing operand");
    }

    for(const std::string& dirname : dirnames){
        std::filesystem::create_directory(dirname);
    }
}

// rmdir command
void removeDir(const std::string &command, const std::vector<std::string> &dirnames){
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

// mv command
void move(const std::string &command, const std::vector<std::string> &args){
    try {
        if(args.size() <= 0){
            throwError(command, "missing file operand");
            return;
        }

        if(args.size() == 1){
            throwError(command, "missing destination file operand after '" + args[0] + "'");
            return;
        }

        for(const auto& item : args){
            if (!std::filesystem::exists(item)){
                throwError(command, "cannot move '" + item + "': No such file or directory");
                return;
            }
        }

        std::filesystem::path destination = args[args.size() - 1];

        std::filesystem::path source;
        for (int i = 0; i < args.size() - 1; i++){
            if(std::filesystem::is_directory(destination)){
                destination = args[args.size() - 1];
                source = args[i];
                destination /= source.filename();
            } else {
                source = args[i];
            }
            std::filesystem::rename(source, destination);
        }
    } catch(const std::filesystem::filesystem_error& e){
        throwError(command, e.what());
    }
}

// throwing errors
void throwError(const std::string &command, const std::string& err){
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
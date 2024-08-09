#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <filesystem>
#include <vector>

extern bool exitCli;

enum Command
{
    exit_cli,
    cd,
    ls,
    pwd,
    echo,
    touch,
    rm,
    mkdir,
    rmdir,
    clear,
    undefined
};

// handling the command
void handleCommand(const Command& command, const std::vector<std::string>& args);
Command decodeCommand(const std::string& command);

// execution functions
void changeDir(const std::filesystem::path& path);
void listFiles(const std::filesystem::path& path);
void printToScreen(const std::vector<std::string>& args);

// file commands
void createFile(const std::vector<std::string>& filenames);
void removeFile(const std::vector<std::string> &args);

// clear screen
void clearScreen();

#endif
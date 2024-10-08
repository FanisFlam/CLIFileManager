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
    mv,
    clear,
    undefined
};

// handling the command
void handleCommand(const std::string &command, const std::vector<std::string> &args, const std::vector<char> &flags);
Command decodeCommand(const std::string &command);

// execution functions
void changeDir(const std::string &command, const std::vector<std::string> &args);
void listFiles(const std::filesystem::path &path, const std::vector<char> &flags);
void printToScreen(const std::vector<std::string> &args);

// file commands
void createFile(const std::string &command, const std::vector<std::string> &filenames);
void removeFile(const std::string &command, const std::vector<std::string> &args);

// directory commands
void makeDir(const std::string &command, const std::vector<std::string> &dirnames);
void removeDir(const std::string &command, const std::vector<std::string> &dirnames);

// move command
void move(const std::string &command, const std::vector<std::string> &args);

// command errors
void throwError(const std::string &command, const std::string& err);

// clear screen
void clearScreen();

#endif
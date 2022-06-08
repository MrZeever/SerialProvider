#pragma once
#ifndef DATABASE_PROVIDER_H // include guard
#define DATABASE_PROVIDER_H
#include <string>
#include <iostream>
#include <windows.h>
#include <future>
#include <fstream>
#include <thread>
#include <filesystem>

namespace database_provider
{
    class DatabaseProvider
    {
    public:
        DatabaseProvider();
        bool getFile();
        bool checkVersion();
        bool pushFile();
        void checkCurrentFolder(std::string& loc);
        std::string getFileLocation();

    private:
        bool fileExist(const std::string& name);
        const std::string serialFolderName{ "SerialBase" };
        std::string file_location;
        const std::string binaryName{ "AntechSolventTrackerOne.ino.bin" };
    };
}

#endif /* DATABASE_PROVIDER_H */
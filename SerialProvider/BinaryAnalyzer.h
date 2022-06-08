#pragma once
#ifndef BINARY_ANALYZER_H // include guard
#define BINARY_ANALYZER_H
#define NO_OF_CHARS 16384
#include <string>
#include <iostream>
#include <windows.h>
#include <future>
#include <fstream>
#include <thread>
#include <filesystem>
#include <sstream>
#include <iomanip>
# define NO_OF_CHARS 2048

namespace binary_analyzer
{
    class SerialCreator
    {
    public:
        SerialCreator(std::string& file_location_);
        std::string getLastSerial();
        bool addToBase(const std::string& str);
        bool addtoLast(const std::string& str);
        std::string createSerial(std::string& last);
        std::vector<std::string> baseList();

    private:
        bool checkIfFileExists(const std::string& path);
        std::string to_format(const int number);
        std::string& file_location;
        const std::string folderBase{ "\\SerialBase" };
        const std::string fileBase{ "\\Base.txt" };
        const std::string fileLast{ "\\Last.txt" };

    };

    class BinaryAnalyzer
    {
    public:
        BinaryAnalyzer(std::string& file_location_);
        int findPattern(std::string& primaryPatter, const std::vector<std::string>& backupList);
        bool pushSerial(const int& index, std::string& pattern);
        bool getBinaryFile();

    private:
        std::string binary_file_location;
        void badCharHeuristic(std::string str, int size, int badchar[NO_OF_CHARS]);
        int search(std::vector<unsigned char>, std::string pat);
        std::vector<unsigned char> buffer;
        const std::string defaultPattern{ "TOREPLACE" };

    };
}

#endif /* DATABASE_PROVIDER_H */
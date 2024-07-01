#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include "DataLogger.h"

#include "spdlog/spdlog.h"

namespace fs = std::filesystem;

DataLogger::DataLogger(std::string _baseDir,
                       std::string _prefix,
                       std::string _extension): baseDir(std::move(_baseDir)),
                                                prefix(std::move(_prefix)),
                                                extension(std::move(_extension)) {
}

void DataLogger::setup() {
    spdlog::info("Setting up data logger");

    ensureBaseDirectoryExists();
    filename = getNextLogFileName();
    spdlog::info("Data log file: {}", filename);

    spdlog::info("Data logger setup completed");
}

void DataLogger::writeToFile(std::string message) const {
    std::ofstream outFile(filename, std::ios_base::app);

    if (!outFile.is_open()) {
        spdlog::error("Error opening file: {}", filename);
        return;
    }

    //for (const auto &message: messages) {
    outFile << message << std::endl;
    //}

    outFile.close();
}

// Private
std::string DataLogger::getNextLogFileName() const {
    int count = 0;
    for (const auto &entry: fs::directory_iterator(baseDir)) {
        if (entry.is_regular_file()) {
            if (std::string filename = entry.path().filename().string();
                filename.find(prefix) == 0 &&
                filename.find(extension) == filename.size() - extension.size()) {
                ++count;
            }
        }
    }

    return baseDir + "/" + prefix + std::to_string(count) + extension;
}

void DataLogger::ensureBaseDirectoryExists() const {
    if (!fs::exists(baseDir)) {
        spdlog::info("Base directory \"{}\" does not exist. Creating.", baseDir);

        fs::create_directories(baseDir);
    }
}

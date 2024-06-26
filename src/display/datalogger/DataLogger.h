#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <string>


class DataLogger {
    std::string baseDir;
    std::string prefix;
    std::string extension;
    std::string filename;

    std::string getNextLogFileName() const;

    void ensureBaseDirectoryExists() const;

public:
    DataLogger(std::string _baseDir, std::string _prefix, std::string _extension);

    void setup();

    void writeToFile(const std::string &message) const;
};


#endif //DATALOGGER_H

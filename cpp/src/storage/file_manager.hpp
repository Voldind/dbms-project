#pragma once
#include <string>

class Database;

enum class FileHeaderType {
    ARRAY,
    FORWARD_LIST,
    DOUBLY_LIST,
    STACK,
    QUEUE,
    TREE,
    UNKNOWN
};

class FileManager {
public:
    static bool loadFromFile(const std::string& filename, Database& db);
    static bool saveToFile(const std::string& filename, const Database& db);

private:
    static FileHeaderType stringToHeader(const std::string& header_str);
};

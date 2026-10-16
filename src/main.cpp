#include <iostream>
#include <string>

#include "parser/query_parser.hpp"
#include "storage/database.hpp"
#include "storage/file_manager.hpp"

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "Необходимо минимум 5 аргументов: " << std::endl;
        std::cout << "./dbms --file file.data --query запрос" << std::endl;
        return 1;
    }
    std::string file_path = "";
    std::string query_str = "";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            file_path = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query_str = argv[++i];
        }
    }
    if (file_path.empty() || query_str.empty()) {
        std::cout << "Ошибка: нехваатет аргументов для --file или --querry";
        return 1;
    }
    Database db;
    FileManager::loadFromFile(file_path, db);
    Query parsed_query = QueryParser::parse(query_str);
    db.executeQuery(parsed_query);
    FileManager::saveToFile(file_path, db);
    return 0;
}
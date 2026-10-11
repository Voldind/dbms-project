#include "parser/query_parser.hpp"
#include "storage/database.hpp"
#include <iostream>
#include <string>

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
    // TODO: вызов загрузки данных из файла
    Query parsed_query = QueryParser::parse(query_str);
    db.executeQuery(parsed_query);
    // TODO: вызов сохранения данных в файл
    // db.saveToFile(file_path);
    return 0;
}
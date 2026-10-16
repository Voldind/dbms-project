#include "file_manager.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "database.hpp"

using json = nlohmann::json;

bool FileManager::loadFromFile(const std::string& filename, Database& db) {
    std::ifstream file(filename);
    if (!file.is_open()) return true;

    try {
        json j;
        file >> j;

        db.clearAll();

        // Загрузка динамических массивов
        if (j.contains("arrays") && j["arrays"].is_object()) {
            for (auto& [name, arr_json] : j["arrays"].items()) {
                for (const auto& item : arr_json) {
                    db.getArrays()[name].push_back(item);
                }
            }
        }
        // Загрузка односвязных списков
        if (j.contains("forward_lists") && j["forward_lists"].is_object()) {
            for (auto& [name, list_json] : j["forward_lists"].items()) {
                for (const auto& item : list_json) {
                    db.getForwardLists()[name].push_back(item);
                }
            }
        }
        // Загрузка двусвязных списков
        if (j.contains("doubly_lists") && j["doubly_lists"].is_object()) {
            for (auto& [name, list_json] : j["doubly_lists"].items()) {
                for (const auto& item : list_json)
                    db.getDoublyLists()[name].push_back(item);
            }
        }
        // Загрузка стеков
        if (j.contains("stacks") && j["stacks"].is_object()) {
            for (auto& [name, list_json] : j["stacks"].items()) {
                std::vector<std::string> temp = list_json;
                for (auto it = temp.rbegin(); it != temp.rend(); ++it)
                    db.getStacks()[name].push(*it);
            }
        }
        // Загрузка очередей
        if (j.contains("queues") && j["queues"].is_object()) {
            for (auto& [name, list_json] : j["queues"].items()) {
                for (const auto& item : list_json)
                    db.getQueues()[name].push(item);
            }
        }
        // Загрузка деревьев
        if (j.contains("trees") && j["trees"].is_object()) {
            for (auto& [name, list_json] : j["trees"].items()) {
                for (const auto& item : list_json)
                    db.getTrees()[name].insert(item);
            }
        }

    } catch (const std::exception& e) {
        std::cout << "Error parsing JSON file: " << e.what() << std::endl;
        return false;
    }
    file.close();
    return true;
}

bool FileManager::saveToFile(const std::string& filename, const Database& db) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) return false;

    json j;

    // Сохранение массивов
    j["arrays"] = json::object();  // Гарантируем, что это будет JSON-объект {}
    for (const auto& [name, arr] : db.getArrays()) {
        j["arrays"][name] = arr.to_vector();
    }
    // Сохранение односвязных списокв
    j["forward_lists"] = json::object();
    for (const auto& [name, flist] : db.getForwardLists()) {
        j["forward_lists"][name] = flist.to_vector();
    }
    // Сохранение двусвязных списокв
    j["doubly_lists"] = json::object();
    for (const auto& [name, l] : db.getDoublyLists())
        j["doubly_lists"][name] = l.to_vector();
    // Сохранение стеков
    j["stacks"] = json::object();
    for (const auto& [name, s] : db.getStacks())
        j["stacks"][name] = s.to_vector();
    // Сохранение очередей
    j["queues"] = json::object();
    for (const auto& [name, q] : db.getQueues())
        j["queues"][name] = q.to_vector();
    // Сохранение деревьев
    j["trees"] = json::object();
    for (const auto& [name, t] : db.getTrees())
        j["trees"][name] = t.to_vector();

    file << j.dump(4) << std::endl;
    file.close();
    return true;
}
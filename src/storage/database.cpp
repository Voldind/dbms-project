#include "database.hpp"

#include <iostream>

void Database::executeQuery(const Query& q) {
    switch (q.command_type) {
        // --- МАССИВ ---
        case CommandType::MPUSH:
            if (q.has_index) {
                m_array.insert(q.index, q.value);
            } else {
                m_array.push_back(q.value);
            }
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::MGET:
            try {
                std::cout << "-> " << m_array[q.index] << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::MDEL:
            try {
                m_array.remove(q.index);
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        // --- СТЕК ---
        case CommandType::SPUSH:
            m_stack.push(q.value);
            std::cout << "-> " << q.value << std::endl;
            break;

        case CommandType::SPOP:
            try {
                std::string top_val = m_stack.top();
                m_stack.pop();
                std::cout << "-> " << top_val << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        // --- ОБЩИЕ ---
        case CommandType::PRINT:
            printAll();
            break;

        case CommandType::UNKNOWN:
        default:
            std::cout << "Error: Unknown or malformed command '"
                      << q.command_raw << "'!" << std::endl;
            break;
    }
}

void Database::printAll() {
    std::cout << "--- Current DBMS State ---" << std::endl;
    std::cout << "Array: ";
    m_array.print();
    std::cout << "Stack: ";
    m_stack.print();
}

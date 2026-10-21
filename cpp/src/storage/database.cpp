#include "database.hpp"

#include <iostream>

void Database::clearAll() {
    m_arrays.clear();
    m_forward_lists.clear();
    m_doubly_lists.clear();
    m_stacks.clear();
    m_queues.clear();
    m_trees.clear();
}

void Database::executeQuery(const Query& q) {
    switch (q.command_type) {
        // ==========================================
        // --- 1. МАССИВ (M) ---
        // ==========================================
        case CommandType::MPUSH:
            if (q.has_index) {
                try {
                    m_arrays[q.db_name].insert(q.index, q.value);
                    std::cout << "-> OK" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "-> Error: " << e.what() << std::endl;
                }
            } else {
                m_arrays[q.db_name].push_back(q.value);
                std::cout << "-> OK" << std::endl;
            }
            break;

        case CommandType::MGET:
            if (!q.has_index) {
                std::cout << "-> Error: MGET requires an index!" << std::endl;
                break;
            }
            if (m_arrays.find(q.db_name) == m_arrays.end()) {
                std::cout << "-> Error: Array '" << q.db_name
                          << "' does not exist!" << std::endl;
                break;
            }
            try {
                std::cout << "-> " << m_arrays[q.db_name][q.index] << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::MDEL:
            if (!q.has_index) {
                std::cout << "-> Error: MDEL requires an index!" << std::endl;
                break;
            }
            if (m_arrays.find(q.db_name) == m_arrays.end()) {
                std::cout << "-> Error: Array '" << q.db_name
                          << "' does not exist!" << std::endl;
                break;
            }
            try {
                m_arrays[q.db_name].remove(q.index);
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::MSET:
            if (!q.has_index || q.value.empty()) {
                std::cout << "-> Error: Invalid MSET syntax!" << std::endl;
                break;
            }
            if (m_arrays.find(q.db_name) == m_arrays.end()) {
                std::cout << "-> Error: Array '" << q.db_name
                          << "' does not exist!" << std::endl;
                break;
            }
            try {
                m_arrays[q.db_name][q.index] = q.value;
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::MSIZE:
            std::cout << "-> Size: "
                      << (m_arrays.find(q.db_name) != m_arrays.end()
                              ? m_arrays[q.db_name].size()
                              : 0)
                      << std::endl;
            break;

        case CommandType::MCLEAR:
            m_arrays.erase(q.db_name);
            std::cout << "-> Array '" << q.db_name << "' cleared and deleted"
                      << std::endl;
            break;
        // ==========================================
        // --- 2. ОДНОСВЯЗНЫЙ СПИСОК (F) ---
        // ==========================================
        case CommandType::FPUSHHEAD:
            m_forward_lists[q.db_name].push_front(q.value);
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::FPUSHTAIL:
            m_forward_lists[q.db_name].push_back(q.value);
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::FADD:
            if (!q.target_value.empty()) {
                try {
                    m_forward_lists[q.db_name].insert_after(q.target_value,
                                                            q.value);
                    std::cout << "-> OK" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "-> Error: " << e.what() << std::endl;
                }
            } else {
                m_forward_lists[q.db_name].push_back(q.value);
                std::cout << "-> OK" << std::endl;
            }
            break;

        case CommandType::FADDBEFORE:
            if (q.target_value.empty()) {
                std::cout
                    << "-> Error: FADDBEFORE requires target_value and value!"
                    << std::endl;
                break;
            }
            try {
                m_forward_lists[q.db_name].insert_before(q.target_value,
                                                         q.value);
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::FDELHEAD:
            if (m_forward_lists.find(q.db_name) == m_forward_lists.end() ||
                m_forward_lists[q.db_name].empty()) {
                std::cout << "-> Error: List is empty or does not exist!"
                          << std::endl;
                break;
            }
            m_forward_lists[q.db_name].pop_front();
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::FDELTAIL:
            if (m_forward_lists.find(q.db_name) == m_forward_lists.end() ||
                m_forward_lists[q.db_name].empty()) {
                std::cout << "-> Error: List is empty or does not exist!"
                          << std::endl;
                break;
            }
            m_forward_lists[q.db_name].pop_back();
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::FDEL:
            if (q.target_value.empty()) {
                std::cout << "-> Error: FDEL requires target value after which "
                             "to delete!"
                          << std::endl;
                break;
            }
            if (m_forward_lists.find(q.db_name) == m_forward_lists.end()) {
                std::cout << "-> Error: List does not exist!" << std::endl;
                break;
            }
            try {
                m_forward_lists[q.db_name].remove_after(q.target_value);
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::FDELBEFORE:
            if (q.target_value.empty()) {
                std::cout << "-> Error: FDELBEFORE requires target value "
                             "before which to delete!"
                          << std::endl;
                break;
            }
            if (m_forward_lists.find(q.db_name) == m_forward_lists.end()) {
                std::cout << "-> Error: List does not exist!" << std::endl;
                break;
            }
            try {
                m_forward_lists[q.db_name].remove_before(q.target_value);
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::FFIND:  // Поиск индекса по значению
            if (m_forward_lists.find(q.db_name) == m_forward_lists.end()) {
                std::cout << "-> Index: -1 (Not found)" << std::endl;
                break;
            }
            {
                size_t idx = m_forward_lists[q.db_name].find(q.value);
                if (idx == static_cast<size_t>(-1))
                    std::cout << "-> Index: -1 (Not found)" << std::endl;
                else
                    std::cout << "-> Index: " << idx << std::endl;
            }
            break;

        case CommandType::FVALDEL:  // Удаление первого вхождения по значению
            if (m_forward_lists.find(q.db_name) == m_forward_lists.end()) {
                std::cout << "-> False (Not found)" << std::endl;
                break;
            }
            {
                bool deleted =
                    m_forward_lists[q.db_name].remove_by_value(q.value);
                std::cout << "-> " << (deleted ? "OK" : "False (Not found)")
                          << std::endl;
            }
            break;

        case CommandType::FGET:  // Получение по индексу
            if (!q.has_index) {
                std::cout << "-> Error: FGET requires an index!" << std::endl;
                break;
            }
            if (m_forward_lists.find(q.db_name) == m_forward_lists.end()) {
                std::cout << "-> Error: List does not exist!" << std::endl;
                break;
            }
            try {
                std::cout << "-> " << m_forward_lists[q.db_name][q.index]
                          << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::FSIZE:
            std::cout << "-> Size: "
                      << (m_forward_lists.find(q.db_name) !=
                                  m_forward_lists.end()
                              ? m_forward_lists[q.db_name].size()
                              : 0)
                      << std::endl;
            break;

        case CommandType::FCLEAR:
            m_forward_lists.erase(q.db_name);
            std::cout << "-> List '" << q.db_name << "' cleared and deleted"
                      << std::endl;
            break;
        // ==========================================
        // --- 3. ДВУСВЯЗНЫЙ СПИСОК (L) ---
        // ==========================================
        case CommandType::LPUSHHEAD:
            m_doubly_lists[q.db_name].push_front(q.value);
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::LPUSHTAIL:
            m_doubly_lists[q.db_name].push_back(q.value);
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::LADD:
            if (!q.target_value.empty()) {
                try {
                    m_doubly_lists[q.db_name].insert_after(q.target_value,
                                                           q.value);
                    std::cout << "-> OK" << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "-> Error: " << e.what() << std::endl;
                }
            } else {
                m_doubly_lists[q.db_name].push_back(q.value);
                std::cout << "-> OK" << std::endl;
            }
            break;

        case CommandType::LADDBEFORE:
            if (q.target_value.empty()) {
                std::cout
                    << "-> Error: LADDBEFORE requires target_value and value!"
                    << std::endl;
                break;
            }
            try {
                m_doubly_lists[q.db_name].insert_before(q.target_value,
                                                        q.value);
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::LDELHEAD:
            if (m_doubly_lists.find(q.db_name) == m_doubly_lists.end() ||
                m_doubly_lists[q.db_name].empty()) {
                std::cout << "-> Error: List is empty!" << std::endl;
                break;
            }
            m_doubly_lists[q.db_name].pop_front();
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::LDELTAIL:
            if (m_doubly_lists.find(q.db_name) == m_doubly_lists.end() ||
                m_doubly_lists[q.db_name].empty()) {
                std::cout << "-> Error: List is empty!" << std::endl;
                break;
            }
            m_doubly_lists[q.db_name].pop_back();
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::LDEL:
            if (q.target_value.empty()) {
                std::cout << "-> Error: LDEL requires target value!"
                          << std::endl;
                break;
            }
            try {
                m_doubly_lists[q.db_name].remove_after(q.target_value);
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::LDELBEFORE:
            if (q.target_value.empty()) {
                std::cout << "-> Error: LDELBEFORE requires target value!"
                          << std::endl;
                break;
            }
            try {
                m_doubly_lists[q.db_name].remove_before(q.target_value);
                std::cout << "-> OK" << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::LFIND:
            if (m_doubly_lists.find(q.db_name) == m_doubly_lists.end()) {
                std::cout << "-> Index: -1" << std::endl;
                break;
            }
            {
                size_t idx = m_doubly_lists[q.db_name].find(q.value);
                if (idx == static_cast<size_t>(-1))
                    std::cout << "-> Index: -1" << std::endl;
                else
                    std::cout << "-> Index: " << idx << std::endl;
            }
            break;

        case CommandType::LVALDEL:
            if (m_doubly_lists.find(q.db_name) == m_doubly_lists.end()) {
                std::cout << "-> False" << std::endl;
                break;
            }
            std::cout << "-> "
                      << (m_doubly_lists[q.db_name].remove_by_value(q.value)
                              ? "OK"
                              : "False")
                      << std::endl;
            break;

        case CommandType::LGET:
            if (!q.has_index) {
                std::cout << "-> Error: LGET requires an index!" << std::endl;
                break;
            }
            try {
                std::cout << "-> " << m_doubly_lists[q.db_name][q.index]
                          << std::endl;
            } catch (const std::exception& e) {
                std::cout << "-> Error: " << e.what() << std::endl;
            }
            break;

        case CommandType::LSIZE:
            std::cout << "-> Size: "
                      << (m_doubly_lists.find(q.db_name) != m_doubly_lists.end()
                              ? m_doubly_lists[q.db_name].size()
                              : 0)
                      << std::endl;
            break;

        case CommandType::LCLEAR:
            m_doubly_lists.erase(q.db_name);
            std::cout << "-> List cleared" << std::endl;
            break;

        // ==========================================
        // --- 4. СТЕК (S) ---
        // ==========================================
        case CommandType::SPUSH:
            m_stacks[q.db_name].push(q.value);
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::SPOP:
            if (m_stacks.find(q.db_name) == m_stacks.end() ||
                m_stacks[q.db_name].empty()) {
                std::cout << "-> Error: Stack is empty!" << std::endl;
                break;
            }
            {
                std::string top_val = m_stacks[q.db_name].top();
                m_stacks[q.db_name].pop();
                std::cout << "-> " << top_val << std::endl;
            }
            break;

        case CommandType::SSIZE:
            std::cout << "-> Size: "
                      << (m_stacks.find(q.db_name) != m_stacks.end()
                              ? m_stacks[q.db_name].size()
                              : 0)
                      << std::endl;
            break;

        case CommandType::SCLEAR:
            m_stacks.erase(q.db_name);
            std::cout << "-> Stack cleared" << std::endl;
            break;

        // ==========================================
        // --- 5. ОЧЕРЕДЬ (Q) ---
        // ==========================================
        case CommandType::QPUSH:
            m_queues[q.db_name].push(q.value);
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::QPOP:
            if (m_queues.find(q.db_name) == m_queues.end() ||
                m_queues[q.db_name].empty()) {
                std::cout << "-> Error: Queue is empty!" << std::endl;
                break;
            }
            {
                std::string front_val = m_queues[q.db_name].front();
                m_queues[q.db_name].pop();
                std::cout << "-> " << front_val << std::endl;
            }
            break;

        case CommandType::QSIZE:
            std::cout << "-> Size: "
                      << (m_queues.find(q.db_name) != m_queues.end()
                              ? m_queues[q.db_name].size()
                              : 0)
                      << std::endl;
            break;

        case CommandType::QCLEAR:
            m_queues.erase(q.db_name);
            std::cout << "-> Queue cleared" << std::endl;
            break;

        // ==========================================
        // --- 6. ПОЛНОЕ БИНАРНОЕ ДЕРЕВО (T) ---
        // ==========================================
        case CommandType::TINSERT:
            m_trees[q.db_name].insert(q.value);
            std::cout << "-> OK" << std::endl;
            break;

        case CommandType::TFIND:
            std::cout << "-> "
                      << (m_trees[q.db_name].find(q.value) ? "Found"
                                                           : "Not Found")
                      << std::endl;
            break;

        case CommandType::TCHECK:
            std::cout << "-> Is Complete: "
                      << (m_trees[q.db_name].isComplete() ? "True" : "False")
                      << std::endl;
            break;

        case CommandType::TCLEAR:
            m_trees.erase(q.db_name);
            std::cout << "-> Tree cleared" << std::endl;
            break;

        // ==========================================
        // --- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ---
        // ==========================================
        case CommandType::PRINT:
            printAll();
            break;
        case CommandType::HELP:
            printHelp();
            break;

        default:
            std::cout << "Error: Operation not supported yet or unknown!"
                      << std::endl;
    }
}

void Database::printAll() {
    std::cout << "--- Current DBMS State ---" << std::endl;
    std::cout << "[Arrays]:" << std::endl;
    for (const auto& [name, arr] : m_arrays) {
        std::cout << "  * " << name << " -> ";
        arr.print();
    }
    std::cout << "[Forward Lists]:" << std::endl;
    for (const auto& [name, l] : m_forward_lists) {
        std::cout << "  * " << name << " -> ";
        l.print();
    }
    std::cout << "[Doubly Lists]:" << std::endl;
    for (const auto& [name, l] : m_doubly_lists) {
        std::cout << "  * " << name << " -> ";
        l.print();
    }
    std::cout << "[Stacks]:" << std::endl;
    for (const auto& [name, s] : m_stacks) {
        std::cout << "  * " << name << " -> ";
        s.print();
    }
    std::cout << "[Queues]:" << std::endl;
    for (const auto& [name, q] : m_queues) {
        std::cout << "  * " << name << " -> ";
        q.print();
    }
    std::cout << "[Trees]:" << std::endl;
    for (const auto& [name, t] : m_trees) {
        std::cout << "  * " << name << " -> ";
        t.print();
    }
}

void Database::printHelp() {
    std::cout << "\n==========================================================="
                 "=============="
              << std::endl;
    std::cout << "                      СУБД CLI — СПРАВКА ПО КОМАНДАМ         "
                 "             "
              << std::endl;
    std::cout << "============================================================="
                 "============"
              << std::endl;

    std::cout << "\n[ОБЩИЕ КОМАНДЫ]:" << std::endl;
    std::cout << "  HELP / --help        - Вывести эту справку" << std::endl;
    std::cout << "  PRINT                - Распечатать текущее состояние всех "
                 "контейнеров в БД"
              << std::endl;

    std::cout << "\n[МАССИВ (Префикс M)]:" << std::endl;
    std::cout << "  MPUSH <name> <val>   - Добавить элемент в конец массива"
              << std::endl;
    std::cout << "  MPUSH <name> <idx> <val> - Вставить элемент по индексу"
              << std::endl;
    std::cout << "  MGET  <name> <idx>   - Получить элемент по индексу"
              << std::endl;
    std::cout << "  MSET  <name> <idx> <val> - Заменить элемент по индексу"
              << std::endl;
    std::cout << "  MDEL  <name> <idx>   - Удалить элемент по индексу"
              << std::endl;
    std::cout << "  MSIZE <name>         - Узнать текущий размер массива"
              << std::endl;
    std::cout << "  MCLEAR <name>        - Полностью очистить и удалить массив"
              << std::endl;

    std::cout << "\n[ОДНОСВЯЗНЫЙ СПИСОК (Префикс F)]:" << std::endl;
    std::cout << "  FPUSHHEAD <name> <val>   - Добавить элемент в начало списка"
              << std::endl;
    std::cout << "  FPUSHTAIL <name> <val>   - Добавить элемент в конец списка"
              << std::endl;
    std::cout << "  FADD      <name> <idx> <val> - Вставить элемент ПОСЛЕ "
                 "указанного индекса"
              << std::endl;
    std::cout << "  FADDBEFORE <name> <idx> <val> - Вставить элемент ДО "
                 "указанного индекса"
              << std::endl;
    std::cout << "  FDELHEAD  <name>         - Удалить элемент из начала списка"
              << std::endl;
    std::cout << "  FDELTAIL   <name>        - Удалить элемент из конца списка"
              << std::endl;
    std::cout << "  FDEL      <name> <idx>   - Удалить элемент ПОСЛЕ "
                 "указанного индекса"
              << std::endl;
    std::cout
        << "  FDELBEFORE <name> <idx>  - Удалить элемент ДО указанного index"
        << std::endl;
    std::cout
        << "  FFIND     <name> <val>   - Найти индекс элемента по значению"
        << std::endl;
    std::cout
        << "  FVALDEL   <name> <val>   - Удалить первое вхождение по значению"
        << std::endl;
    std::cout << "  FGET      <name> <idx>   - Получить элемент по индексу"
              << std::endl;
    std::cout << "  FSIZE     <name>         - Получить размер списка"
              << std::endl;
    std::cout << "  FCLEAR    <name>         - Очистить и удалить список"
              << std::endl;

    std::cout << "\n[ДВУСВЯЗНЫЙ СПИСОК (Префикс L)]:" << std::endl;
    std::cout << "  LPUSHHEAD <name> <val>   - Добавить элемент в начало списка"
              << std::endl;
    std::cout << "  LPUSHTAIL <name> <val>   - Добавить элемент в конец списка"
              << std::endl;
    std::cout << "  LADD      <name> <idx> <val> - Вставить элемент ПОСЛЕ "
                 "указанного индекса"
              << std::endl;
    std::cout << "  LADDBEFORE <name> <idx> <val> - Вставить элемент ДО "
                 "указанного индекса"
              << std::endl;
    std::cout << "  LDELHEAD  <name>         - Удалить элемент из начала списка"
              << std::endl;
    std::cout << "  LDELTAIL   <name>        - Удалить элемент из конца списка"
              << std::endl;
    std::cout << "  LDEL      <name> <idx>   - Удалить элемент ПОСЛЕ "
                 "указанного индекса"
              << std::endl;
    std::cout
        << "  LDELBEFORE <name> <idx>  - Удалить элемент ДО указанного index"
        << std::endl;
    std::cout
        << "  LFIND     <name> <val>   - Найти индекс элемента по значению"
        << std::endl;
    std::cout
        << "  LVALDEL   <name> <val>   - Удалить первое вхождение по значению"
        << std::endl;
    std::cout << "  LGET      <name> <idx>   - Получить элемент по индексу"
              << std::endl;
    std::cout << "  LSIZE     <name>         - Получить размер списка"
              << std::endl;
    std::cout << "  LCLEAR    <name>         - Очистить и удалить список"
              << std::endl;

    std::cout << "\n[СТЕК (Префикс S)]:" << std::endl;
    std::cout << "  SPUSH  <name> <val>  - Поместить элемент на вершину стека"
              << std::endl;
    std::cout << "  SPOP   <name>        - Извлечь и удалить элемент с вершины "
                 "стека (LIFO)"
              << std::endl;
    std::cout << "  SSIZE  <name>        - Получить текущий размер стека"
              << std::endl;
    std::cout << "  SCLEAR <name>        - Полностью очистить стек"
              << std::endl;

    std::cout << "\n[ОЧЕРЕДЬ (Префикс Q)]:" << std::endl;
    std::cout << "  QPUSH  <name> <val>  - Добавить элемент в конец очереди"
              << std::endl;
    std::cout << "  QPOP   <name>        - Извлечь и удалить элемент из начала "
                 "очереди (FIFO)"
              << std::endl;
    std::cout << "  QSIZE  <name>        - Получить текущий размер очереди"
              << std::endl;
    std::cout << "  QCLEAR <name>        - Полностью очистить очередь"
              << std::endl;

    std::cout << "\n[ПОЛНОЕ БИНАРНОЕ ДЕРЕВО (Префикс T)]:" << std::endl;
    std::cout << "  TINSERT <name> <val> - Вставить элемент в дерево "
                 "(заполнение по уровням)"
              << std::endl;
    std::cout << "  TFIND   <name> <val> - Проверить наличие элемента в дереве"
              << std::endl;
    std::cout << "  TCHECK  <name>       - Честная проверка структуры на "
                 "свойство Complete"
              << std::endl;
    std::cout << "  TCLEAR  <name>       - Полностью очистить и удалить дерево"
              << std::endl;

    std::cout << "============================================================="
                 "============\n"
              << std::endl;
}
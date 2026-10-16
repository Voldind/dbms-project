#pragma once
#include <string>
#include <unordered_map>

#include "../parser/query_parser.hpp"
#include "../structures/array.hpp"
#include "../structures/doubly_list.hpp"
#include "../structures/forward_list.hpp"
#include "../structures/queue.hpp"
#include "../structures/stack.hpp"
#include "../structures/tree.hpp"

class Database {
   private:
    std::unordered_map<std::string, DynamicArray<std::string>> m_arrays;
    std::unordered_map<std::string, ForwardList<std::string>> m_forward_lists;
    std::unordered_map<std::string, DoublyList<std::string>> m_doubly_lists;
    std::unordered_map<std::string, Stack<std::string>> m_stacks;
    std::unordered_map<std::string, Queue<std::string>> m_queues;
    std::unordered_map<std::string, CompleteBinaryTree<std::string>> m_trees;

   public:
    Database() = default;
    void executeQuery(const Query& q);
    void clearAll();

    auto& getArrays() { return m_arrays; }
    auto& getForwardLists() { return m_forward_lists; }
    auto& getDoublyLists() { return m_doubly_lists; }
    auto& getStacks() { return m_stacks; }
    auto& getQueues() { return m_queues; }
    auto& getTrees() { return m_trees; }

    const auto& getArrays() const { return m_arrays; }
    const auto& getForwardLists() const { return m_forward_lists; }
    const auto& getDoublyLists() const { return m_doubly_lists; }
    const auto& getStacks() const { return m_stacks; }
    const auto& getQueues() const { return m_queues; }
    const auto& getTrees() const { return m_trees; }

   private:
    void printHelp();
    void printAll();
};

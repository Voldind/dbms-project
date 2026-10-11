#pragma once
#include "../parser/query_parser.hpp"
#include "../structures/array.hpp"
#include "../structures/doubly_list.hpp"
#include "../structures/forward_list.hpp"
#include "../structures/queue.hpp"
#include "../structures/stack.hpp"
#include "../structures/tree.hpp"

class Database {
   private:
    DynamicArray<std::string> m_array;
    ForwardList<std::string> m_forward_list;
    DoublyList<std::string> m_doubly_list;
    Stack<std::string> m_stack;
    Queue<std::string> m_queue;
    CompleteBinaryTree<std::string> m_tree;

   public:
    Database() = default;
    void executeQuery(const Query& q);

   private:
    void printAll();
};

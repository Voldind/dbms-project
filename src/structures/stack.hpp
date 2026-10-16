#pragma once
#include <stdexcept>
#include <vector>

#include "forward_list.hpp"

template <typename T>
class Stack {
   private:
    ForwardList<T> m_list;  // список - хранилище стека

   public:
    Stack() = default;
    ~Stack() { clear(); }
    void clear() { m_list.clear(); }
    void push(const T& value) { m_list.push_front(value); }
    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty! Cannot pop.");
        }
        m_list.pop_front();
    }
    T& top() {
        if (empty()) {
            throw std::underflow_error("Stack is empty! Cannot read top.");
        }
        return m_list[0];
    }
    const T& top() const {
        if (empty()) {
            throw std::underflow_error("Stack is empty! Cannot read top.");
        }
        return m_list[0];
    }
    bool empty() const { return m_list.empty(); }
    size_t size() const { return m_list.size(); }
    void print() const { m_list.print(); }
    std::vector<T> to_vector() const { return m_list.to_vector(); }
};
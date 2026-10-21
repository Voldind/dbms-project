#pragma once
#include <stdexcept>
#include <vector>

#include "forward_list.hpp"

template <typename T>
class Queue {
   private:
    ForwardList<T> m_list;

   public:
    Queue() = default;
    ~Queue() { clear(); }
    void clear() { m_list.clear(); }
    void push(const T& value) { m_list.push_back(value); }
    void pop() {
        if (empty()) {
            throw std::underflow_error(
                "Queue is empty! Cannot pop");
        }
        m_list.pop_front();
    }
    T& front() {
        if (empty()) {
            throw std::underflow_error("Queue is empty! Cannot read front");
        }
        return m_list[0];
    }
    const T& front() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty! Cannot read front");
        }
        return m_list[0];
    }
    bool empty() const { return m_list.empty(); }
    size_t size() const { return m_list.size(); }
    void print() const { m_list.print(); }
    std::vector<T> to_vector() const { return m_list.to_vector(); }
};
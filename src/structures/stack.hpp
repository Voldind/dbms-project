#pragma once
#include <stdexcept>

#include "forward_list.hpp"

template <typename T>
class Stack {
   private:
    ForwardList<T> m_list;  // список - хранилище стека

   public:
    // конструктор
    Stack() = default;

    // добавление элемента в стек
    void push(const T& value) { m_list.push_front(value); }

    // удаление элемента из стека
    void pop() {
        if (empty()) {
            throw std::underflow_error("Стек пустой. Удаление невозможно");
        }
        m_list.pop_front();
    }

    // чтение верхнего элемента
    T& top() {
        if (empty()) {
            throw std::underflow_error("Стек пустой. Чтение невозможно");
        }
        return m_list[0];
    }
    const T& top() const {
        if (empty()) {
            throw std::underflow_error("Стек пустой. Чтение невозможно");
        }
        return m_list[0];
    }

    // проверка на пустоту
    bool empty() const {
        return m_list.empty();
    }

    // получение длины стека
    size_t size() const {
        return m_list.size();
    }

    //вывод стека на экран
    void print() const {
        m_list.print();
    }
};
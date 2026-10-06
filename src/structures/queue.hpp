#pragma once
#include <stdexcept>

#include "forward_list.hpp"

template <typename T>
class Queue {
   private:
    ForwardList<T> m_list;

   public:
    // конструктор
    Queue() = default;

    // добавление элемента в конец
    void push(const T& value) { m_list.push_back(value); }

    // чтение и удаление элемента с начала
    void pop() {
        if (empty()) {
            throw std::underflow_error(
                "Очередь пуста. Невозможно удалить элемент");
        }
        m_list.pop_front();
    }

    // чтение первого элемента очереди
    T& front() {
        if (empty()) {
            throw std::underflow_error("Очередь пуста. Невозможно прочитать");
        }
        return m_list[0];
    }
    const T& front() const {
        if (empty()) {
            throw std::underflow_error("Очередь пуста. Невозможно прочитать");
        }
        return m_list[0];
    }

    // проверка на пустоту
    bool empty() const { return m_list.empty(); }

    // получение длины очереди
    size_t size() const { return m_list.size(); }

    // вывод очереди на экран
    void print() const { m_list.print(); }
};
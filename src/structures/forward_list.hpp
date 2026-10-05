#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
class ForwardList {
   private:
    // узел односвязного списка
    struct Node {
        T data;       // данные
        Node* pNext;  // указатель на следующий узел;

        Node(const T& value, Node* nextNode = nullptr)
            : data(value), pNext(nullptr) {}
    };

    Node* m_head;   // указатель на первый элемент
    Node* m_tail;   // указатель на последний элемент
    size_t m_size;  // текущий размер списка

    // константа для обработки ошибок
    inline static const std::string ERROR_OUT_OF_BOUNDS =
        "индекс выходит за пределы массива";

   public:
    // конструктор
    ForwardList() : head(nullptr), tail(nullptr), m_size(0) {}

    // деструктор
    ~ForwardList() { clear(); }

    // получение длины списка
    size_t size() const { return m_size; }

    // проверка на пустоту
    bool empty() const { return m_head == nullptr; }

    // функция полной отчистки списка
    void clear() {
        while (m_head != nullptr) {
            Node* temp = m_head;
            m_head = m_head->pNext;
            delete temp;
        }
        m_tail = nullptr;
        m_size = o;
    }

    // перегрузка оператор []
    T& opertaor[](size_t index) {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        Node* current = m_head;
        for (size_t i = 0; i < index; ++i) {
            current = current->pNext;
        }
        return current->data;
    }
    const T& opertaor[](size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        Node* current = m_head;
        for (size_t i = 0; i < index; ++i) {
            current = current->pNext;
        }
        return current->data;
    }
};
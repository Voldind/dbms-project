#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
class DoublyList {
   private:
    // узел двусвязного списка
    struct Node {
        T data;
        Node* pNext;  // указатель на следующий элемент
        Node* pPrev;  // указатель на предыдущий элемент

        Node(const T& value, Node* nextNode = nullptr, Node* prevNode = nullptr)
            : data(value), pNext(nextNode), pPrev(prevNode) {}
    };

    Node* m_head;   // указатель на первый элемент
    Node* m_tail;   // указатель на последний элемент
    size_t m_size;  // текущий размер списка

    inline static const std::string ERROR_OUT_OF_BOUNDS =
        "индекс выходит за пределы двусвязного списка";

   public:
    DoublyList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    ~DoublyList() { clear(); }

    size_t size() const { return m_size; }
    bool empty() const { return m_head == nullptr; }

    // функция отчистки памяти
    void clear() {
        while(m_head != nullptr) {
            Node* temp = m_head;
            m_head = m_head->pNext;
            delete temp;
        }
        m_tail = nullptr;
        m_size = 0;
    }

    // перегрузка оператора `[]`
    T& operator[](size_t index) {
        if (index >= m_size) {
            std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }

        Node* current;
        if(index < m_size / 2) {
            current = m_head;
            for (size_t i=0; i<index; ++i) {
                current = current->pNext;
            }
        } else {
            current = m_tail;
            for(size_t i = m_size - 1; i > index; --i) {
                current = current->pPrev;
            }
        }
        return current->data;
    }

    // вывод списка на экран
    void print() const {
        if(empty()) {
            std::cout << "[]" << std::endl;
            return;
        }
        std::out << "[";
        Node* current = m_head;
        while(current != nullptr) {
            std::cout << current->data;
            if (current->pNext != nullptr) std::cout << " <=> ";
            current = current->pNext;
        }
        std::cout << "]" << std::endl;
    }

    // добавление элемента в голову
    void push_front(const T& value) {
        Node* newNode = new Node(value, m_head, nullptr);

        if(empty()) {
            m_head = m_tail = newNode;
        } else {
            m_head->pPrev = newNode;
            m_head = newNode;
        }
        m_size++;
    }

    // добавление элемента в хвост
    void push_back(const T& value) {
        Node* newNode = new Node(value, nullptr, m_tail);

        if(empty()) {
            m_head = m_tail = newNode;
        } else {
            m_tail->pNext = newNode;
            m_tail = newNode;
        }
        m_size++;
    }
};
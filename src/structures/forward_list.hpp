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
            : data(value), pNext(nextNode) {}
    };

    Node* m_head;   // указатель на первый элемент
    Node* m_tail;   // указатель на последний элемент
    size_t m_size;  // текущий размер списка

    // константа для обработки ошибок
    inline static const std::string ERROR_OUT_OF_BOUNDS =
        "индекс выходит за пределы массива";

   public:
    // конструктор
    ForwardList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

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
        m_size = 0;
    }

    // перегрузка оператор []
    T& operator[](size_t index) {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        Node* current = m_head;
        for (size_t i = 0; i < index; ++i) {
            current = current->pNext;
        }
        return current->data;
    }
    const T& operator[](size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        Node* current = m_head;
        for (size_t i = 0; i < index; ++i) {
            current = current->pNext;
        }
        return current->data;
    }

    // добавление элемента в голову
    void push_front(const T& value) {
        Node* newNode = new Node(value, m_head);
        m_head = newNode;

        if (m_tail == nullptr) {
            m_tail = m_head;
        }

        m_size++;
    }

    // добавление элемента в хвост
    void push_back(const T& value) {
        if (empty()) {
            push_front(value);
            return;
        }
        Node* newNode = new Node(value);
        m_tail->pNext = newNode;
        m_tail = newNode;
        m_size++;
    }

    // вывод списка
    void print() const {
        if (empty()) {
            std::cout << "[]" << std::endl;
            return;
        }
        std::cout << "[";
        Node* current = m_head;
        while (current != nullptr) {
            std::cout << current->data;

            if (current->pNext != nullptr) {
                std::cout << " -> ";
            }
            current = current->pNext;
        }
        std::cout << "]" << std::endl;
    }

    void insert_after(size_t index, const T& value) {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        if (index == m_size - 1) {
            push_back(value);
            return;
        }
        Node* current = m_head;
        for (size_t i = 0; i < index; ++i) {
            current = current->pNext;
        }
        Node* newNode = new Node(value, current->pNext);
        current->pNext = newNode;

        m_size++;
    }

    void insert_before(size_t index, const T& value) {
        if (index > m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        if (index == 0) {
            push_front(value);
        }
        if (index == m_size) {
            push_back(value);
            return;
        }
        Node* previous = m_head;
        for (size_t i = 0; i < index - 1; ++i) {
            previous = previous->pNext;
        }

        Node* newNode = new Node(value, previous->pNext);
        previous->pNext = newNode;

        m_size++;
    }
};
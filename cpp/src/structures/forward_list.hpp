#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
class ForwardList {
   private:
    struct Node {
        T data;
        Node* pNext;

        Node(const T& value, Node* nextNode = nullptr)
            : data(value), pNext(nextNode) {}
    };

    Node* m_head;
    Node* m_tail;
    size_t m_size;

    inline static const std::string ERROR_OUT_OF_BOUNDS = "index out of bounds";
    inline static const std::string ERROR_NOT_FOUND = "Value not found in list";
    Node* getNode(size_t index) const {
        Node* current = m_head;
        for (size_t i = 0; i < index; ++i) {
            current = current->pNext;
        }
        return current;
    }
    Node* getNode(const T& value) const {
        Node* current = m_head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->pNext;
        }
        return nullptr;
    }
   public:
    ForwardList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}
    ~ForwardList() { clear(); }
    size_t size() const { return m_size; }
    bool empty() const { return m_head == nullptr; }
    void clear() {
        while (m_head != nullptr) {
            Node* temp = m_head;
            m_head = m_head->pNext;
            delete temp;
        }
        m_tail = nullptr;
        m_size = 0;
    }
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
    T& operator[](size_t index) {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        return getNode(index)->data;
    }
    const T& operator[](size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        return getNode(index)->data;
    }
    void push_front(const T& value) {
        Node* newNode = new Node(value, m_head);
        m_head = newNode;
        if (m_tail == nullptr) {
            m_tail = m_head;
        }
        m_size++;
    }
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
    void insert_after(const T& target_value, const T& value) {
        Node* current = getNode(target_value);
        if (current == nullptr) {
            throw std::runtime_error(ERROR_NOT_FOUND);
        }
        if (current == m_tail) {
            push_back(value);
            return;
        }
        Node* newNode = new Node(value, current->pNext);
        current->pNext = newNode;
        m_size++;
    }
    void insert_before(const T& target_value, const T& value) {
        if (empty()) {
            throw std::runtime_error(ERROR_NOT_FOUND);
        }
        if (m_head->data == target_value) {
            push_front(value);
            return;
        }
        Node* prev = m_head;
        while (prev->pNext != nullptr && prev->pNext->data != target_value) {
            prev = prev->pNext;
        }
        if (prev->pNext == nullptr) {
            throw std::runtime_error(ERROR_NOT_FOUND);
        }
        Node* newNode = new Node(value, prev->pNext);
        prev->pNext = newNode;
        m_size++;
    }
    void pop_front() {
        if (empty()) return;
        Node* temp = m_head;
        m_head = m_head->pNext;
        delete temp;
        m_size--;
        if (m_head == nullptr) {
            m_tail = nullptr;
        }
    }
    void pop_back() {
        if (empty()) return;
        if (m_head == m_tail) {
            delete m_head;
            m_head = m_tail = nullptr;
            m_size = 0;
            return;
        }
        Node* previous = getNode(m_size - 2);
        delete m_tail;
        m_tail = previous;
        m_tail->pNext = nullptr;
        m_size--;
    }
    void remove_after(const T& target_value) {
        Node* current = getNode(target_value);
        if (current == nullptr) {
            throw std::runtime_error(ERROR_NOT_FOUND);
        }
        if (current == m_tail || current->pNext == nullptr) {
            throw std::out_of_range("No element exists after target value");
        }
        Node* toDelete = current->pNext;
        current->pNext = toDelete->pNext;
        if (toDelete == m_tail) {
            m_tail = current;
        }
        delete toDelete;
        m_size--;
    }
    void remove_before(const T& target_value) {
        if (empty() || m_head->data == target_value) {
            throw std::out_of_range("No element exists before target value");
        }
        if (m_head->pNext != nullptr && m_head->pNext->data == target_value) {
            pop_front();
            return;
        }
        Node* current = m_head;
        while (current->pNext != nullptr && current->pNext->pNext != nullptr &&
               current->pNext->pNext->data != target_value) {
            current = current->pNext;
        }
        if (current->pNext == nullptr || current->pNext->pNext == nullptr) {
            throw std::runtime_error(ERROR_NOT_FOUND);
        }
        Node* toDelete = current->pNext;
        current->pNext = toDelete->pNext;
        delete toDelete;
        m_size--;
    }
    size_t find(const T& value) const {
        Node* current = m_head;
        size_t index = 0;
        while (current != nullptr) {
            if (current->data == value) {
                return index;
            }
            current = current->pNext;
            index++;
        }
        return static_cast<size_t>(-1);
    }
    bool remove_by_value(const T& value) {
        if (empty()) return false;
        if (m_head->data == value) {
            pop_front();
            return true;
        }
        Node* current = m_head;
        while (current->pNext != nullptr && current->pNext->data != value) {
            current = current->pNext;
        }
        if (current->pNext == nullptr) {
            return false;
        }
        Node* toDelete = current->pNext;
        current->pNext = toDelete->pNext;
        if (toDelete == m_tail) {
            m_tail = current;
        }
        delete toDelete;
        m_size--;
        return true;
    }
    std::vector<T> to_vector() const {
        std::vector<T> res;
        Node* current = m_head;
        while (current != nullptr) {
            res.push_back(current->data);
            current = current->pNext;
        }
        return res;
    }
};
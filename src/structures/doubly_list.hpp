#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
class DoublyList {
   private:
    struct Node {
        T data;
        Node* pNext;
        Node* pPrev;

        Node(const T& value, Node* nextNode = nullptr, Node* prevNode = nullptr)
            : data(value), pNext(nextNode), pPrev(prevNode) {}
    };

    Node* m_head;
    Node* m_tail;
    size_t m_size;

    inline static const std::string ERROR_OUT_OF_BOUNDS = "Index out of bounds";
    inline static const std::string ERROR_NOT_FOUND = "Value not found in list";
    Node* getNode(size_t index) const {
        Node* current;
        if (index < m_size / 2) {
            current = m_head;
            for (size_t i = 0; i < index; ++i) {
                current = current->pNext;
            }
        } else {
            current = m_tail;
            for (size_t i = m_size - 1; i > index; --i) {
                current = current->pPrev;
            }
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
    DoublyList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}
    ~DoublyList() { clear(); }
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
            if (current->pNext != nullptr) std::cout << " <=> ";
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
        Node* newNode = new Node(value, m_head, nullptr);
        if (empty()) {
            m_head = m_tail = newNode;
        } else {
            m_head->pPrev = newNode;
            m_head = newNode;
        }
        m_size++;
    }
    void push_back(const T& value) {
        Node* newNode = new Node(value, nullptr, m_tail);
        if (empty()) {
            m_head = m_tail = newNode;
        } else {
            m_tail->pNext = newNode;
            m_tail = newNode;
        }
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
        Node* newNode = new Node(value, current->pNext, current);
        current->pNext->pPrev = newNode;
        current->pNext = newNode;
        m_size++;
    }
    void insert_before(const T& target_value, const T& value) {
        Node* current = getNode(target_value);
        if (current == nullptr) {
            throw std::runtime_error(ERROR_NOT_FOUND);
        }
        if (current == m_head) {
            push_front(value);
            return;
        }
        Node* newNode = new Node(value, current, current->pPrev);
        current->pPrev->pNext = newNode;
        current->pPrev = newNode;
        m_size++;
    }
    void pop_front() {
        if (empty()) return;
        Node* temp = m_head;
        m_head = m_head->pNext;
        if (m_head == nullptr) {
            m_tail = nullptr;
        } else {
            m_head->pPrev = nullptr;
        }
        delete temp;
        m_size--;
    }
    void pop_back() {
        if (empty()) return;
        if (m_head == m_tail) {
            delete m_head;
            m_head = m_tail = nullptr;
            m_size = 0;
            return;
        }
        Node* temp = m_tail;
        m_tail = m_tail->pPrev;
        m_tail->pNext = nullptr;
        delete temp;
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
        } else {
            toDelete->pNext->pPrev = current;
        }
        delete toDelete;
        m_size--;
    }
    void remove_before(const T& target_value) {
        Node* current = getNode(target_value);
        if (current == nullptr) {
            throw std::runtime_error(ERROR_NOT_FOUND);
        }
        if (current == m_head || current->pPrev == nullptr) {
            throw std::out_of_range("No element exists before target value");
        }
        Node* toDelete = current->pPrev;
        if (toDelete == m_head) {
            pop_front();
            return;
        }
        toDelete->pPrev->pNext = current;
        current->pPrev = toDelete->pPrev;
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
        Node* toDelete = getNode(value);
        if (toDelete == nullptr) {
            return false;
        }
        if (toDelete == m_head) {
            pop_front();
            return true;
        }
        if (toDelete == m_tail) {
            pop_back();
            return true;
        }
        toDelete->pPrev->pNext = toDelete->pNext;
        toDelete->pNext->pPrev = toDelete->pPrev;
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
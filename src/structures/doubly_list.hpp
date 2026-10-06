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

    // константа для обработки ошибок
    inline static const std::string ERROR_OUT_OF_BOUNDS =
        "индекс выходит за пределы двусвязного списка";

    // вспомогательный метод для поиска узла
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
    };

   public:
    DoublyList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    ~DoublyList() { clear(); }

    size_t size() const { return m_size; }
    bool empty() const { return m_head == nullptr; }

    // функция отчистки памяти
    void clear() {
        while (m_head != nullptr) {
            Node* temp = m_head;
            m_head = m_head->pNext;
            delete temp;
        }
        m_tail = nullptr;
        m_size = 0;
    }

    // вывод списка на экран
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

    // перегрузка оператора `[]`
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

    // добавление элемента в голову
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

    // добавление элемента в хвост
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

    // вставка элемента после заданного индекса
    void insert_after(size_t index, const T& value) {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        if (index == m_size - 1) {
            push_back(value);
            return;
        }

        Node* current = getNode(index);
        Node* newNode = new Node(value, current->pNext, current);
        current->pNext->pPrev = newNode;
        current->pNext = newNode;

        m_size++;
    }

    // вставка элемента перед заданным индексом
    void insert_before(size_t index, const T& value) {
        if (index > m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        if (index == 0) {
            push_front(value);
            return;
        }
        if (index == m_size) {
            push_back(value);
            return;
        }

        Node* current = getNode(index);

        Node* newNode = new Node(value, current, current->pPrev);
        current->pPrev->pNext = newNode;
        current->pPrev = newNode;

        m_size++;
    }

    // удаление элемента из головы
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

    // удаление элемента из хвоста
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

    // удаление после заданного индекса
    void remove_after(size_t index) {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        if (index == m_size - 1) {
            throw std::out_of_range("после данного индекса нет элементов");
        }

        Node* current = getNode(index);
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

    // удаление после заданного индекса
    void remove_before(size_t index) {
        if (index >= m_size) {
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        if (index == m_size - 1) {
            throw std::out_of_range("перед данным индексом нет элементов");
        }

        if (index == 1) {
            pop_front();
            return;
        }

        Node* current = getNode(index);
        Node* toDelete = current->pPrev;
        toDelete->pPrev->pNext = current;
        current->pPrev = toDelete->pPrev;

        delete toDelete;
        m_size--;
    }

    // Поиск элемента по значению.
    // Возвращает индекс первого совпадения, либо `-1`
    // если элемент не найден
    size_t find(const T& value) const {
        Node* current = m_head;
        size_t index = 0;

        while(current != nullptr) {
            if (current->data == value) {
                return index;
            }
            current = current->pNext;
            index++;
        }

        return static_cast<size_t>(-1);
    }

    // удаление элемента по значению
    // возвращает успешность операции true либо false
    bool remove_by_value(const T& value) {
        size_t index = find(value);

        if (index == static_cast<size_t>(-1)) {
            return false;
        }
        if (index == 0) {
            pop_front();
            return true;
        }
        if (index == m_size - 1) {
            pop_back();
            return true;
        }

        Node* toDelete = getNode(index);
        toDelete->pPrev->pNext = toDelete->pNext;
        toDelete->pNext->pPrev = toDelete->pPrev;

        delete toDelete;
        m_size--;
        return true;
    }
};
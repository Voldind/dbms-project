#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class DynamicArray {
   private:
    T* data;            // указатель на массив
    size_t m_size;      // текущее количество элементов
    size_t m_capacity;  // ёмкость буфера

    // константа для вывода ошибок
    inline static const std::string ERROR_OUT_OF_BOUNDS = "индекс выходит за пределы массива";
    
    // вспомогательный метод для реалокации
    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        m_capacity = new_capacity;
    }

   public:
    // конструктор
    DynamicArray() : data(nullptr), m_size(0), m_capacity(0) {}

    // деструктор
    ~DynamicArray() { delete[] data; }

    // добавление элемента в конец массива
    void push_back(const T& value) {
        if (m_size == m_capacity) {
            size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
            resize(new_capacity);
        }
        data[m_size] = value;
        m_size++;
    }

    // добавление элемента по индексу
    void insert(size_t index, const T& value) {
        if (index > m_size)
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        if (m_size == m_capacity) {
            size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
            resize(new_capacity);
        }
        for (size_t i = m_size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        m_size++;
    }

    // получение элемента по индексу
    T& operator[](size_t index) {
        if (index >= m_size)
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        return data[index];
    }

    // чтение элемента по индексу
    const T& operator[](size_t index) const {
        if (index >= m_size) {

            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        }
        return data[index];
    }

    // замена элемента по индексу
    void set(size_t index, const T& value) {
        if (index >= m_size)
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        data[index] = value;
    }

    // удаление элемента по индексу
    void remove(size_t index) {
        if (index >= m_size)
            throw std::out_of_range(ERROR_OUT_OF_BOUNDS);
        for (size_t i = index; i < m_size - 1; ++i) {
            data[i] = data[i + 1];
        }
        m_size--;
    }

    // получение длины массива
    size_t size() const { return m_size; }

    // чтение масссива
    void print() const {
        if (m_size == 0) {
            std::cout << "[]" << std::endl;
            return;
        }
        std::cout << "[";
        for (size_t i = 0; i < m_size; ++i) {
            std::cout << data[i];
            if (i < m_size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
};
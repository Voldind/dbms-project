#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    T* data; // указатель на массив
    size_t m_size; // текущее количество элементов
    size-t m_capacity; // ёмкость буфера

    // вспомогательный метод для изменения размера памяти
    void resize(size_t new_capacity){
        // Логика: выделяем новую память, копируем старые данные
        // удаляем старую память, меняем указатель data
    }

public:
    // конструктор
    DynamicArray() : data(nullptr), m_size(0), m_capacity(0) {}

    // деструктор
    ~DynamicArray() {
        delete[] data;
    }

    // добавление элемента в конец массива
    void push_back(const T& value) {
        // Если m_size == m_capacity, вызываем resize(m_capacity * 2)
    }
    
    // добавление элемента по индексу
    void insert(size_t index, const T& value) {
        // проверяем валидность индекса
        // сдвигаем элементы вправо, освобождая место
    }

    // получение элемента по индексу
    T& get(size_t index) {
        if (index >= m_size) throw std::out_of_range("индекс выходит за пределы массива");
        return data[index];
    }

    // замена элемента по индексу
    void set(size_t index, const T& value) {
        if (index >= m_size) throw std::out_of_range("индекс выходит за пределы массива");
        data[index] = value;
    }

    // удаление элемента по индексу
    void remove(size_t index) {
        // проверяем индекс. Сдвигаем элементы влево, затирая удаляемый.
        // уменьшаем m_size.
    }

    // получение длины массива
    size_t size() const {
        return m_size
    }
};
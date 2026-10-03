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
    DynamicArray()
}
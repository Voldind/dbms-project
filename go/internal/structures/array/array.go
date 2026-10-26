package array

import (
	"fmt"
	"errors"
)

var ErrOutOfBounds = errors.New("index out of boudns")

type DynamicArray[T any] struct {
	data     []T
	size     int
	capacity int
}

func NewDynamicArray[T any]() *DynamicArray[T] {
	return &DynamicArray[T]{}
}

func (da *DynamicArray[T]) IsEmpty() bool {
	return len(da.data) == 0
}

func (da *DynamicArray[T]) Size() int {
	return da.size
}

func (da *DynamicArray[T]) Capacity() int {
	return da.capacity
}

func (da *DynamicArray[T]) Clear() {
	da.data = make([]T, da.capacity)
	da.size = 0
}

func (da *DynamicArray[T]) resize(newCapacity int) {
	newData := make([]T, newCapacity)
	for i := 0; i < da.size; i++ {
		newData[i] = da.data[i]
	}
	da.data = newData
	da.capacity = newCapacity
}

func (da *DynamicArray[T]) PushBack(value T) {
	
	if da.size == da.capacity {
		newCap := da.capacity * 2
		if newCap == 0 {
			newCap = 2
		}
		da.resize(newCap)
	}
	da.data[da.size] = value
	da.size++
}

func (da *DynamicArray[T]) InsertAt(index int, value T) error {
	if index < 0 || index >= da.size {
		return fmt.Errorf("array: failed to insert element at %d (size %d): %w", index, da.size, ErrOutOfBounds)
	}
	if da.size == da.capacity {
		da.resize(da.capacity * 2)
	}
	for i := da.size; i > index; i-- {
		da.data[i] = da.data[i-1]
	}
	da.data[index] = value
	return nil
}

func (da *DynamicArray[T]) Get(index int) (T, error) {
	var result T
	if index < 0 || index >= da.size {
		return result, fmt.Errorf("array: failed to get element at %d (size %d): %w", index, da.size, ErrOutOfBounds)
	}
	return da.data[index], nil
}

func (da *DynamicArray[T]) Set(index int, value T) error {
	if index < 0 || index >= da.size {
		return fmt.Errorf("array: failed to set element at %d (size %d): %w", index, da.size, ErrOutOfBounds)
	}
	da.data[index] = value
	return nil
}

func (da *DynamicArray[T]) Remove(index int) error {
	if index < 0 || index >= da.size {
		return fmt.Errorf("array: failed to remove element at %d (size %d): %w", index, da.size, ErrOutOfBounds)
	}
	for i := index; i < da.size-1; i++ {
		da.data[i] = da.data[i+1]
	}
	da.size--
	return nil
}

func (da *DynamicArray[T]) Print() {
	fmt.Print("[")
	for i := 0; i < da.size; i++ {
		fmt.Print(da.data[i])
		if i < da.size-1 {
			fmt.Print(", ")
		}
	}
	fmt.Print("]")
}

func (da *DynamicArray[T]) ToSlice() []T {
	result := make([]T, da.size)
	copy(result, da.data[:da.size])
	return result
}

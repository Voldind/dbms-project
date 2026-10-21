package structures

import (
	"errors"
	"fmt"
)

var (
	ErrOutOfBounds = errors.New("index out of boudns")
	ErrNotFound    = errors.New("value not found")
)

type DynamicArray[T any] struct {
	data     []T
	size     int
	capacity int
}

func NewDynamicArray[T any](initialCapacity int) *DynamicArray[T] {
	if initialCapacity <= 0 {
		initialCapacity = 2
	}
	return &DynamicArray[T]{
		data:     make([]T, initialCapacity),
		size:     0,
		capacity: initialCapacity,
	}
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
		da.resize(da.capacity * 2)
	}
	da.data[da.size] = value
	da.size++
}
func (da *DynamicArray[T]) InsertAt(index int, value T) error {
	if index < 0 || index >= da.size {
		return ErrOutOfBounds
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
		return result, ErrOutOfBounds
	}
	return da.data[index], nil
}
func (da *DynamicArray[T]) Set(index int, value T) error {
	if index < 0 || index >= da.size {
		return ErrOutOfBounds
	}
	da.data[index] = value
	return nil
}
func (da *DynamicArray[T]) Remove(index int) error {
	if index < 0 || index >= da.size {
		return ErrOutOfBounds
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
		if i < da.size - 1 {
			fmt.Print(", ")
		}
	}
	fmt.Print("]")
}

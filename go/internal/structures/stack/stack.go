package stack

import (
	"errors"
	"fmt"
)

var ErrEmpty = errors.New("stack is empty")

type Stack[T any] struct {
	data []T
}

func NewStack[T any]() *Stack[T] {
	return &Stack[T]{
		data: make([]T, 0),
	}
}

func (s *Stack[T]) IsEmpty() bool {
	return len(s.data) == 0
}

func (s *Stack[T]) Size() int {
	return len(s.data)
}

// Push добавляет элемент на вершину стека
func (s *Stack[T]) Push(value T) {
	s.data = append(s.data, value)
}

// Pop удаляет верхний элемент стека
func (s *Stack[T]) Pop() (T, error) {
	var deleted T
	if s.IsEmpty() {
		return deleted, fmt.Errorf("stack: failed to pop: %w", ErrEmpty)
	}
	deleted = s.data[len(s.data)-1]
	s.data = s.data[:len(s.data)-1]
	return deleted, nil
}

// Top возвращает верхний элемент без его удаления
func (s *Stack[T]) Top() (T, error) {
	var zero T
	if s.IsEmpty() {
		return zero, fmt.Errorf("stack: failed to get top: %w", ErrEmpty)
	}
	return s.data[len(s.data)-1], nil
}
package queue

import (
	"errors"
	"fmt"
)

var ErrEmpty = errors.New("queue is empty")

type Queue[T any] struct {
	data []T
}

func NewQueue[T any]() *Queue[T] {
	return &Queue[T]{
		data: make([]T, 0),
	}
}

func (q *Queue[T]) IsEmpty() bool {
	return len(q.data) == 0
}

func (q *Queue[T]) Size() int {
	return len(q.data)
}

// Push добавляет элемент в конец очереди
func (q *Queue[T]) Push(value T) {
	q.data = append(q.data, value)
}

// Pop удаляет первый элемент из начала очереди
func (q *Queue[T]) Pop() (T, error) {
	var deleted T
	if q.IsEmpty() {
		return deleted, fmt.Errorf("queue: failed to pop: %w", ErrEmpty)
	}
	deleted = q.data[0]
	q.data = q.data[1:]
	return deleted, nil
}

// Front возвращает первый элемент в очереди без его удаления
func (q *Queue[T]) Front() (T, error) {
	var zero T
	if q.IsEmpty() {
		return zero, fmt.Errorf("queue: failed to get front: %w", ErrEmpty)
	}
	return q.data[0], nil
}
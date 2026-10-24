package doublylist

import (
	"errors"
	"fmt"
)

var (
	ErrNotFound    = errors.New("value not found")
	ErrEmpty       = errors.New("list is empty")
	ErrNotExist    = errors.New("value not exist")
	ErrOutOfBounds = errors.New("index out of boudns")
)

type Node[T any] struct {
	Value T
	Next  *Node[T]
	Prev  *Node[T]
}

type DoublyList[T comparable] struct {
	head *Node[T]
	tail *Node[T]
	size int
}

func NewDoublyList[T comparable]() *DoublyList[T] {
	return &DoublyList[T]{}
}

func (dl *DoublyList[T]) IsEmpty() bool {
	return dl.head == nil
}

func (dl *DoublyList[T]) Size() int {
	return dl.size
}

func (dl *DoublyList[T]) Clear() {
	dl.head = nil
	dl.tail = nil
	dl.size = 0
}

func (dl *DoublyList[T]) PushFront(value T) {
	newNode := &Node[T]{Value: value}
	if dl.IsEmpty() {
		dl.head = newNode
		dl.tail = newNode
	} else {
		newNode.Next = dl.head
		dl.head.Prev = newNode
		dl.head = newNode
	}
	dl.size++
}

func (dl *DoublyList[T]) PushBack(value T) {
	newNode := &Node[T]{Value: value}
	if dl.IsEmpty() {
		dl.head = newNode
		dl.tail = newNode
	} else {
		dl.tail.Next = newNode
		newNode.Prev = dl.tail
		dl.tail = newNode
	}
	dl.size++
}

func (dl *DoublyList[T]) PopFront() error {
	if dl.IsEmpty() {
		return fmt.Errorf("dlist: failed to pop front: %w", ErrEmpty)
	}
	if dl.head == dl.tail {
		dl.head = nil
		dl.tail = nil
	} else {
		dl.head = dl.head.Next
		dl.head.Prev = nil
	}
	dl.size--
	return nil
}

func (dl *DoublyList[T]) PopBack() error {
	if dl.IsEmpty() {
		return fmt.Errorf("dlist: failed to pop back: %w", ErrEmpty)
	}
	if dl.head == dl.tail {
		dl.head = nil
		dl.tail = nil
	} else {
		dl.tail = dl.tail.Prev
		dl.tail.Next = nil
	}
	dl.size--
	return nil
}

func (dl *DoublyList[T]) getNode(target T) (*Node[T], error) {
	if dl.IsEmpty() {
		return nil, ErrEmpty
	}
	current := dl.head
	for current != nil {
		if current.Value == target {
			return current, nil
		}
		current = current.Next
	}
	return nil, ErrNotFound
}

func (dl *DoublyList[T]) InsertAfter(target, value T) error {
	targetNode, err := dl.getNode(target)
	if err != nil {
		return fmt.Errorf("dlist: failed to insert %v after %v: %w", value, target, err)
	}
	if targetNode == dl.tail {
		dl.PushBack(value)
		return nil
	}
	newNode := &Node[T]{Value: value, Next: targetNode.Next, Prev: targetNode}
	targetNode.Next.Prev = newNode
	targetNode.Next = newNode
	dl.size++
	return nil
}

func (dl *DoublyList[T]) InsertBefore(target, value T) error {
	targetNode, err := dl.getNode(target)
	if err != nil {
		return fmt.Errorf("dlist: failed to insert %v before %v: %w", value, target, err)
	}
	if targetNode == dl.head {
		dl.PushFront(value)
		return nil
	}
	newNode := &Node[T]{Value: value, Next: targetNode, Prev: targetNode.Prev}
	targetNode.Prev.Next = newNode
	targetNode.Prev = newNode
	dl.size++
	return nil
}

func (dl *DoublyList[T]) RemoveAfter(target T) error {
	targetNode, err := dl.getNode(target)
	if err != nil {
		return fmt.Errorf("dlist: failed to remove after %v: %w", target, err)
	}
	if targetNode == dl.tail {
		return fmt.Errorf("dlist: failed to remove after %v: %w", target, ErrNotExist)
	}
	if targetNode.Next == dl.tail {
		return dl.PopBack()
	}
	toDelete := targetNode.Next
	targetNode.Next = toDelete.Next
	toDelete.Next.Prev = targetNode
	dl.size--
	return nil
}

func (dl *DoublyList[T]) RemoveBefore(target T) error {
	targetNode, err := dl.getNode(target)
	if err != nil {
		return fmt.Errorf("dlist: failed to remove before %v: %w", target, err)
	}
	if targetNode == dl.head {
		return fmt.Errorf("dlist: failed to remove before %v: %w", target, ErrNotExist)
	}
	if targetNode.Prev == dl.head {
		return dl.PopFront()
	}
	toDelete := targetNode.Prev
	targetNode.Prev = toDelete.Prev
	toDelete.Prev.Next = targetNode
	dl.size--
	return nil
}

func (dl *DoublyList[T]) Remove(target T) error {
	targetNode, err := dl.getNode(target)
	if err != nil {
		return fmt.Errorf("dlist: failed to remove %v: %w", target, err)
	}
	if targetNode == dl.head {
		return dl.PopFront()
	}
	if targetNode == dl.tail {
		return dl.PopBack()
	}
	targetNode.Prev.Next = targetNode.Next
	targetNode.Next.Prev = targetNode.Prev
	dl.size--
	return nil
}

func (dl *DoublyList[T]) Find(target T) (int, bool) {
	current := dl.head
	index := 0
	for current != nil {
		if current.Value == target {
			return index, true
		}
		current = current.Next
		index++
	}
	return -1, false
}

func (dl *DoublyList[T]) Get(index int) (T, error) {
	var result T
	if index < 0 || index >= dl.size {
		return result, fmt.Errorf("dlist: failed to get element by index %v: %w", index, ErrOutOfBounds)
	}
	if index < dl.size/2 {
		current := dl.head
		for i := 0; i < index; i++ {
			current = current.Next
		}
		return current.Value, nil
	}
	current := dl.tail
	for i := dl.size - 1; i > index; i-- {
		current = current.Prev
	}
	return current.Value, nil
}

// Возвращает первый элемент и ошибку если список пустой
func (dl *DoublyList[T]) Front() (T, error) {
	var result T
	if dl.IsEmpty() {
		return result, fmt.Errorf("dlist: failed to get element from front: %w", ErrEmpty)
	}
	return dl.head.Value, nil
}

// Возвращает последний элемент и ошибку если список пустой
func (dl *DoublyList[T]) Back() (T, error) {
	var result T
	if dl.IsEmpty() {
		return result, fmt.Errorf("dlist: failed to get element from back: %w", ErrEmpty)
	}
	return dl.tail.Value, nil
}

func (dl *DoublyList[T]) ToSlice() []T {
	slice := make([]T, dl.size)
	current := dl.head
	index := 0
	for current != nil {
		slice[index] = current.Value
		current = current.Next
		index++
	}
	return slice
}

func (dl *DoublyList[T]) Print() {
	fmt.Print("[")
	current := dl.head
	for current != nil {
		fmt.Print(current.Value)
		if current != dl.tail {
			fmt.Print(", ")
		}
		current = current.Next
	}
	fmt.Println("]")
}

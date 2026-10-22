package forwardlist

import (
	"fmt"
	"errors"
)

var (
	ErrNotFound    = errors.New("value not found")
	ErrEmpty       = errors.New("container is empty")
	ErrNotExist    = errors.New("value not exist")
)

type Node[T comparable] struct {
	Value T
	Next  *Node[T]
}

type ForwardList[T comparable] struct {
	head *Node[T]
	tail *Node[T]
	size int
}

func NewForwardList[T comparable]() *ForwardList[T] {
	return &ForwardList[T]{}
}

func (fl *ForwardList[T]) IsEmpty() bool {
	return fl.head == nil
}

func (fl *ForwardList[T]) Size() int {
	return fl.size
}

func (fl *ForwardList[T]) Clear() {
	fl.head = nil
	fl.tail = nil
	fl.size = 0
}

func (fl *ForwardList[T]) Print() {
	fmt.Print("[")
	current := fl.head
	for current != nil {
		fmt.Print(current.Value)
		if current != fl.tail {
			fmt.Print(" -> ")
		}
		current = current.Next
	}
	fmt.Println("]")
}

func (fl *ForwardList[T]) getNode(target T) (*Node[T], error) {
	if fl.IsEmpty() {
		return nil, ErrEmpty
	}
	current := fl.head
	for current != nil {
		if current.Value == target {
			return current, nil
		}
		current = current.Next
	}
	return nil, ErrNotFound
}

func (fl *ForwardList[T]) getPrevNode(target T) (*Node[T], error) {
	if fl.IsEmpty() {
		return nil, ErrEmpty
	}
	prev := fl.head
	for prev.Next != nil {
		if prev.Next.Value == target {
			return prev, nil
		}
		prev = prev.Next
	}
	return nil, ErrNotFound
}

func (fl *ForwardList[T]) PushBack(value T) {
	newNode := &Node[T]{Value: value}
	if fl.IsEmpty() {
		fl.head = newNode
		fl.tail = newNode
	} else {
		fl.tail.Next = newNode
		fl.tail = newNode
	}
	fl.size++
}

func (fl *ForwardList[T]) PushFront(value T) {
	newNode := &Node[T]{Value: value}
	if fl.IsEmpty() {
		fl.head = newNode
		fl.tail = newNode
	} else {
		newNode.Next = fl.head
		fl.head = newNode
	}
	fl.size++
}

func (fl *ForwardList[T]) InsertAfter(target T, value T) error {
	targetNode, err := fl.getNode(target)
	if err != nil {
		return fmt.Errorf("flist: failed to insert \"%v\" after \"%v\": %w", value, target, err)
	}
	if targetNode == fl.tail {
		fl.PushBack(value)
		return nil
	}
	newNode := &Node[T]{Value: value, Next: targetNode.Next}
	targetNode.Next = newNode
	fl.size++
	return nil
}

func (fl *ForwardList[T]) InsertBefore(target T, value T) error {
	if fl.head != nil && fl.head.Value == target {
		fl.PushFront(value)
		return nil
	}
	prev, err := fl.getPrevNode(target)
	if err != nil {
		return fmt.Errorf("flist: failed to insert \"%v\" before \"%v\": %w", value, target, err)
	}
	newNode := &Node[T]{Value: value, Next: prev.Next}
	prev.Next = newNode
	fl.size++
	return nil
}

func (fl *ForwardList[T]) PopFront() error {
	if fl.IsEmpty() {
		return fmt.Errorf("flist: failed to pop front: %w", ErrEmpty)
	}
	fl.head = fl.head.Next
	fl.size--
	if fl.head == nil {
		fl.tail = nil
	}
	return nil
}

func (fl *ForwardList[T]) PopBack() error {
	if fl.IsEmpty() {
		return fmt.Errorf("flist: failed to pop back: %w", ErrEmpty)
	}
	if fl.head == fl.tail {
		return fl.PopFront()
	}
	prev := fl.head
	for prev.Next != fl.tail {
		prev = prev.Next
	}
	prev.Next = nil
	fl.tail = prev
	fl.size--
	return nil
}

func (fl *ForwardList[T]) RemoveAfter(target T) error {
	targetNode, err := fl.getNode(target)
	if err != nil {
		return fmt.Errorf("flist: failed to remove after %v: %w", target, err)
	}
	if targetNode == fl.tail || targetNode.Next == nil {
		return fmt.Errorf("flist: failed to remove after %v: %w", target, ErrNotExist)
	}
	toDelete := targetNode.Next
	targetNode.Next = toDelete.Next
	if toDelete == fl.tail {
		fl.tail = targetNode
	}
	fl.size--
	return nil
}

func (fl *ForwardList[T]) RemoveBefore(target T) error {
	if fl.IsEmpty() {
		return fmt.Errorf("flist: failed to remove before %v: %w", target, ErrEmpty)
	}
	if fl.head == fl.tail || fl.head.Value == target {
		return fmt.Errorf("flist: failed to remove before %v: %w", target, ErrNotExist)
	}
	if fl.head.Next.Value == target {
		return fl.PopFront()
	}
	prevPrev := fl.head
	for prevPrev.Next.Next != nil {
		if prevPrev.Next.Next.Value == target {
			prevPrev.Next = prevPrev.Next.Next
			fl.size--
			return nil
		}
		prevPrev = prevPrev.Next
	}
	return fmt.Errorf("flist: failed to remove before %v: %w", target, ErrNotFound)
}

// Find ищет первое вхождение target в списке.
// Возвращает (индекс, true), если элемент найден, или (-1, false), если нет.
func (fl *ForwardList[T]) Find(target T) (int, bool) {
	current := fl.head
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

// Remove удаляет первый узел со значением target из списка.
func (fl *ForwardList[T]) Remove(target T) error {
	if fl.IsEmpty() {
		return fmt.Errorf("flist: failed to remove element \"%v\": %w", target, ErrEmpty)
	}
	if fl.head.Value == target {
		return fl.PopFront()
	}
	prev, err := fl.getPrevNode(target)
	if err != nil {
		return fmt.Errorf("flist: failed to remove element \"%v\": %w", target, err)
	}
	toDelete := prev.Next
	prev.Next = toDelete.Next
	if toDelete == fl.tail {
		fl.tail = prev
	}
	fl.size--
	return nil
}
package stack

import (
	"errors"
	"testing"
)

func TestStack(t *testing.T) {
	s := NewStack[string]()

	if !s.IsEmpty() {
		t.Errorf("ожидался пустой стек")
	}

	s.Push("first")
	s.Push("second")

	top, err := s.Top()
	if err != nil || top != "second" {
		t.Errorf("ожидалось 'second' на вершине, получено: %v", top)
	}

	val, err := s.Pop()
	if err != nil || val != "second" {
		t.Fatalf("ошибка Pop: %v", err)
	}

	top, _ = s.Top()
	if top != "first" {
		t.Errorf("ожидалось 'first' на вершине, получено: %v", top)
	}

	s.Pop()
	if !s.IsEmpty() {
		t.Errorf("стек должен быть пуст")
	}

	if _, err := s.Pop(); !errors.Is(err, ErrEmpty) {
		t.Errorf("ожидалась ошибка ErrEmpty")
	}
}

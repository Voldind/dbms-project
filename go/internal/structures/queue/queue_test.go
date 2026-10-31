package queue

import (
	"errors"
	"testing"
)

func TestQueue(t *testing.T) {
	q := NewQueue[int]()

	q.Push(100)
	q.Push(200)

	front, err := q.Front()
	if err != nil || front != 100 {
		t.Errorf("ожидалось 100 в голове очереди, получено: %v", front)
	}

	val, err := q.Pop()
	if err != nil || val != 100 {
		t.Fatalf("ошибка Pop: %v", err)
	}

	front, _ = q.Front()
	if front != 200 {
		t.Errorf("ожидалось 200 в голове очереди, получено: %v", front)
	}

	q.Pop()
	if !q.IsEmpty() {
		t.Errorf("очередь должна быть пуста")
	}

	if _, err := q.Pop(); !errors.Is(err, ErrEmpty) {
		t.Errorf("ожидалась ошибка ErrEmpty")
	}
}

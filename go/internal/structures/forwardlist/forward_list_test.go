package forwardlist

import (
	"errors"
	"testing"
)

func TestForwardList_PushAndFind(t *testing.T) {
	fl := &ForwardList[string]{}

	if !fl.IsEmpty() {
		t.Errorf("ожидался пустой список")
	}

	fl.PushBack("B")
	fl.PushFront("A")
	fl.PushBack("C") // Список: A -> B -> C

	if fl.Size() != 3 {
		t.Errorf("ожидался размер 3, получено %d", fl.Size())
	}

	// Проверка поиска
	idx, ok := fl.Find("B")
	if !ok || idx != 1 {
		t.Errorf("ожидался элемент B на индексе 1, получено idx=%d, ok=%v", idx, ok)
	}

	_, ok = fl.Find("NON_EXISTENT")
	if ok {
		t.Errorf("поиск несуществующего элемента должен возвращать false")
	}
}

func TestForwardList_InsertAfterAndBefore(t *testing.T) {
	fl := &ForwardList[string]{}
	fl.PushBack("A")
	fl.PushBack("C")

	// Вставка в середину через InsertAfter
	err := fl.InsertAfter("A", "B")
	if err != nil {
		t.Fatalf("ошибка при InsertAfter: %v", err)
	} // Список: A -> B -> C

	// Вставка перед головой через InsertBefore
	err = fl.InsertBefore("A", "START")
	if err != nil {
		t.Fatalf("ошибка при InsertBefore: %v", err)
	} // Список: START -> A -> B -> C

	idx, _ := fl.Find("START")
	if idx != 0 {
		t.Errorf("START должен быть на 0 позиции, получено %d", idx)
	}

	idx, _ = fl.Find("B")
	if idx != 2 {
		t.Errorf("B должен быть на 2 позиции, получено %d", idx)
	}

	// Проверка ошибки при вставке перед несуществующим целевым элементом
	err = fl.InsertBefore("UNKNOWN", "X")
	if err == nil {
		t.Errorf("ожидалась ошибка при вставке перед несуществующим элементом")
	}
}

func TestForwardList_Removals(t *testing.T) {
	fl := &ForwardList[string]{}
	fl.PushBack("A")
	fl.PushBack("B")
	fl.PushBack("C")
	fl.PushBack("D") // Список: A -> B -> C -> D

	// Удаление после "A" (удаляется "B")
	if err := fl.RemoveAfter("A"); err != nil {
		t.Fatalf("ошибка при RemoveAfter: %v", err)
	}
	if _, ok := fl.Find("B"); ok {
		t.Errorf("элемент B должен быть удален")
	}

	// Удаление перед "D" (удаляется "C")
	if err := fl.RemoveBefore("D"); err != nil {
		t.Fatalf("ошибка при RemoveBefore: %v", err)
	}
	if _, ok := fl.Find("C"); ok {
		t.Errorf("элемент C должен быть удален")
	}

	// Удаление по значению самого элемента "D"
	if err := fl.Remove("D"); err != nil {
		t.Fatalf("ошибка при Remove: %v", err)
	}

	if fl.Size() != 1 {
		t.Errorf("ожидался размер 1, получено %d", fl.Size())
	}
}

func TestForwardList_PopAndClear(t *testing.T) {
	fl := &ForwardList[int]{}
	fl.PushBack(10)
	fl.PushBack(20)

	if err := fl.PopFront(); err != nil {
		t.Fatalf("ошибка PopFront: %v", err)
	}
	if fl.Size() != 1 {
		t.Errorf("ожидался размер 1 после PopFront")
	}

	if err := fl.PopBack(); err != nil {
		t.Fatalf("ошибка PopBack: %v", err)
	}
	if !fl.IsEmpty() {
		t.Errorf("список должен быть пуст после PopBack")
	}

	// Вызов Pop для пустого списка должен возвращать ErrEmpty
	err := fl.PopFront()
	if !errors.Is(err, ErrEmpty) {
		t.Errorf("ожидалась ошибка ErrEmpty, получено: %v", err)
	}

	// Проверка Clear
	fl.PushBack(100)
	fl.Clear()
	if !fl.IsEmpty() || fl.Size() != 0 {
		t.Errorf("ожидался очищенный список после Clear()")
	}
}
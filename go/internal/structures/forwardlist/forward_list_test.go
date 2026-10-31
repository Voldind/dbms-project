package forwardlist

import (
	"errors"
	"testing"
)

// 1. Тест базовых операций: PushFront, PushBack, PopFront, PopBack, Front, Back
func TestForwardList_PushPopFrontBack(t *testing.T) {
	fl := &ForwardList[string]{}

	if !fl.IsEmpty() {
		t.Errorf("Ожидался пустой список")
	}

	fl.PushBack("B")  // [B]
	fl.PushFront("A") // [A, B]
	fl.PushBack("C")  // [A, B, C]

	if fl.Size() != 3 {
		t.Errorf("Ожидался размер 3, получено %d", fl.Size())
	}

	// Проверка чтения с головы и хвоста (Front и Back)
	front, err := fl.Front()
	if err != nil || front != "A" {
		t.Errorf("Front(): ожидалось 'A', получено '%s', err: %v", front, err)
	}

	back, err := fl.Back()
	if err != nil || back != "C" {
		t.Errorf("Back(): ожидалось 'C', получено '%s', err: %v", back, err)
	}

	// Удаление с головы
	if err := fl.PopFront(); err != nil {
		t.Fatalf("Ошибка PopFront: %v", err)
	} // Осталось: [B, C]

	front, _ = fl.Front()
	if front != "B" {
		t.Errorf("Ожидалась новая голова 'B', получено '%s'", front)
	}

	// Удаление с хвоста
	if err := fl.PopBack(); err != nil {
		t.Fatalf("Ошибка PopBack: %v", err)
	} // Осталось: [B]

	back, _ = fl.Back()
	if back != "B" {
		t.Errorf("Ожидался новый хвост 'B', получено '%s'", back)
	}

	// Удаление последнего оставшегося элемента
	if err := fl.PopBack(); err != nil {
		t.Fatalf("Ошибка при удалении единственного элемента: %v", err)
	}

	if !fl.IsEmpty() || fl.Size() != 0 {
		t.Errorf("Список должен быть пуст")
	}

	// Проверка ошибок на пустом списке
	if err := fl.PopFront(); !errors.Is(err, ErrEmpty) {
		t.Errorf("PopFront на пустом списке должен возвращать ErrEmpty, получено: %v", err)
	}
	if _, err := fl.Front(); !errors.Is(err, ErrEmpty) {
		t.Errorf("Front на пустом списке должен возвращать ErrEmpty, получено: %v", err)
	}
	if _, err := fl.Back(); !errors.Is(err, ErrEmpty) {
		t.Errorf("Back на пустом списке должен возвращать ErrEmpty, получено: %v", err)
	}
}

// 2. Тест методов чтения и поиска: Get, Find, ToSlice
func TestForwardList_GetFindToSlice(t *testing.T) {
	fl := &ForwardList[int]{}
	fl.PushBack(10)
	fl.PushBack(20)
	fl.PushBack(30) // [10, 20, 30]

	// Проверка Get по индексу
	val, err := fl.Get(1)
	if err != nil || val != 20 {
		t.Errorf("Get(1): ожидалось 20, получено %d, err: %v", val, err)
	}

	// Проверка выхода за границы в Get
	if _, err := fl.Get(-1); !errors.Is(err, ErrOutOfBounds) {
		t.Errorf("Get(-1): ожидалась ошибка ErrOutOfBounds, получено: %v", err)
	}
	if _, err := fl.Get(3); !errors.Is(err, ErrOutOfBounds) {
		t.Errorf("Get(3): ожидалась ошибка ErrOutOfBounds, получено: %v", err)
	}

	// Проверка Find
	idx, ok := fl.Find(30)
	if !ok || idx != 2 {
		t.Errorf("Find(30): ожидался индекс 2 и true, получено idx=%d, ok=%v", idx, ok)
	}

	if _, ok := fl.Find(999); ok {
		t.Errorf("Find(999): ожидался false для несуществующего элемента")
	}

	// Проверка ToSlice
	slice := fl.ToSlice()
	expected := []int{10, 20, 30}
	if len(slice) != len(expected) {
		t.Fatalf("ToSlice(): ожидалась длина %d, получено %d", len(expected), len(slice))
	}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("ToSlice()[%d]: ожидалось %d, получено %d", i, v, slice[i])
		}
	}
}

// 3. Тест вставки до и после элемента: InsertAfter, InsertBefore
func TestForwardList_InsertAfterAndBefore(t *testing.T) {
	fl := &ForwardList[string]{}
	fl.PushBack("A")
	fl.PushBack("C") // [A, C]

	// Вставка в середину (после A)
	if err := fl.InsertAfter("A", "B"); err != nil {
		t.Fatalf("Ошибка InsertAfter: %v", err)
	} // [A, B, C]

	// Вставка перед головой (перед A)
	if err := fl.InsertBefore("A", "START"); err != nil {
		t.Fatalf("Ошибка InsertBefore для головы: %v", err)
	} // [START, A, B, C]

	// Вставка после хвоста (после C)
	if err := fl.InsertAfter("C", "END"); err != nil {
		t.Fatalf("Ошибка InsertAfter для хвоста: %v", err)
	} // [START, A, B, C, END]

	expected := []string{"START", "A", "B", "C", "END"}
	slice := fl.ToSlice()
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("На позиции %d ожидалось '%s', получено '%s'", i, v, slice[i])
		}
	}

	// Попытка вставки относительно несуществующего элемента
	if err := fl.InsertBefore("UNKNOWN", "X"); err == nil {
		t.Errorf("Ожидалась ошибка при InsertBefore для несуществующего элемента")
	}
}

// 4. Тест удалений: RemoveAfter, RemoveBefore, Remove, Clear
func TestForwardList_Removals(t *testing.T) {
	fl := &ForwardList[string]{}
	fl.PushBack("A")
	fl.PushBack("B")
	fl.PushBack("C")
	fl.PushBack("D") // [A, B, C, D]

	// Удаление после "A" (удаляется "B")
	if err := fl.RemoveAfter("A"); err != nil {
		t.Fatalf("Ошибка RemoveAfter: %v", err)
	} // [A, C, D]

	if _, ok := fl.Find("B"); ok {
		t.Errorf("Элемент 'B' должен быть удален")
	}

	// Удаление перед "D" (удаляется "C")
	if err := fl.RemoveBefore("D"); err != nil {
		t.Fatalf("Ошибка RemoveBefore: %v", err)
	} // [A, D]

	if _, ok := fl.Find("C"); ok {
		t.Errorf("Элемент 'C' должен быть удален")
	}

	// Удаление по значению ("D" — хвост)
	if err := fl.Remove("D"); err != nil {
		t.Fatalf("Ошибка Remove: %v", err)
	} // [A]

	back, _ := fl.Back()
	if back != "A" || fl.Size() != 1 {
		t.Errorf("После удаления 'D' хвостом должен стать 'A', размер 1")
	}

	// Проверка Clear
	fl.Clear()
	if !fl.IsEmpty() || fl.Size() != 0 {
		t.Errorf("После Clear() список должен быть пустым")
	}
}
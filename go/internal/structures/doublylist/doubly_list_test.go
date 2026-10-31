package doublylist

import (
	"errors"
	"testing"
)

// Тест добавления и извлечения из головы/хвоста
func TestDoublyList_PushAndPop(t *testing.T) {
	dl := NewDoublyList[string]()

	if !dl.IsEmpty() {
		t.Errorf("Ожидался пустой список")
	}

	dl.PushBack("B")  // [B]
	dl.PushFront("A") // [A, B]
	dl.PushBack("C")  // [A, B, C]

	if dl.Size() != 3 {
		t.Errorf("Ожидался размер 3, получено %d", dl.Size())
	}

	// Проверка Front и Back
	front, err := dl.Front()
	if err != nil || front != "A" {
		t.Errorf("Front: ожидалось 'A', получено '%s', ошибка: %v", front, err)
	}

	back, err := dl.Back()
	if err != nil || back != "C" {
		t.Errorf("Back: ожидалось 'C', получено '%s', ошибка: %v", back, err)
	}

	// Удаление из головы
	if err := dl.PopFront(); err != nil {
		t.Fatalf("Ошибка PopFront: %v", err)
	} // Осталось: [B, C]

	if dl.Size() != 2 {
		t.Errorf("Ожидался размер 2 после PopFront, получено %d", dl.Size())
	}

	front, _ = dl.Front()
	if front != "B" {
		t.Errorf("Ожидалась новая голова 'B', получено '%s'", front)
	}

	// Удаление из хвоста
	if err := dl.PopBack(); err != nil {
		t.Fatalf("Ошибка PopBack: %v", err)
	} // Осталось: [B]

	back, _ = dl.Back()
	if back != "B" {
		t.Errorf("Ожидался новый хвост 'B', получено '%s'", back)
	}

	// Очищаем единственный элемент
	_ = dl.PopBack()
	if !dl.IsEmpty() {
		t.Errorf("Список должен быть пуст")
	}

	// Проверка ошибки при вызове Pop для пустого списка
	if err := dl.PopFront(); !errors.Is(err, ErrEmpty) {
		t.Errorf("Ожидалась ошибка ErrEmpty при PopFront, получено: %v", err)
	}
	if err := dl.PopBack(); !errors.Is(err, ErrEmpty) {
		t.Errorf("Ожидалась ошибка ErrEmpty при PopBack, получено: %v", err)
	}
}

// Тест методов чтения Get, Find и ToSlice
func TestDoublyList_GetAndFind(t *testing.T) {
	dl := NewDoublyList[int]()
	dl.PushBack(10)
	dl.PushBack(20)
	dl.PushBack(30)
	dl.PushBack(40) // [10, 20, 30, 40]

	// Чтение из первой половины (проход с головы)
	val, err := dl.Get(1)
	if err != nil || val != 20 {
		t.Errorf("Get(1): ожидалось 20, получено %d, ошибка: %v", val, err)
	}

	// Чтение из второй половины (проход с хвоста)
	val, err = dl.Get(3)
	if err != nil || val != 40 {
		t.Errorf("Get(3): ожидалось 40, получено %d, ошибка: %v", val, err)
	}

	// Выход за границы
	_, err = dl.Get(4)
	if err == nil {
		t.Errorf("Ожидалась ошибка при обращении к некорректному индексу 4")
	}

	_, err = dl.Get(-1)
	if err == nil {
		t.Errorf("Ожидалась ошибка при обращении к отрицательному индексу")
	}

	// Поиск элементов
	idx, ok := dl.Find(30)
	if !ok || idx != 2 {
		t.Errorf("Find(30): ожидался индекс 2, получено idx=%d, ok=%v", idx, ok)
	}

	_, ok = dl.Find(999)
	if ok {
		t.Errorf("Find(999): ожидался false для отсутствующего элемента")
	}

	// Проверка ToSlice
	slice := dl.ToSlice()
	expected := []int{10, 20, 30, 40}
	if len(slice) != len(expected) {
		t.Fatalf("ToSlice: неверная длина среза")
	}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("ToSlice[%d]: ожидалось %d, получено %d", i, v, slice[i])
		}
	}
}

// Тест вставки InsertAfter / InsertBefore
func TestDoublyList_Insertions(t *testing.T) {
	dl := NewDoublyList[string]()
	dl.PushBack("A")
	dl.PushBack("C") // [A, C]

	// Вставка в середину через InsertAfter
	if err := dl.InsertAfter("A", "B"); err != nil {
		t.Fatalf("Ошибка InsertAfter: %v", err)
	} // [A, B, C]

	// Вставка перед головой через InsertBefore
	if err := dl.InsertBefore("A", "START"); err != nil {
		t.Fatalf("Ошибка InsertBefore: %v", err)
	} // [START, A, B, C]

	// Вставка после хвоста
	if err := dl.InsertAfter("C", "END"); err != nil {
		t.Fatalf("Ошибка InsertAfter для хвоста: %v", err)
	} // [START, A, B, C, END]

	slice := dl.ToSlice()
	expected := []string{"START", "A", "B", "C", "END"}
	for i, v := range expected {
		if slice[i] != v {
			t.Errorf("Инструкция вставки ошибочна, на позиции %d ожидалось %s, получено %s", i, v, slice[i])
		}
	}

	// Несуществующий элемент
	if err := dl.InsertAfter("NOT_EXISTS", "X"); err == nil {
		t.Errorf("Ожидалась ошибка при вставке относительно несуществующего узла")
	}
}

// Тест удаления Remove, RemoveAfter, RemoveBefore
func TestDoublyList_Removals(t *testing.T) {
	dl := NewDoublyList[string]()
	dl.PushBack("A")
	dl.PushBack("B")
	dl.PushBack("C")
	dl.PushBack("D") // [A, B, C, D]

	// Удаление элемента ПОСЛЕ "A" (удалит "B")
	if err := dl.RemoveAfter("A"); err != nil {
		t.Fatalf("Ошибка RemoveAfter: %v", err)
	} // [A, C, D]

	if idx, ok := dl.Find("B"); ok {
		t.Errorf("Элемент 'B' не должен присутствовать в списке (найден на индексе %d)", idx)
	}

	// Удаление элемента ПЕРЕД "D" (удалит "C")
	if err := dl.RemoveBefore("D"); err != nil {
		t.Fatalf("Ошибка RemoveBefore: %v", err)
	} // [A, D]

	if idx, ok := dl.Find("C"); ok {
		t.Errorf("Элемент 'C' не должен присутствовать в списке (найден на индексе %d)", idx)
	}

	// Прямое удаление по значению
	if err := dl.Remove("A"); err != nil {
		t.Fatalf("Ошибка Remove: %v", err)
	} // [D]

	front, _ := dl.Front()
	if front != "D" || dl.Size() != 1 {
		t.Errorf("Ожидался единственный элемент 'D', размер 1")
	}

	// Проверка Clear
	dl.Clear()
	if !dl.IsEmpty() || dl.Size() != 0 {
		t.Errorf("Список должен быть полностью очищен после Clear()")
	}
}
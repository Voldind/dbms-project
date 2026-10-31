package array

import (
	"testing"
)

func TestArray_PushAndGet(t *testing.T) {
	arr := NewDynamicArray[string]()

	if !arr.IsEmpty() {
		t.Errorf("ожидался пустой массив")
	}

	arr.PushBack("first")
	arr.PushBack("second")

	if arr.Size() != 2 {
		t.Errorf("ожидался размер 2, получено %d", arr.Size())
	}

	val, err := arr.Get(0)
	if err != nil || val != "first" {
		t.Errorf("ожидалось 'first', получено '%s', ошибка: %v", val, err)
	}

	// Проверка выхода за границы массива
	_, err = arr.Get(5)
	if err == nil {
		t.Errorf("ожидалась ошибка при выходе за границы индекса")
	}
}

func TestArray_SetAndRemove(t *testing.T) {
	arr := NewDynamicArray[int]()
	arr.PushBack(10)
	arr.PushBack(20)
	arr.PushBack(30) // [10, 20, 30]

	// Изменение элемента по индексу
	err := arr.Set(1, 25)
	if err != nil {
		t.Fatalf("ошибка метода Set: %v", err)
	}

	val, _ := arr.Get(1)
	if val != 25 {
		t.Errorf("ожидалось значение 25 на индексе 1, получено %d", val)
	}

	// Удаление элемента из середины
	err = arr.Remove(1) // Удаляем 25 -> остается [10, 30]
	if err != nil {
		t.Fatalf("ошибка метода Remove: %v", err)
	}

	if arr.Size() != 2 {
		t.Errorf("ожидался размер 2 после удаления, получено %d", arr.Size())
	}

	val, _ = arr.Get(1)
	if val != 30 {
		t.Errorf("ожидалось 30 на индексе 1 после сдвига элементов, получено %d", val)
	}
}
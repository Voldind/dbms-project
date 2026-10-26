package storage

import (
	"errors"
	"fmt"
	"strconv"
	"strings"

	"dbms/internal/parser"
	"dbms/internal/structures/array"
	"dbms/internal/structures/doublylist"
	"dbms/internal/structures/forwardlist"
	"dbms/internal/structures/queue"
	"dbms/internal/structures/stack"
)

var (
	ErrUnknownCommand    = errors.New("unknown command")
	ErrNotFound          = errors.New("structure not found")
	ErrNotEnoughArgs     = errors.New("not enough arguments for command")
	ErrInvalidIndex      = errors.New("index must be an integer")
	ErrMissingTargetName = errors.New("structure name is required")
)

type Database struct {
	arrays       map[string]*array.DynamicArray[string]
	forwardlists map[string]*forwardlist.ForwardList[string]
	doublylists  map[string]*doublylist.DoublyList[string]
	stacks       map[string]*stack.Stack[string]
	queues       map[string]*queue.Queue[string]
	filePath     string
}

func NewDatabase(filePath string) *Database {
	return &Database{
		arrays:       make(map[string]*array.DynamicArray[string]),
		forwardlists: make(map[string]*forwardlist.ForwardList[string]),
		doublylists:  make(map[string]*doublylist.DoublyList[string]),
		stacks:       make(map[string]*stack.Stack[string]),
		queues:       make(map[string]*queue.Queue[string]),
		filePath:     filePath,
	}
}

// ExecuteQuery направляет команду в нужный обработчик по первой букве или имени
func (db *Database) ExecuteQuery(q parser.Query) (string, error) {
	if q.Command == "HELP" || q.Command == "--HELP" || q.Command == "-H" {
		return db.getHelp(), nil
	}

	if q.Command == "PRINT" {
		return db.executePrint(q.Name)
	}

	if q.Name == "" {
		return "", ErrMissingTargetName
	}

	switch {
	case strings.HasPrefix(q.Command, "M"):
		return db.executeArray(q)
	case strings.HasPrefix(q.Command, "F"):
		return db.executeForwardList(q)
	case strings.HasPrefix(q.Command, "L"):
		return db.executeDoublyList(q)
	case strings.HasPrefix(q.Command, "S"):
		return db.executeStack(q)
	case strings.HasPrefix(q.Command, "Q"):
		return db.executeQueue(q)
	default:
		return "", fmt.Errorf("%w: %s", ErrUnknownCommand, q.Command)
	}
}

// ==========================================
// 1. ДИНАМИЧЕСКИЙ МАССИВ (M)
// ==========================================
func (db *Database) executeArray(q parser.Query) (string, error) {
	switch q.Command {
	case "MPUSH":
		if len(q.Args) == 0 {
			return "", ErrNotEnoughArgs
		}
		arr, exists := db.arrays[q.Name]
		if !exists {
			arr = array.NewDynamicArray[string]()
			db.arrays[q.Name] = arr
		}
		// Если передан 1 аргумент — добавляем в конец: MPUSH myarr val
		if len(q.Args) == 1 {
			arr.PushBack(q.Args[0])
			return q.Args[0], db.SaveToFile()
		}
		// Если 2 аргумента — вставка по индексу: MPUSH myarr idx val
		idx, err := strconv.Atoi(q.Args[0])
		if err != nil {
			return "", ErrInvalidIndex
		}
		if err := arr.InsertAt(idx, q.Args[1]); err != nil {
			return "", err
		}
		return q.Args[1], db.SaveToFile()

	case "MDEL":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		arr, exists := db.arrays[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		idx, err := strconv.Atoi(q.Args[0])
		if err != nil {
			return "", ErrInvalidIndex
		}
		if err := arr.Remove(idx); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "MGET":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		arr, exists := db.arrays[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		idx, err := strconv.Atoi(q.Args[0])
		if err != nil {
			return "", ErrInvalidIndex
		}
		return arr.Get(idx)

	case "MSET":
		if len(q.Args) < 2 {
			return "", ErrNotEnoughArgs
		}
		arr, exists := db.arrays[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		idx, err := strconv.Atoi(q.Args[0])
		if err != nil {
			return "", ErrInvalidIndex
		}
		if err := arr.Set(idx, q.Args[1]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "MSIZE":
		arr, exists := db.arrays[q.Name]
		if !exists {
			return "0", nil
		}
		return strconv.Itoa(arr.Size()), nil

	case "MCLEAR":
		arr, exists := db.arrays[q.Name]
		if !exists {
			return "OK", nil
		}
		arr.Clear()
		return "OK", db.SaveToFile()

	default:
		return "", fmt.Errorf("%w: %s", ErrUnknownCommand, q.Command)
	}
}

// ==========================================
// 2. ОДНОСВЯЗНЫЙ СПИСОК (F)
// ==========================================
func (db *Database) executeForwardList(q parser.Query) (string, error) {
	switch q.Command {
	case "FPUSHHEAD":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			fl = forwardlist.NewForwardList[string]()
			db.forwardlists[q.Name] = fl
		}
		fl.PushFront(q.Args[0])
		return q.Args[0], db.SaveToFile()

	case "FPUSHTAIL", "FPUSH":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			fl = forwardlist.NewForwardList[string]()
			db.forwardlists[q.Name] = fl
		}
		fl.PushBack(q.Args[0])
		return q.Args[0], db.SaveToFile()

	case "FADD": // Вставка ПОСЛЕ target: FADD name target val
		if len(q.Args) < 2 {
			return "", ErrNotEnoughArgs
		}
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := fl.InsertAfter(q.Args[0], q.Args[1]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "FADDBEFORE": // Вставка ДО target: FADDBEFORE name target val
		if len(q.Args) < 2 {
			return "", ErrNotEnoughArgs
		}
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := fl.InsertBefore(q.Args[0], q.Args[1]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "FDELHEAD":
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := fl.PopFront(); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "FDELTAIL":
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := fl.PopBack(); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "FDEL": // Удаление ПОСЛЕ target: FDEL name target
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := fl.RemoveAfter(q.Args[0]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "FDELBEFORE": // Удаление ДО target: FDELBEFORE name target
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := fl.RemoveBefore(q.Args[0]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "FVALDEL": // Удаление по значению: FVALDEL name target
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := fl.Remove(q.Args[0]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "FFIND":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "FALSE", nil
		}
		idx, ok := fl.Find(q.Args[0])
		if !ok {
			return "FALSE", nil
		}
		return fmt.Sprintf("TRUE (index: %d)", idx), nil

	case "FGET":
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if len(q.Args) == 0 || strings.ToUpper(q.Args[0]) == "HEAD" {
			return fl.Front()
		}
		if strings.ToUpper(q.Args[0]) == "TAIL" {
			return fl.Back()
		}
		idx, err := strconv.Atoi(q.Args[0])
		if err != nil {
			return "", ErrInvalidIndex
		}
		return fl.Get(idx)

	case "FSIZE":
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "0", nil
		}
		return strconv.Itoa(fl.Size()), nil

	case "FCLEAR":
		fl, exists := db.forwardlists[q.Name]
		if !exists {
			return "OK", nil
		}
		fl.Clear()
		return "OK", db.SaveToFile()

	default:
		return "", fmt.Errorf("%w: %s", ErrUnknownCommand, q.Command)
	}
}

// ==========================================
// 3. ДВУСВЯЗНЫЙ СПИСОК (L)
// ==========================================
func (db *Database) executeDoublyList(q parser.Query) (string, error) {
	switch q.Command {
	case "LPUSHHEAD":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		dl, exists := db.doublylists[q.Name]
		if !exists {
			dl = doublylist.NewDoublyList[string]()
			db.doublylists[q.Name] = dl
		}
		dl.PushFront(q.Args[0])
		return q.Args[0], db.SaveToFile()

	case "LPUSHTAIL", "LPUSH":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		dl, exists := db.doublylists[q.Name]
		if !exists {
			dl = doublylist.NewDoublyList[string]()
			db.doublylists[q.Name] = dl
		}
		dl.PushBack(q.Args[0])
		return q.Args[0], db.SaveToFile()

	case "LADD": // Вставка ПОСЛЕ target: LADD name target val
		if len(q.Args) < 2 {
			return "", ErrNotEnoughArgs
		}
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := dl.InsertAfter(q.Args[0], q.Args[1]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "LADDBEFORE": // Вставка ДО target: LADDBEFORE name target val
		if len(q.Args) < 2 {
			return "", ErrNotEnoughArgs
		}
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := dl.InsertBefore(q.Args[0], q.Args[1]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "LDELHEAD":
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := dl.PopFront(); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "LDELTAIL":
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := dl.PopBack(); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "LDEL": // Удаление ПОСЛЕ target: LDEL name target
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := dl.RemoveAfter(q.Args[0]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "LDELBEFORE": // Удаление ДО target: LDELBEFORE name target
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := dl.RemoveBefore(q.Args[0]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "LVALDEL": // Удаление по значению: LVALDEL name target
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if err := dl.Remove(q.Args[0]); err != nil {
			return "", err
		}
		return "OK", db.SaveToFile()

	case "LFIND":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "FALSE", nil
		}
		idx, ok := dl.Find(q.Args[0])
		if !ok {
			return "FALSE", nil
		}
		return fmt.Sprintf("TRUE (index: %d)", idx), nil

	case "LGET":
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		if len(q.Args) == 0 || strings.ToUpper(q.Args[0]) == "HEAD" {
			return dl.Front()
		}
		if strings.ToUpper(q.Args[0]) == "TAIL" {
			return dl.Back()
		}
		idx, err := strconv.Atoi(q.Args[0])
		if err != nil {
			return "", ErrInvalidIndex
		}
		return dl.Get(idx)

	case "LSIZE":
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "0", nil
		}
		return strconv.Itoa(dl.Size()), nil

	case "LCLEAR":
		dl, exists := db.doublylists[q.Name]
		if !exists {
			return "OK", nil
		}
		dl.Clear()
		return "OK", db.SaveToFile()

	default:
		return "", fmt.Errorf("%w: %s", ErrUnknownCommand, q.Command)
	}
}

// ==========================================
// 4. СТЕК (S)
// ==========================================
func (db *Database) executeStack(q parser.Query) (string, error) {
	switch q.Command {
	case "SPUSH":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		st, exists := db.stacks[q.Name]
		if !exists {
			st = stack.NewStack[string]()
			db.stacks[q.Name] = st
		}
		st.Push(q.Args[0])
		return q.Args[0], db.SaveToFile()

	case "SPOP":
		st, exists := db.stacks[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		val, err := st.Pop()
		if err != nil {
			return "", err
		}
		return val, db.SaveToFile()

	case "STOP", "SGET":
		st, exists := db.stacks[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		return st.Top()

	case "SSIZE":
		st, exists := db.stacks[q.Name]
		if !exists {
			return "0", nil
		}
		return strconv.Itoa(st.Size()), nil

	case "SCLEAR":
		st, exists := db.stacks[q.Name]
		if !exists {
			return "OK", nil
		}
		st.Clear()
		return "OK", db.SaveToFile()

	default:
		return "", fmt.Errorf("%w: %s", ErrUnknownCommand, q.Command)
	}
}

// ==========================================
// 5. ОЧЕРЕДЬ (Q)
// ==========================================
func (db *Database) executeQueue(q parser.Query) (string, error) {
	switch q.Command {
	case "QPUSH":
		if len(q.Args) < 1 {
			return "", ErrNotEnoughArgs
		}
		qu, exists := db.queues[q.Name]
		if !exists {
			qu = queue.NewQueue[string]()
			db.queues[q.Name] = qu
		}
		qu.Push(q.Args[0])
		return q.Args[0], db.SaveToFile()

	case "QPOP":
		qu, exists := db.queues[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		val, err := qu.Pop()
		if err != nil {
			return "", err
		}
		return val, db.SaveToFile()

	case "QFRONT", "QGET":
		qu, exists := db.queues[q.Name]
		if !exists {
			return "", ErrNotFound
		}
		return qu.Front()

	case "QSIZE":
		qu, exists := db.queues[q.Name]
		if !exists {
			return "0", nil
		}
		return strconv.Itoa(qu.Size()), nil

	case "QCLEAR":
		qu, exists := db.queues[q.Name]
		if !exists {
			return "OK", nil
		}
		qu.Clear()
		return "OK", db.SaveToFile()

	default:
		return "", fmt.Errorf("%w: %s", ErrUnknownCommand, q.Command)
	}
}

// ==========================================
// 6. ПЕЧАТЬ СТРУКТУР (PRINT) И СПРАВКА (HELP)
// ==========================================
func (db *Database) executePrint(name string) (string, error) {
	var sb strings.Builder

	// Если имя не указано — выводим вообще все структуры БД
	if name == "" {
		for k, v := range db.arrays {
			sb.WriteString(fmt.Sprintf("Array '%s': [%s]\n", k, strings.Join(v.ToSlice(), ", ")))
		}
		for k, v := range db.forwardlists {
			sb.WriteString(fmt.Sprintf("ForwardList '%s': [%s]\n", k, strings.Join(v.ToSlice(), " -> ")))
		}
		for k, v := range db.doublylists {
			sb.WriteString(fmt.Sprintf("DoublyList '%s': [%s]\n", k, strings.Join(v.ToSlice(), " <-> ")))
		}
		for k, v := range db.stacks {
			sb.WriteString(fmt.Sprintf("Stack '%s': [%s]\n", k, strings.Join(v.ToSlice(), ", ")))
		}
		for k, v := range db.queues {
			sb.WriteString(fmt.Sprintf("Queue '%s': [%s]\n", k, strings.Join(v.ToSlice(), ", ")))
		}
		if sb.Len() == 0 {
			return "Database is empty", nil
		}
		return strings.TrimRight(sb.String(), "\n"), nil
	}

	// Ищем конкретную структуру по имени
	found := false
	if v, ok := db.arrays[name]; ok {
		sb.WriteString(fmt.Sprintf("[%s]\n", strings.Join(v.ToSlice(), ", ")))
		found = true
	}
	if v, ok := db.forwardlists[name]; ok {
		sb.WriteString(fmt.Sprintf("[%s]\n", strings.Join(v.ToSlice(), " -> ")))
		found = true
	}
	if v, ok := db.doublylists[name]; ok {
		sb.WriteString(fmt.Sprintf("[%s]\n", strings.Join(v.ToSlice(), " <-> ")))
		found = true
	}
	if v, ok := db.stacks[name]; ok {
		sb.WriteString(fmt.Sprintf("[%s]\n", strings.Join(v.ToSlice(), ", ")))
		found = true
	}
	if v, ok := db.queues[name]; ok {
		sb.WriteString(fmt.Sprintf("[%s]\n", strings.Join(v.ToSlice(), ", ")))
		found = true
	}

	if !found {
		return "", ErrNotFound
	}
	return strings.TrimRight(sb.String(), "\n"), nil
}

func (db *Database) getHelp() string {
	return `Available DBMS Commands:
  Array (M):
    MPUSH <name> <val>            - Push value to the end of array
    MPUSH <name> <idx> <val>      - Insert value at index
    MDEL <name> <idx>             - Remove element at index
    MGET <name> <idx>             - Get element at index
    MSET <name> <idx> <val>       - Replace element at index
    MSIZE <name> / MCLEAR <name>  - Get size / Clear array

  ForwardList (F) & DoublyList (L) (replace F with L for DoublyList):
    FPUSHHEAD <name> <val>        - Push to front
    FPUSHTAIL <name> <val>        - Push to back
    FADD <name> <target> <val>    - Insert <val> AFTER <target>
    FADDBEFORE <name> <target> <val> - Insert <val> BEFORE <target>
    FDELHEAD <name> / FDELTAIL <name> - Delete front / back
    FDEL <name> <target>          - Delete element AFTER <target>
    FDELBEFORE <name> <target>    - Delete element BEFORE <target>
    FVALDEL <name> <target>       - Delete element by value <target>
    FFIND <name> <target>         - Find index of <target>
    FGET <name> [idx|HEAD|TAIL]   - Get element by index, HEAD, or TAIL
    FSIZE <name> / FCLEAR <name>  - Get size / Clear list

  Stack (S) & Queue (Q):
    SPUSH <name> <val> / SPOP <name> / STOP <name> / SSIZE <name> / SCLEAR <name>
    QPUSH <name> <val> / QPOP <name> / QFRONT <name> / QSIZE <name> / QCLEAR <name>

  General:
    PRINT [name]                  - Print specific structure or entire database
    HELP                          - Show this help message`
}

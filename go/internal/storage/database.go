package storage

import (
	"dbms/internal/structures/array"
	"dbms/internal/structures/doublylist"
	"dbms/internal/structures/forwardlist"
	"dbms/internal/structures/queue"
	"dbms/internal/structures/stack"
)

type Database struct {
	arrays       map[string]*array.DynamicArray[string]
	forwardlists map[string]*forwardlist.ForwardList[string]
	doublylists  map[string]*doublylist.DoublyList[string]
	stacks       map[string]*stack.Stack[string]
	queues       map[string]*queue.Queue[string]
	filePath string
}

func NewDatabase(filePath string) *Database {
	return &Database{
		arrays:       make(map[string]*array.DynamicArray[string]),
		forwardlists: make(map[string]*forwardlist.ForwardList[string]),
		doublylists:  make(map[string]*doublylist.DoublyList[string]),
		stacks:       make(map[string]*stack.Stack[string]),
		queues:       make(map[string]*queue.Queue[string]),
		filePath: filePath,
	}
}

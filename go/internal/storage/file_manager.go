package storage

import (
	"dbms/internal/structures/array"
	"dbms/internal/structures/doublylist"
	"dbms/internal/structures/forwardlist"
	"dbms/internal/structures/queue"
	"dbms/internal/structures/stack"
	"encoding/json"
	"os"
)

type databaseSnapshot struct {
	Arrays       map[string][]string `json:"arrays"`
	ForwardLists map[string][]string `json:"forward_lists"`
	DoublyLists  map[string][]string `json:"doubly_lists"`
	Stacks       map[string][]string `json:"stacks"`
	Queues       map[string][]string `json:"queues"`
}

func (db *Database) SaveToFile() error {
	snapshot := databaseSnapshot{
		Arrays:       make(map[string][]string),
		ForwardLists: make(map[string][]string),
		DoublyLists:  make(map[string][]string),
		Stacks:       make(map[string][]string),
		Queues:       make(map[string][]string),
	}
	for name, arr := range db.arrays {
		snapshot.Arrays[name] = arr.ToSlice()	
	}
	for name, flist := range db.forwardlists {
		snapshot.ForwardLists[name] = flist.ToSlice()	
	}
	for name, dlist := range db.doublylists {
		snapshot.DoublyLists[name] = dlist.ToSlice()	
	}
	for name, stack := range db.stacks {
		snapshot.Stacks[name] = stack.ToSlice()	
	}
	for name, queue := range db.queues {
		snapshot.Queues[name] = queue.ToSlice()	
	}
	data, err := json.MarshalIndent(snapshot, "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(db.filePath, data, 0644)
}

func (db *Database) LoadFromFile() error {
	data, err := os.ReadFile(db.filePath)
	if err != nil {
		if os.IsNotExist(err) {
			return nil
		}
		return err
	}
	if len(data) == 0 {
		return nil
	}
	var snapshot databaseSnapshot
	if err := json.Unmarshal(data, &snapshot); err != nil {
		return err
	} 
	for name, arr := range snapshot.Arrays {
		db.arrays[name] = array.NewDynamicArray[string]()
		for _, val := range arr {
			db.arrays[name].PushBack(val)
		}
	}
	for name, flist := range snapshot.ForwardLists {
		db.forwardlists[name] = forwardlist.NewForwardList[string]()
		for _, val := range flist {
			db.forwardlists[name].PushBack(val)
		}
	}
	for name, dlist := range snapshot.DoublyLists {
		db.doublylists[name] = doublylist.NewDoublyList[string]()
		for _, val := range dlist {
			db.doublylists[name].PushBack(val)
		}
	}
	for name, q := range snapshot.Queues {
		db.queues[name] = queue.NewQueue[string]()
		for _, val := range q {
			db.queues[name].Push(val)
		}
	}
	for name, st := range snapshot.Stacks {
		db.stacks[name] = stack.NewStack[string]()
		for _, val := range st {
			db.stacks[name].Push(val)
		}
	}
	return nil
}

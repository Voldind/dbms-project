package main

import (
	"flag"
	"fmt"
	"os"

	"dbms/internal/parser"
	"dbms/internal/storage"
)

func main() {
	// Чтение флагов командной строки --file и --query
	filePath := flag.String("file", "file.data", "path to the database file")
	queryStr := flag.String("query", "", "query to execute")
	flag.Parse()

	if *queryStr == "" {
		fmt.Println("Usage: ./dbms --file <filepath> --query '<COMMAND> <name> [args...]'")
		fmt.Println("Tip: run ./dbms --query 'HELP' to see all available commands.")
		os.Exit(1)
	}

	// Инициализация базы данных и загрузка данных из файла
	db := storage.NewDatabase(*filePath)
	if err := db.LoadFromFile(); err != nil {
		fmt.Fprintf(os.Stderr, "-> Error loading file: %v\n", err)
		os.Exit(1)
	}

	// Парсинг запроса
	q, err := parser.Parse(*queryStr)
	if err != nil {
		fmt.Fprintf(os.Stderr, "-> Parse error: %v\n", err)
		os.Exit(1)
	}

	// Выполнение запроса
	res, err := db.ExecuteQuery(q)
	if err != nil {
		fmt.Fprintf(os.Stderr, "-> Error: %v\n", err)
		os.Exit(1)
	}

	// Вывод результата
	if q.Command == "HELP" || q.Command == "PRINT" {
		fmt.Println(res)
	} else {
		fmt.Printf("-> %s\n", res)
	}
}
package parser

import (
	"errors"
	"strings"
)

var ErrEmptyCommand = errors.New("command is empty")

type Query struct {
	Command string
	Name    string
	Args    []string
}

func Parse(queryStr string) (Query, error) {
	tokens := strings.Fields(queryStr)
	if len(tokens) == 0 {
		return Query{}, ErrEmptyCommand
	}
	q := Query{Command: strings.ToUpper(tokens[0])}
	if len(tokens) > 1 {
		q.Name = tokens[1]
	}
	if len(tokens) > 2 {
		q.Args = tokens[2:]
	}
	return q, nil
}		

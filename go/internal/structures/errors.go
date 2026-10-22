package structures

import "errors"

var (
	ErrOutOfBounds = errors.New("index out of boudns")
	ErrNotFound    = errors.New("value not found")
	ErrEmpty       = errors.New("container is empty")
	ErrNotExist    = errors.New("value not exist")
)

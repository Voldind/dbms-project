package structures

import "errors"

var (
	ErrOutOfBounds = errors.New("index out of boudns")
	ErrNotFound    = errors.New("value not found")
)
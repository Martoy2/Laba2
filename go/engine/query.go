package engine

import (
	"errors"
	"strings"

	"dbms/structures"
)

// Tokenize делит запрос на слова. В двойных кавычках можно писать пробелы, \" — кавычка внутри.
func Tokenize(query string) (*structures.Array, error) {
	tokens := structures.NewArray()
	var token strings.Builder
	inToken := false
	inQuotes := false

	for i := 0; i < len(query); i++ {
		c := query[i]

		switch {
		case inQuotes:
			if c == '\\' && i+1 < len(query) {
				i++
				token.WriteByte(query[i])
			} else if c == '"' {
				inQuotes = false
			} else {
				token.WriteByte(c)
			}
		case c == '"':
			inQuotes = true
			inToken = true
		case structures.IsSpace(c):
			if inToken {
				tokens.PushBack(token.String())
				token.Reset()
				inToken = false
			}
		default:
			token.WriteByte(c)
			inToken = true
		}
	}

	if inQuotes {
		return nil, errors.New("query: unclosed quote")
	}
	if inToken {
		tokens.PushBack(token.String())
	}
	return tokens, nil
}

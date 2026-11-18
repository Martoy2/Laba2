package seventh

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"

	"dbms/structures"
)

func Run() int {
	reader := bufio.NewReader(os.Stdin)
	line, _ := reader.ReadString('\n')
	fields := structures.SplitFields(line)
	capacity := 0
	if len(fields) > 0 {
		capacity, _ = strconv.Atoi(fields[0])
	}

	cache, err := NewLRUCache(capacity)
	if err != nil {
		fmt.Fprintln(os.Stderr, "error: expected positive capacity")
		return 1
	}

	var rest strings.Builder
	for {
		chunk, err := reader.ReadString('\n')
		rest.WriteString(chunk)
		if err != nil {
			break
		}
	}
	tokens := structures.SplitFields(rest.String())

	var out []string
	for i := 0; i < len(tokens); i++ {
		switch tokens[i] {
		case "SET":
			if i+2 >= len(tokens) {
				fmt.Fprintln(os.Stderr, "error: SET needs key and value")
				return 1
			}
			cache.Set(tokens[i+1], tokens[i+2])
			i += 2
		case "GET":
			if i+1 >= len(tokens) {
				fmt.Fprintln(os.Stderr, "error: GET needs key")
				return 1
			}
			if value, ok := cache.Get(tokens[i+1]); ok {
				out = append(out, value)
			} else {
				out = append(out, "-1")
			}
			i++
		default:
			fmt.Fprintln(os.Stderr, "error: unknown query:", tokens[i])
			return 1
		}
	}
	fmt.Println(strings.Join(out, " "))
	return 0
}

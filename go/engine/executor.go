package engine

import (
	"errors"
	"strconv"

	"dbms/structures"
)

// list — общие операции односвязного и двусвязного списков, чтобы F и L обрабатывались одним кодом.
type list interface {
	structures.Container
	PushFront(value string)
	PushBack(value string)
	InsertBefore(target, value string) error
	InsertAfter(target, value string) error
	PopFront() error
	PopBack() error
	RemoveBefore(target string) error
	RemoveAfter(target string) error
	RemoveValue(value string) error
	Front() (string, error)
	Back() (string, error)
	Find(value string) bool
	Size() int
	ReverseString() string
}

func arg(tokens *structures.Array, index int) string {
	value, _ := tokens.Get(index)
	return value
}

func toUpper(text string) string {
	b := []byte(text)
	for i, c := range b {
		if c >= 'a' && c <= 'z' {
			b[i] = c - 'a' + 'A'
		}
	}
	return string(b)
}

func requireArgs(tokens *structures.Array, count int) error {
	if tokens.Size() != count {
		return errors.New("wrong number of arguments for " + arg(tokens, 0))
	}
	return nil
}

func parseIndex(token string) (int, error) {
	if token == "" || len(token) > 18 {
		return 0, errors.New("bad index: " + token)
	}

	index := 0
	for i := 0; i < len(token); i++ {
		if token[i] < '0' || token[i] > '9' {
			return 0, errors.New("bad index: " + token)
		}
		index = index*10 + int(token[i]-'0')
	}
	return index, nil
}

func checkName(name string) error {
	if name == "" {
		return errors.New("empty structure name")
	}
	for i := 0; i < len(name); i++ {
		if structures.IsSpace(name[i]) || name[i] == '\\' {
			return errors.New("bad structure name: " + name)
		}
	}
	return nil
}

// findAs находит структуру по имени и проверяет её тип; если create, то создаёт новую.
func findAs(storage *Storage, name string, t byte, create bool) (structures.Container, error) {
	container := storage.Find(name)
	if container == nil {
		if !create {
			return nil, errors.New("structure not found: " + name)
		}
		if err := checkName(name); err != nil {
			return nil, err
		}
		container = Create(t)
		if err := storage.Add(name, container); err != nil {
			return nil, err
		}
	}

	if container.Type() != t {
		return nil, errors.New(name + " is not of type " + string(t))
	}
	return container, nil
}

func boolText(value bool) string {
	if value {
		return "TRUE"
	}
	return "FALSE"
}

func executeArray(storage *Storage, op string, tokens *structures.Array) (string, bool, error) {
	name := arg(tokens, 1)
	create := op == "PUSH" || op == "INSERT"

	counts := map[string]int{"PUSH": 3, "INSERT": 4, "GET": 3, "DEL": 3, "SET": 4, "LEN": 2}
	count, ok := counts[op]
	if !ok {
		return "", false, errors.New("unknown command: " + arg(tokens, 0))
	}
	if err := requireArgs(tokens, count); err != nil {
		return "", false, err
	}

	// Индекс у INSERT проверяется до создания массива, у остальных команд после поиска (как в C++).
	var index int
	var err error
	if op == "INSERT" {
		if index, err = parseIndex(arg(tokens, 2)); err != nil {
			return "", false, err
		}
	}

	container, err := findAs(storage, name, 'M', create)
	if err != nil {
		return "", false, err
	}
	array := container.(*structures.Array)

	if op == "GET" || op == "DEL" || op == "SET" {
		if index, err = parseIndex(arg(tokens, 2)); err != nil {
			return "", false, err
		}
	}

	switch op {
	case "PUSH":
		array.PushBack(arg(tokens, 2))
		return arg(tokens, 2), true, nil
	case "INSERT":
		if err := array.Insert(index, arg(tokens, 3)); err != nil {
			return "", false, err
		}
		return arg(tokens, 3), true, nil
	case "GET":
		value, err := array.Get(index)
		return value, false, err
	case "DEL":
		value, err := array.Get(index)
		if err != nil {
			return "", false, err
		}
		array.Remove(index)
		return value, true, nil
	case "SET":
		if err := array.Set(index, arg(tokens, 3)); err != nil {
			return "", false, err
		}
		return arg(tokens, 3), true, nil
	default:
		return strconv.Itoa(array.Size()), false, nil
	}
}

func executeList(storage *Storage, t byte, op string, tokens *structures.Array) (string, bool, error) {
	name := arg(tokens, 1)
	create := op == "PUSH" || op == "PUSHTAIL" || op == "PUSHHEAD"

	counts := map[string]int{
		"PUSH": 3, "PUSHTAIL": 3, "PUSHHEAD": 3, "PUSHBEFORE": 4, "PUSHAFTER": 4,
		"DELHEAD": 2, "DELTAIL": 2, "DELBEFORE": 3, "DELAFTER": 3, "DEL": 3,
		"FIND": 3, "GET": 2, "GETREV": 2, "LEN": 2,
	}
	count, ok := counts[op]
	if !ok {
		return "", false, errors.New("unknown command: " + arg(tokens, 0))
	}
	if err := requireArgs(tokens, count); err != nil {
		return "", false, err
	}

	container, err := findAs(storage, name, t, create)
	if err != nil {
		return "", false, err
	}
	l := container.(list)

	switch op {
	case "PUSH", "PUSHTAIL":
		l.PushBack(arg(tokens, 2))
		return arg(tokens, 2), true, nil
	case "PUSHHEAD":
		l.PushFront(arg(tokens, 2))
		return arg(tokens, 2), true, nil
	case "PUSHBEFORE":
		if err := l.InsertBefore(arg(tokens, 2), arg(tokens, 3)); err != nil {
			return "", false, err
		}
		return arg(tokens, 3), true, nil
	case "PUSHAFTER":
		if err := l.InsertAfter(arg(tokens, 2), arg(tokens, 3)); err != nil {
			return "", false, err
		}
		return arg(tokens, 3), true, nil
	case "DELHEAD":
		value, err := l.Front()
		if err != nil {
			return "", false, err
		}
		l.PopFront()
		return value, true, nil
	case "DELTAIL":
		value, err := l.Back()
		if err != nil {
			return "", false, err
		}
		l.PopBack()
		return value, true, nil
	case "DELBEFORE":
		if err := l.RemoveBefore(arg(tokens, 2)); err != nil {
			return "", false, err
		}
		return "OK", true, nil
	case "DELAFTER":
		if err := l.RemoveAfter(arg(tokens, 2)); err != nil {
			return "", false, err
		}
		return "OK", true, nil
	case "DEL":
		if err := l.RemoveValue(arg(tokens, 2)); err != nil {
			return "", false, err
		}
		return arg(tokens, 2), true, nil
	case "FIND":
		return boolText(l.Find(arg(tokens, 2))), false, nil
	case "GET":
		return l.String(), false, nil
	case "GETREV":
		return l.ReverseString(), false, nil
	default:
		return strconv.Itoa(l.Size()), false, nil
	}
}

func executeStack(storage *Storage, op string, tokens *structures.Array) (string, bool, error) {
	counts := map[string]int{"PUSH": 3, "POP": 2, "PEEK": 2, "LEN": 2}
	count, ok := counts[op]
	if !ok {
		return "", false, errors.New("unknown command: " + arg(tokens, 0))
	}
	if err := requireArgs(tokens, count); err != nil {
		return "", false, err
	}

	container, err := findAs(storage, arg(tokens, 1), 'S', op == "PUSH")
	if err != nil {
		return "", false, err
	}
	stack := container.(*structures.Stack)

	switch op {
	case "PUSH":
		stack.Push(arg(tokens, 2))
		return arg(tokens, 2), true, nil
	case "POP":
		value, err := stack.Pop()
		return value, err == nil, err
	case "PEEK":
		value, err := stack.Top()
		return value, false, err
	default:
		return strconv.Itoa(stack.Size()), false, nil
	}
}

func executeQueue(storage *Storage, op string, tokens *structures.Array) (string, bool, error) {
	counts := map[string]int{"PUSH": 3, "POP": 2, "PEEK": 2, "LEN": 2}
	count, ok := counts[op]
	if !ok {
		return "", false, errors.New("unknown command: " + arg(tokens, 0))
	}
	if err := requireArgs(tokens, count); err != nil {
		return "", false, err
	}

	container, err := findAs(storage, arg(tokens, 1), 'Q', op == "PUSH")
	if err != nil {
		return "", false, err
	}
	queue := container.(*structures.Queue)

	switch op {
	case "PUSH":
		queue.Push(arg(tokens, 2))
		return arg(tokens, 2), true, nil
	case "POP":
		value, err := queue.Pop()
		return value, err == nil, err
	case "PEEK":
		value, err := queue.Front()
		return value, false, err
	default:
		return strconv.Itoa(queue.Size()), false, nil
	}
}

func executeSet(storage *Storage, command string, tokens *structures.Array) (string, bool, error) {
	if err := requireArgs(tokens, 3); err != nil {
		return "", false, err
	}
	name := arg(tokens, 1)
	value := arg(tokens, 2)

	container, err := findAs(storage, name, 'H', command == "SETADD")
	if err != nil {
		return "", false, err
	}
	set := container.(*structures.HashSet)

	switch command {
	case "SETADD":
		return value, set.Add(value), nil
	case "SETDEL":
		if !set.Remove(value) {
			return "", false, errors.New("value not found: " + value)
		}
		return value, true, nil
	default:
		return boolText(set.Contains(value)), false, nil
	}
}

// Execute выполняет одну команду. changed = true, если данные изменились и их надо сохранить.
func Execute(storage *Storage, tokens *structures.Array) (result string, changed bool, err error) {
	if tokens.Size() < 2 {
		return "", false, errors.New("expected: COMMAND name [args...]")
	}

	command := toUpper(arg(tokens, 0))
	if len(command) < 2 {
		return "", false, errors.New("unknown command: " + arg(tokens, 0))
	}

	if command == "PRINT" {
		if err := requireArgs(tokens, 2); err != nil {
			return "", false, err
		}
		container := storage.Find(arg(tokens, 1))
		if container == nil {
			return "", false, errors.New("structure not found: " + arg(tokens, 1))
		}
		return container.String(), false, nil
	}

	if command == "SETADD" || command == "SETDEL" || command == "SET_AT" {
		return executeSet(storage, command, tokens)
	}

	op := command[1:]
	switch command[0] {
	case 'M':
		return executeArray(storage, op, tokens)
	case 'F', 'L':
		return executeList(storage, command[0], op, tokens)
	case 'S':
		return executeStack(storage, op, tokens)
	case 'Q':
		return executeQueue(storage, op, tokens)
	default:
		return "", false, errors.New("unknown command: " + arg(tokens, 0))
	}
}

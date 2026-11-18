package engine

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"

	"dbms/structures"
)

type entry struct {
	name      string
	container structures.Container
	next      *entry
}

// Storage — список «имя → структура». Порядок добавления сохраняется и в файле.
type Storage struct {
	head *entry
	tail *entry
	sz   int
}

func (s *Storage) Find(name string) structures.Container {
	for cur := s.head; cur != nil; cur = cur.next {
		if cur.name == name {
			return cur.container
		}
	}
	return nil
}

func (s *Storage) Add(name string, container structures.Container) error {
	if s.Find(name) != nil {
		return errors.New("Storage: name already exists")
	}

	e := &entry{name: name, container: container}
	if s.tail == nil {
		s.head = e
		s.tail = e
	} else {
		s.tail.next = e
		s.tail = e
	}
	s.sz++
	return nil
}

func (s *Storage) Remove(name string) error {
	var prev *entry
	cur := s.head
	for cur != nil && cur.name != name {
		prev = cur
		cur = cur.next
	}
	if cur == nil {
		return errors.New("Storage: name not found")
	}

	if prev == nil {
		s.head = cur.next
	} else {
		prev.next = cur.next
	}
	if cur == s.tail {
		s.tail = prev
	}
	s.sz--
	return nil
}

func (s *Storage) Size() int {
	return s.sz
}

func (s *Storage) Clear() {
	s.head = nil
	s.tail = nil
	s.sz = 0
}

func Create(t byte) structures.Container {
	switch t {
	case 'M':
		return structures.NewArray()
	case 'F':
		return structures.NewForwardList()
	case 'L':
		return structures.NewDoublyList()
	case 'S':
		return structures.NewStack()
	case 'Q':
		return structures.NewQueue()
	case 'H':
		return structures.NewHashSet()
	default:
		return nil
	}
}

// nextToken пропускает пробелы и читает слово, как оператор >> в C++.
func nextToken(line string, pos int) (string, int) {
	for pos < len(line) && structures.IsSpace(line[pos]) {
		pos++
	}
	start := pos
	for pos < len(line) && !structures.IsSpace(line[pos]) {
		pos++
	}
	return line[start:pos], pos
}

func (s *Storage) Load(path string) error {
	s.Clear()

	file, err := os.Open(path)
	if errors.Is(err, os.ErrNotExist) {
		return nil
	}
	if err != nil {
		return err
	}
	defer file.Close()

	reader := bufio.NewReader(file)
	lineNumber := 0
	for {
		line, readErr := reader.ReadString('\n')
		if readErr != nil && readErr != io.EOF {
			return readErr
		}
		if readErr == io.EOF && line == "" {
			break
		}

		lineNumber++
		line = strings.TrimSuffix(line, "\n")
		if strings.Trim(line, " \t\r") == "" {
			if readErr == io.EOF {
				break
			}
			continue
		}

		typeToken, pos := nextToken(line, 0)
		name, pos := nextToken(line, pos)
		if name == "" || len(typeToken) != 1 {
			return fmt.Errorf("Storage: bad line %d", lineNumber)
		}

		container := Create(typeToken[0])
		if container == nil {
			return fmt.Errorf("Storage: unknown type on line %d", lineNumber)
		}
		if err := s.Add(name, container); err != nil {
			return err
		}
		container.Deserialize(line[pos:])

		if readErr == io.EOF {
			break
		}
	}
	return nil
}

// Save пишет во временный файл и подменяет им старый, чтобы сбой не испортил данные.
func (s *Storage) Save(path string) error {
	tmpPath := path + ".tmp"
	file, err := os.Create(tmpPath)
	if err != nil {
		return fmt.Errorf("Storage: cannot open %s", tmpPath)
	}

	writer := bufio.NewWriter(file)
	for cur := s.head; cur != nil; cur = cur.next {
		writer.WriteByte(cur.container.Type())
		writer.WriteByte(' ')
		writer.WriteString(cur.name)
		if data := cur.container.Serialize(); data != "" {
			writer.WriteByte(' ')
			writer.WriteString(data)
		}
		writer.WriteByte('\n')
	}

	if err := writer.Flush(); err != nil {
		file.Close()
		os.Remove(tmpPath)
		return fmt.Errorf("Storage: cannot write %s", tmpPath)
	}
	if err := file.Close(); err != nil {
		os.Remove(tmpPath)
		return fmt.Errorf("Storage: cannot write %s", tmpPath)
	}

	if err := os.Rename(tmpPath, path); err != nil {
		os.Remove(tmpPath)
		return fmt.Errorf("Storage: cannot replace %s", path)
	}
	return nil
}

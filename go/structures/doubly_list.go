package structures

import (
	"errors"
	"strings"
)

var (
	errDLEmpty    = errors.New("DoublyList: list is empty")
	errDLNotFound = errors.New("DoublyList: element not found")
	errDLNoBefore = errors.New("DoublyList: no element before target")
	errDLNoAfter  = errors.New("DoublyList: no element after target")
)

// DoublyList — двусвязный список: каждый узел знает соседа слева и справа.
type DoublyList struct {
	head *DNode
	tail *DNode
	sz   int
}

func NewDoublyList(values ...string) *DoublyList {
	l := &DoublyList{}
	for _, value := range values {
		l.PushBack(value)
	}
	return l
}

func (l *DoublyList) findNode(value string) *DNode { // O(n) - перебор
	for cur := l.head; cur != nil; cur = cur.Next {
		if cur.Value == value {
			return cur
		}
	}
	return nil
}

// unlink вырезает любой узел за O(1); на нём построены все удаления.
func (l *DoublyList) unlink(node *DNode) { // O(1)
	if node.Prev != nil {
		node.Prev.Next = node.Next
	} else {
		l.head = node.Next
	}

	if node.Next != nil {
		node.Next.Prev = node.Prev
	} else {
		l.tail = node.Prev
	}

	node.Prev = nil
	node.Next = nil
	l.sz--
}

func (l *DoublyList) PushFront(value string) { // O(1)
	node := &DNode{Value: value, Next: l.head}
	if l.head != nil {
		l.head.Prev = node
	} else {
		l.tail = node
	}
	l.head = node
	l.sz++
}

func (l *DoublyList) PushBack(value string) { // O(1)
	node := &DNode{Value: value, Prev: l.tail}
	if l.tail != nil {
		l.tail.Next = node
	} else {
		l.head = node
	}
	l.tail = node
	l.sz++
}

func (l *DoublyList) InsertBefore(target, value string) error { // O(n) - поиск, вставка O(1)
	node := l.findNode(target)
	if node == nil {
		return errDLNotFound
	}

	if node == l.head {
		l.PushFront(value)
		return nil
	}

	newNode := &DNode{Value: value, Prev: node.Prev, Next: node}
	node.Prev.Next = newNode
	node.Prev = newNode
	l.sz++
	return nil
}

func (l *DoublyList) InsertAfter(target, value string) error { // O(n) - поиск, вставка O(1)
	node := l.findNode(target)
	if node == nil {
		return errDLNotFound
	}

	if node == l.tail {
		l.PushBack(value)
		return nil
	}

	newNode := &DNode{Value: value, Prev: node, Next: node.Next}
	node.Next.Prev = newNode
	node.Next = newNode
	l.sz++
	return nil
}

func (l *DoublyList) PopFront() error { // O(1)
	if l.head == nil {
		return errDLEmpty
	}

	l.unlink(l.head)
	return nil
}

func (l *DoublyList) PopBack() error { // O(1) - есть prev
	if l.tail == nil {
		return errDLEmpty
	}

	l.unlink(l.tail)
	return nil
}

func (l *DoublyList) RemoveBefore(target string) error { // O(n) - поиск, удаление O(1)
	node := l.findNode(target)
	if node == nil {
		return errDLNotFound
	}
	if node.Prev == nil {
		return errDLNoBefore
	}

	l.unlink(node.Prev)
	return nil
}

func (l *DoublyList) RemoveAfter(target string) error { // O(n) - поиск, удаление O(1)
	node := l.findNode(target)
	if node == nil {
		return errDLNotFound
	}
	if node.Next == nil {
		return errDLNoAfter
	}

	l.unlink(node.Next)
	return nil
}

func (l *DoublyList) RemoveValue(value string) error { // O(n) - поиск, удаление O(1)
	node := l.findNode(value)
	if node == nil {
		return errDLNotFound
	}

	l.unlink(node)
	return nil
}

func (l *DoublyList) Front() (string, error) { // O(1)
	if l.head == nil {
		return "", errDLEmpty
	}
	return l.head.Value, nil
}

func (l *DoublyList) Back() (string, error) { // O(1)
	if l.tail == nil {
		return "", errDLEmpty
	}
	return l.tail.Value, nil
}

func (l *DoublyList) Find(value string) bool { // O(n)
	return l.findNode(value) != nil
}

func (l *DoublyList) Size() int { // O(1)
	return l.sz
}

func (l *DoublyList) Empty() bool { // O(1)
	return l.sz == 0
}

func (l *DoublyList) Clear() { // O(1) - узлы соберёт сборщик мусора
	l.head = nil
	l.tail = nil
	l.sz = 0
}

func (l *DoublyList) Type() byte {
	return 'L'
}

func (l *DoublyList) Serialize() string { // O(n)
	var b strings.Builder
	for cur := l.head; cur != nil; cur = cur.Next {
		b.WriteString(EscapeValue(cur.Value))
		if cur.Next != nil {
			b.WriteByte(' ')
		}
	}
	return b.String()
}

func (l *DoublyList) Deserialize(line string) { // O(n)
	l.Clear()
	for _, token := range SplitFields(line) {
		l.PushBack(UnescapeValue(token))
	}
}

func (l *DoublyList) String() string { // O(n)
	var b strings.Builder
	b.WriteByte('[')
	for cur := l.head; cur != nil; cur = cur.Next {
		b.WriteString(cur.Value)
		if cur.Next != nil {
			b.WriteString(", ")
		}
	}
	b.WriteByte(']')
	return b.String()
}

// ReverseString выводит список с хвоста, обычным циклом по Prev.
func (l *DoublyList) ReverseString() string { // O(n)
	var b strings.Builder
	b.WriteByte('[')
	for cur := l.tail; cur != nil; cur = cur.Prev {
		b.WriteString(cur.Value)
		if cur.Prev != nil {
			b.WriteString(", ")
		}
	}
	b.WriteByte(']')
	return b.String()
}

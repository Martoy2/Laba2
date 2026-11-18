package structures

import (
	"errors"
	"strings"
)

var (
	errFLEmpty    = errors.New("ForwardList: list is empty")
	errFLNotFound = errors.New("ForwardList: element not found")
	errFLNoBefore = errors.New("ForwardList: no element before target")
	errFLNoAfter  = errors.New("ForwardList: no element after target")
)

// ForwardList — односвязный список с указателями на голову и хвост.
type ForwardList struct {
	head *Node
	tail *Node
	sz   int
}

func NewForwardList(values ...string) *ForwardList {
	l := &ForwardList{}
	for _, value := range values {
		l.PushBack(value)
	}
	return l
}

func (l *ForwardList) findNode(value string) *Node { // O(n) - перебор
	for cur := l.head; cur != nil; cur = cur.Next {
		if cur.Value == value {
			return cur
		}
	}
	return nil
}

func (l *ForwardList) PushFront(value string) { // O(1)
	l.head = &Node{Value: value, Next: l.head}
	if l.tail == nil {
		l.tail = l.head
	}
	l.sz++
}

func (l *ForwardList) PushBack(value string) { // O(1) - есть tail
	node := &Node{Value: value}
	if l.tail == nil {
		l.head = node
		l.tail = node
	} else {
		l.tail.Next = node
		l.tail = node
	}
	l.sz++
}

func (l *ForwardList) InsertBefore(target, value string) error { // O(n) - поиск предыдущего
	if l.head == nil {
		return errFLNotFound
	}

	if l.head.Value == target {
		l.PushFront(value)
		return nil
	}

	prev := l.head
	for prev.Next != nil && prev.Next.Value != target {
		prev = prev.Next
	}
	if prev.Next == nil {
		return errFLNotFound
	}

	prev.Next = &Node{Value: value, Next: prev.Next}
	l.sz++
	return nil
}

func (l *ForwardList) InsertAfter(target, value string) error { // O(n) - поиск, вставка O(1)
	node := l.findNode(target)
	if node == nil {
		return errFLNotFound
	}

	node.Next = &Node{Value: value, Next: node.Next}
	if node == l.tail {
		l.tail = node.Next
	}
	l.sz++
	return nil
}

func (l *ForwardList) PopFront() error { // O(1)
	if l.head == nil {
		return errFLEmpty
	}

	l.head = l.head.Next
	if l.head == nil {
		l.tail = nil
	}
	l.sz--
	return nil
}

func (l *ForwardList) PopBack() error { // O(n) - поиск предпоследнего
	if l.head == nil {
		return errFLEmpty
	}

	if l.head == l.tail {
		l.head = nil
		l.tail = nil
		l.sz--
		return nil
	}

	prev := l.head
	for prev.Next != l.tail {
		prev = prev.Next
	}
	prev.Next = nil
	l.tail = prev
	l.sz--
	return nil
}

func (l *ForwardList) RemoveBefore(target string) error { // O(n)
	if l.head == nil || l.head.Value == target {
		return errFLNoBefore
	}

	if l.head.Next != nil && l.head.Next.Value == target {
		return l.PopFront()
	}

	prev := l.head
	for prev.Next != nil && prev.Next.Next != nil && prev.Next.Next.Value != target {
		prev = prev.Next
	}
	if prev.Next == nil || prev.Next.Next == nil {
		return errFLNotFound
	}

	prev.Next = prev.Next.Next
	l.sz--
	return nil
}

func (l *ForwardList) RemoveAfter(target string) error { // O(n) - поиск, удаление O(1)
	node := l.findNode(target)
	if node == nil {
		return errFLNotFound
	}
	if node.Next == nil {
		return errFLNoAfter
	}

	if node.Next == l.tail {
		l.tail = node
	}
	node.Next = node.Next.Next
	l.sz--
	return nil
}

func (l *ForwardList) RemoveValue(value string) error { // O(n)
	if l.head == nil {
		return errFLNotFound
	}

	if l.head.Value == value {
		return l.PopFront()
	}

	prev := l.head
	for prev.Next != nil && prev.Next.Value != value {
		prev = prev.Next
	}
	if prev.Next == nil {
		return errFLNotFound
	}

	if prev.Next == l.tail {
		l.tail = prev
	}
	prev.Next = prev.Next.Next
	l.sz--
	return nil
}

func (l *ForwardList) Front() (string, error) { // O(1)
	if l.head == nil {
		return "", errFLEmpty
	}
	return l.head.Value, nil
}

func (l *ForwardList) Back() (string, error) { // O(1)
	if l.tail == nil {
		return "", errFLEmpty
	}
	return l.tail.Value, nil
}

func (l *ForwardList) Find(value string) bool { // O(n)
	return l.findNode(value) != nil
}

func (l *ForwardList) Size() int { // O(1)
	return l.sz
}

func (l *ForwardList) Empty() bool { // O(1)
	return l.sz == 0
}

func (l *ForwardList) Clear() { // O(1) - узлы соберёт сборщик мусора
	l.head = nil
	l.tail = nil
	l.sz = 0
}

func (l *ForwardList) Type() byte {
	return 'F'
}

func (l *ForwardList) Serialize() string { // O(n)
	var b strings.Builder
	for cur := l.head; cur != nil; cur = cur.Next {
		b.WriteString(EscapeValue(cur.Value))
		if cur.Next != nil {
			b.WriteByte(' ')
		}
	}
	return b.String()
}

func (l *ForwardList) Deserialize(line string) { // O(n)
	l.Clear()
	for _, token := range SplitFields(line) {
		l.PushBack(UnescapeValue(token))
	}
}

func (l *ForwardList) String() string { // O(n)
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

// ReverseString выводит список с хвоста. Ссылок назад нет, поэтому через рекурсию.
func (l *ForwardList) ReverseString() string { // O(n), рекурсия глубиной n
	var b strings.Builder
	b.WriteByte('[')
	writeReverse(&b, l.head)
	b.WriteByte(']')
	return b.String()
}

func writeReverse(b *strings.Builder, node *Node) { // O(n)
	if node == nil {
		return
	}

	writeReverse(b, node.Next)
	if node.Next != nil {
		b.WriteString(", ")
	}
	b.WriteString(node.Value)
}

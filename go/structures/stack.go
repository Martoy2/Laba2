package structures

import "errors"

var errStackEmpty = errors.New("Stack: stack is empty")

// Stack хранит односвязный список; вершина стека — голова списка.
type Stack struct {
	list ForwardList
}

func NewStack(values ...string) *Stack {
	s := &Stack{}
	for _, value := range values {
		s.Push(value)
	}
	return s
}

func (s *Stack) Push(value string) { // O(1)
	s.list.PushFront(value)
}

func (s *Stack) Pop() (string, error) { // O(1)
	if s.list.Empty() {
		return "", errStackEmpty
	}

	value, _ := s.list.Front()
	s.list.PopFront()
	return value, nil
}

func (s *Stack) Top() (string, error) { // O(1)
	if s.list.Empty() {
		return "", errStackEmpty
	}
	return s.list.Front()
}

func (s *Stack) Size() int { // O(1)
	return s.list.Size()
}

func (s *Stack) Empty() bool { // O(1)
	return s.list.Empty()
}

func (s *Stack) Clear() { // O(1)
	s.list.Clear()
}

func (s *Stack) Type() byte {
	return 'S'
}

func (s *Stack) Serialize() string { // O(n)
	return s.list.Serialize()
}

func (s *Stack) Deserialize(line string) { // O(n)
	s.list.Deserialize(line)
}

func (s *Stack) String() string { // O(n)
	return s.list.String()
}

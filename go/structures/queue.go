package structures

import "errors"

var errQueueEmpty = errors.New("Queue: queue is empty")

// Queue хранит односвязный список: добавляем в хвост, забираем из головы.
type Queue struct {
	list ForwardList
}

func NewQueue(values ...string) *Queue {
	q := &Queue{}
	for _, value := range values {
		q.Push(value)
	}
	return q
}

func (q *Queue) Push(value string) { // O(1)
	q.list.PushBack(value)
}

func (q *Queue) Pop() (string, error) { // O(1)
	if q.list.Empty() {
		return "", errQueueEmpty
	}

	value, _ := q.list.Front()
	q.list.PopFront()
	return value, nil
}

func (q *Queue) Front() (string, error) { // O(1)
	if q.list.Empty() {
		return "", errQueueEmpty
	}
	return q.list.Front()
}

func (q *Queue) Size() int { // O(1)
	return q.list.Size()
}

func (q *Queue) Empty() bool { // O(1)
	return q.list.Empty()
}

func (q *Queue) Clear() { // O(1)
	q.list.Clear()
}

func (q *Queue) Type() byte {
	return 'Q'
}

func (q *Queue) Serialize() string { // O(n)
	return q.list.Serialize()
}

func (q *Queue) Deserialize(line string) { // O(n)
	q.list.Deserialize(line)
}

func (q *Queue) String() string { // O(n)
	return q.list.String()
}

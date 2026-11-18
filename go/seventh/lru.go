package seventh

import (
	"errors"

	"dbms/structures"
)

type entry struct {
	value string
	node  *structures.DNode
}

// LRUCache — двусвязный список из ЛР1 (свежие в голове) + хеш-таблица ключ → узел.
type LRUCache struct {
	table *structures.HashTable[entry]
	head  *structures.DNode
	tail  *structures.DNode
	cap   int
	sz    int
}

func NewLRUCache(capacity int) (*LRUCache, error) {
	if capacity <= 0 {
		return nil, errors.New("LRUCache: capacity must be positive")
	}
	return &LRUCache{table: structures.NewHashTable[entry](structures.Double, 11, 0.7), cap: capacity}, nil
}

func (c *LRUCache) pushFront(n *structures.DNode) { // O(1)
	n.Prev = nil
	n.Next = c.head
	if c.head != nil {
		c.head.Prev = n
	} else {
		c.tail = n
	}
	c.head = n
}

func (c *LRUCache) unlink(n *structures.DNode) { // O(1)
	if n.Prev != nil {
		n.Prev.Next = n.Next
	} else {
		c.head = n.Next
	}

	if n.Next != nil {
		n.Next.Prev = n.Prev
	} else {
		c.tail = n.Prev
	}
}

func (c *LRUCache) Set(key, value string) { // O(1) в среднем
	if e := c.table.Find(key); e != nil {
		e.value = value
		c.unlink(e.node)
		c.pushFront(e.node)
		return
	}

	if c.sz == c.cap {
		oldest := c.tail
		c.unlink(oldest)
		c.table.Remove(oldest.Value)
		c.sz--
	}

	n := &structures.DNode{Value: key}
	c.pushFront(n)
	c.table.Insert(key, entry{value: value, node: n})
	c.sz++
}

func (c *LRUCache) Get(key string) (string, bool) { // O(1) в среднем
	e := c.table.Find(key)
	if e == nil {
		return "", false
	}

	c.unlink(e.node)
	c.pushFront(e.node)
	return e.value, true
}

func (c *LRUCache) Size() int { // O(1)
	return c.sz
}

func (c *LRUCache) Capacity() int { // O(1)
	return c.cap
}

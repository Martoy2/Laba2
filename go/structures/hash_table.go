package structures

type Probing int

const (
	Linear Probing = iota
	Double
)

type cellState uint8

const (
	cellEmpty cellState = iota
	cellBusy
	cellDeleted
)

type cell[V any] struct {
	key   string
	value V
	state cellState
}

// HashTable — хеш-таблица с открытой адресацией: линейное пробирование или двойное хеширование.
type HashTable[V any] struct {
	cells   []cell[V]
	probing Probing
	maxLoad float64
	sz      int
	used    int
	probes  int
}

func NewHashTable[V any](probing Probing, capacity int, maxLoad float64) *HashTable[V] {
	return &HashTable[V]{cells: make([]cell[V], nextPrime(capacity)), probing: probing, maxLoad: maxLoad}
}

func hashDJB2(key string) uint64 { // O(длины ключа)
	h := uint64(5381)
	for i := 0; i < len(key); i++ {
		h = h*33 + uint64(key[i])
	}
	return h
}

func nextPrime(n int) int {
	if n < 3 {
		return 3
	}
	for ; ; n++ {
		prime := n%2 != 0
		for d := 3; prime && d*d <= n; d += 2 {
			prime = n%d != 0
		}
		if prime {
			return n
		}
	}
}

func (t *HashTable[V]) step(key string) int { // O(1)
	if t.probing == Linear {
		return 1
	}
	return 1 + int(hashDJB2(key)%uint64(len(t.cells)-1))
}

func (t *HashTable[V]) locate(key string) (int, bool) { // O(1) в среднем
	capacity := len(t.cells)
	index := int(hashFNV(key) % uint64(capacity))
	delta := t.step(key)
	firstDeleted := -1

	for i := 0; i < capacity; i++ {
		t.probes++
		c := &t.cells[index]
		if c.state == cellEmpty {
			if firstDeleted != -1 {
				return firstDeleted, false
			}
			return index, false
		}
		if c.state == cellBusy && c.key == key {
			return index, true
		}
		if c.state == cellDeleted && firstDeleted == -1 {
			firstDeleted = index
		}
		index = (index + delta) % capacity
	}
	return firstDeleted, false
}

func (t *HashTable[V]) resize(newCapacity int) { // O(n)
	old := t.cells
	t.cells = make([]cell[V], nextPrime(newCapacity))
	t.sz = 0
	t.used = 0
	for i := range old {
		if old[i].state == cellBusy {
			t.Insert(old[i].key, old[i].value)
		}
	}
}

func (t *HashTable[V]) Insert(key string, value V) bool { // O(1) в среднем
	if float64(t.used+1) > float64(len(t.cells))*t.maxLoad {
		t.resize(len(t.cells) * 2)
	}

	index, found := t.locate(key)
	c := &t.cells[index]
	if found {
		c.value = value
		return false
	}

	if c.state == cellEmpty {
		t.used++
	}
	c.key = key
	c.value = value
	c.state = cellBusy
	t.sz++
	return true
}

func (t *HashTable[V]) Find(key string) *V { // O(1) в среднем
	index, found := t.locate(key)
	if !found {
		return nil
	}
	return &t.cells[index].value
}

func (t *HashTable[V]) Remove(key string) bool { // O(1) в среднем
	index, found := t.locate(key)
	if !found {
		return false
	}

	var zero V
	t.cells[index].state = cellDeleted
	t.cells[index].value = zero
	t.sz--
	return true
}

func (t *HashTable[V]) Size() int { // O(1)
	return t.sz
}

func (t *HashTable[V]) Capacity() int { // O(1)
	return len(t.cells)
}

func (t *HashTable[V]) LoadFactor() float64 { // O(1)
	return float64(t.sz) / float64(len(t.cells))
}

func (t *HashTable[V]) Probes() int {
	return t.probes
}

func (t *HashTable[V]) ResetProbes() {
	t.probes = 0
}

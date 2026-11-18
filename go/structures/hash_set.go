package structures

import "strings"

const initialBuckets = 8

// HashSet — множество на хеш-таблице методом цепочек; корзины — односвязные списки из ЛР1.
type HashSet struct {
	buckets []ForwardList
	sz      int
}

func NewHashSet(values ...string) *HashSet {
	s := &HashSet{buckets: make([]ForwardList, initialBuckets)}
	for _, value := range values {
		s.Add(value)
	}
	return s
}

func hashFNV(value string) uint64 { // O(длины строки)
	h := uint64(14695981039346656037)
	for i := 0; i < len(value); i++ {
		h ^= uint64(value[i])
		h *= 1099511628211
	}
	return h
}

func (s *HashSet) bucketOf(value string) int { // O(1)
	return int(hashFNV(value) % uint64(len(s.buckets)))
}

func (s *HashSet) rehash(newCount int) { // O(n)
	newBuckets := make([]ForwardList, newCount)
	for i := range s.buckets {
		for !s.buckets[i].Empty() {
			value, _ := s.buckets[i].Front()
			s.buckets[i].PopFront()
			newBuckets[hashFNV(value)%uint64(newCount)].PushFront(value)
		}
	}
	s.buckets = newBuckets
}

func (s *HashSet) Add(value string) bool { // O(1) в среднем
	bucket := &s.buckets[s.bucketOf(value)]
	if bucket.Find(value) {
		return false
	}

	bucket.PushFront(value)
	s.sz++
	if s.sz*4 > len(s.buckets)*3 {
		s.rehash(len(s.buckets) * 2)
	}
	return true
}

func (s *HashSet) Remove(value string) bool { // O(1) в среднем
	bucket := &s.buckets[s.bucketOf(value)]
	if !bucket.Find(value) {
		return false
	}

	bucket.RemoveValue(value)
	s.sz--
	return true
}

func (s *HashSet) Contains(value string) bool { // O(1) в среднем
	return s.buckets[s.bucketOf(value)].Find(value)
}

func (s *HashSet) Size() int { // O(1)
	return s.sz
}

func (s *HashSet) Empty() bool { // O(1)
	return s.sz == 0
}

func (s *HashSet) Clear() { // O(1) — старые корзины соберёт сборщик мусора
	s.buckets = make([]ForwardList, initialBuckets)
	s.sz = 0
}

func (s *HashSet) Type() byte {
	return 'H'
}

// each обходит элементы через копию списка: PopFront у копии не трогает узлы оригинала.
func (s *HashSet) each(visit func(value string)) { // O(n)
	for i := range s.buckets {
		bucket := s.buckets[i]
		for !bucket.Empty() {
			value, _ := bucket.Front()
			bucket.PopFront()
			visit(value)
		}
	}
}

func (s *HashSet) Serialize() string { // O(n)
	var b strings.Builder
	s.each(func(value string) {
		if b.Len() > 0 {
			b.WriteByte(' ')
		}
		b.WriteString(EscapeValue(value))
	})
	return b.String()
}

func (s *HashSet) Deserialize(line string) { // O(n)
	s.Clear()
	for _, token := range SplitFields(line) {
		s.Add(UnescapeValue(token))
	}
}

func (s *HashSet) String() string { // O(n)
	var b strings.Builder
	b.WriteByte('{')
	first := true
	s.each(func(value string) {
		if !first {
			b.WriteString(", ")
		}
		b.WriteString(value)
		first = false
	})
	b.WriteByte('}')
	return b.String()
}

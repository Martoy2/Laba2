package structures

import (
	"errors"
	"strings"
)

var errIndex = errors.New("Array: index out of range")

// Array — динамический массив. data — выделенная память (её длина = ёмкость), sz — сколько элементов занято.
type Array struct {
	data []string
	sz   int
}

func NewArray(values ...string) *Array {
	a := &Array{}
	a.reserve(len(values))
	for _, value := range values {
		a.data[a.sz] = value
		a.sz++
	}
	return a
}

func NewArrayN(n int, value string) *Array {
	a := &Array{}
	a.reserve(n)
	for i := 0; i < n; i++ {
		a.data[i] = value
	}
	a.sz = n
	return a
}

func (a *Array) reserve(newCap int) { // O(n) - перенос всех элементов
	if newCap <= len(a.data) {
		return
	}

	newData := make([]string, newCap)
	for i := 0; i < a.sz; i++ {
		newData[i] = a.data[i]
	}
	a.data = newData
}

func (a *Array) grow() { // O(1) амортиз., O(n) при расширении
	if a.sz < len(a.data) {
		return
	}
	if len(a.data) == 0 {
		a.reserve(4)
	} else {
		a.reserve(len(a.data) * 2)
	}
}

func (a *Array) checkIndex(index int) error { // O(1)
	if index < 0 || index >= a.sz {
		return errIndex
	}
	return nil
}

func (a *Array) PushBack(value string) { // O(1) амортиз., O(n) при расширении
	a.grow()
	a.data[a.sz] = value
	a.sz++
}

func (a *Array) Insert(index int, value string) error { // O(n) - сдвиг вправо
	if index < 0 || index > a.sz {
		return errIndex
	}

	a.grow()
	for i := a.sz; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = value
	a.sz++
	return nil
}

func (a *Array) Get(index int) (string, error) { // O(1)
	if err := a.checkIndex(index); err != nil {
		return "", err
	}

	return a.data[index], nil
}

func (a *Array) Remove(index int) error { // O(n) - сдвиг влево
	if err := a.checkIndex(index); err != nil {
		return err
	}

	for i := index; i+1 < a.sz; i++ {
		a.data[i] = a.data[i+1]
	}
	a.sz--
	a.data[a.sz] = ""
	return nil
}

func (a *Array) Set(index int, value string) error { // O(1)
	if err := a.checkIndex(index); err != nil {
		return err
	}

	a.data[index] = value
	return nil
}

func (a *Array) Size() int { // O(1)
	return a.sz
}

func (a *Array) Cap() int { // O(1)
	return len(a.data)
}

func (a *Array) Clear() { // O(n)
	for i := 0; i < a.sz; i++ {
		a.data[i] = ""
	}
	a.sz = 0
}

func (a *Array) Type() byte {
	return 'M'
}

func (a *Array) Serialize() string { // O(n)
	var b strings.Builder
	for i := 0; i < a.sz; i++ {
		if i > 0 {
			b.WriteByte(' ')
		}
		b.WriteString(EscapeValue(a.data[i]))
	}
	return b.String()
}

func (a *Array) Deserialize(line string) { // O(n)
	a.Clear()
	for _, token := range SplitFields(line) {
		a.PushBack(UnescapeValue(token))
	}
}

func (a *Array) String() string { // O(n)
	var b strings.Builder
	b.WriteByte('[')
	for i := 0; i < a.sz; i++ {
		if i > 0 {
			b.WriteString(", ")
		}
		b.WriteString(a.data[i])
	}
	b.WriteByte(']')
	return b.String()
}

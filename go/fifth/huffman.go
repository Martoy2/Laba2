package fifth

import (
	"errors"
	"fmt"
	"io"
	"strings"

	"dbms/structures"
)

type node struct {
	symbol string
	weight int
	left   *node
	right  *node
}

// Huffman строит дерево по частотам символов и хранит таблицу кодов.
type Huffman struct {
	root        *node
	symbols     *structures.Array
	frequencies *structures.Array
	codes       *structures.HashTable[string]
}

func NewHuffman(text string) *Huffman {
	h := &Huffman{
		symbols:     structures.NewArray(),
		frequencies: structures.NewArray(),
		codes:       structures.NewHashTable[string](structures.Double, 11, 0.7),
	}

	all := structures.SplitSymbols(text)
	counts := structures.NewHashTable[int](structures.Double, 11, 0.7)
	for i := 0; i < all.Size(); i++ {
		symbol, _ := all.Get(i)
		if count := counts.Find(symbol); count != nil {
			*count++
		} else {
			counts.Insert(symbol, 1)
			h.symbols.PushBack(symbol)
		}
	}

	for i := 1; i < h.symbols.Size(); i++ {
		current, _ := h.symbols.Get(i)
		weight := *counts.Find(current)
		j := i
		for j > 0 {
			prev, _ := h.symbols.Get(j - 1)
			if *counts.Find(prev) >= weight {
				break
			}
			h.symbols.Set(j, prev)
			j--
		}
		h.symbols.Set(j, current)
	}

	count := h.symbols.Size()
	if count == 0 {
		return h
	}

	pool := make([]*node, 0, count)
	for i := 0; i < count; i++ {
		symbol, _ := h.symbols.Get(i)
		weight := *counts.Find(symbol)
		pool = append(pool, &node{symbol: symbol, weight: weight})
		h.frequencies.PushBack(fmt.Sprint(weight))
	}

	for len(pool) > 1 {
		var left, right *node
		left, pool = takeMin(pool)
		right, pool = takeMin(pool)
		pool = append(pool, &node{symbol: left.symbol + right.symbol, weight: left.weight + right.weight, left: left, right: right})
	}
	h.root = pool[0]

	if h.root.left == nil {
		h.codes.Insert(h.root.symbol, "0")
	} else {
		h.assignCodes(h.root, "")
	}
	return h
}

func takeMin(pool []*node) (*node, []*node) { // O(k)
	best := len(pool) - 1
	for i := len(pool) - 2; i >= 0; i-- {
		if pool[i].weight < pool[best].weight {
			best = i
		}
	}

	n := pool[best]
	copy(pool[best:], pool[best+1:])
	return n, pool[:len(pool)-1]
}

func (h *Huffman) assignCodes(n *node, code string) { // O(k)
	if n.left == nil {
		h.codes.Insert(n.symbol, code)
		return
	}

	h.assignCodes(n.left, code+"0")
	h.assignCodes(n.right, code+"1")
}

func (h *Huffman) Encode(text string) (string, error) { // O(n * длина кода)
	all := structures.SplitSymbols(text)
	var bits strings.Builder
	for i := 0; i < all.Size(); i++ {
		symbol, _ := all.Get(i)
		code := h.codes.Find(symbol)
		if code == nil {
			return "", errors.New("symbol not in table: " + symbol)
		}
		bits.WriteString(*code)
	}
	return bits.String(), nil
}

func (h *Huffman) Decode(bits string) (string, error) { // O(длины кода)
	var text strings.Builder
	if h.root == nil {
		return "", nil
	}

	n := h.root
	for i := 0; i < len(bits); i++ {
		if bits[i] != '0' && bits[i] != '1' {
			return "", errors.New("bad bit in code")
		}
		if h.root.left != nil {
			if bits[i] == '0' {
				n = n.left
			} else {
				n = n.right
			}
		}
		if n.left == nil {
			text.WriteString(n.symbol)
			n = h.root
		}
	}
	return text.String(), nil
}

func (h *Huffman) PrintTable(w io.Writer) { // O(k)
	for i := 0; i < h.symbols.Size(); i++ {
		symbol, _ := h.symbols.Get(i)
		frequency, _ := h.frequencies.Get(i)
		fmt.Fprintf(w, "'%s' %s %s\n", symbol, frequency, *h.codes.Find(symbol))
	}
}

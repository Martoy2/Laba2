package fourth

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"dbms/structures"
)

const maxElements = 20

func keyOf(elements *structures.Array, chosen []int) string { // O(k^2)
	sorted := structures.NewArray()
	for _, index := range chosen {
		value, _ := elements.Get(index)
		pos := sorted.Size()
		for pos > 0 {
			prev, _ := sorted.Get(pos - 1)
			if prev <= value {
				break
			}
			pos--
		}
		sorted.Insert(pos, value)
	}

	var key strings.Builder
	for i := 0; i < sorted.Size(); i++ {
		value, _ := sorted.Get(i)
		key.WriteString(structures.EscapeValue(value) + " ")
	}
	return key.String()
}

func combinations(out *strings.Builder, elements *structures.Array, chosen []int, size, start int, seen *structures.HashSet, first *bool) { // O(n * 2^n) для всех размеров
	if len(chosen) == size {
		if !seen.Add(keyOf(elements, chosen)) {
			return
		}

		if !*first {
			out.WriteString(", ")
		}
		out.WriteByte('{')
		for i, index := range chosen {
			if i > 0 {
				out.WriteString(", ")
			}
			value, _ := elements.Get(index)
			out.WriteString(value)
		}
		out.WriteByte('}')
		*first = false
		return
	}

	for i := start; i < elements.Size(); i++ {
		combinations(out, elements, append(chosen, i), size, i+1, seen, first)
	}
}

func Run() int {
	line, _ := bufio.NewReader(os.Stdin).ReadString('\n')
	elements := structures.NewArray()
	for _, token := range structures.SplitFields(line) {
		elements.PushBack(token)
	}

	if elements.Size() > maxElements {
		fmt.Fprintf(os.Stderr, "error: too many elements (max %d)\n", maxElements)
		return 1
	}

	var out strings.Builder
	seen := structures.NewHashSet()
	first := true
	out.WriteByte('[')
	for size := 0; size <= elements.Size(); size++ {
		combinations(&out, elements, make([]int, 0, elements.Size()), size, 0, seen, &first)
	}
	out.WriteByte(']')
	fmt.Println(out.String())
	return 0
}

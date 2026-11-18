package structures

// SplitSymbols режет строку на символы UTF-8, а не на байты: русская буква — один символ.
func SplitSymbols(text string) *Array { // O(n)
	symbols := NewArray()
	for _, r := range text {
		symbols.PushBack(string(r))
	}
	return symbols
}

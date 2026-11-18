package fifth

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func Run() int {
	line, _ := bufio.NewReader(os.Stdin).ReadString('\n')
	text := strings.TrimRight(line, "\r\n")
	if text == "" {
		fmt.Fprintln(os.Stderr, "error: empty string")
		return 1
	}

	huffman := NewHuffman(text)
	bits, err := huffman.Encode(text)
	if err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		return 1
	}
	decoded, err := huffman.Decode(bits)
	if err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		return 1
	}

	fmt.Println("Code: " + bits)
	fmt.Println("Table (symbol, frequency, code):")
	huffman.PrintTable(os.Stdout)
	fmt.Println("Decoded: " + decoded)
	fmt.Printf("Bits: %d instead of %d\n", len(bits), len(text)*8)

	if decoded != text {
		return 1
	}
	return 0
}

package first

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func Run() int {
	line, _ := bufio.NewReader(os.Stdin).ReadString('\n')
	text := strings.TrimRight(line, "\r\n")

	if result, ok := RepairXML(text); ok {
		fmt.Println(result)
	} else {
		fmt.Println("Incorrect")
	}
	return 0
}

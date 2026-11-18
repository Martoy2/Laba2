package main

import (
	"fmt"
	"os"
	"strconv"

	"dbms/fifth"
	"dbms/first"
	"dbms/fourth"
	"dbms/second"
	"dbms/seventh"
	"dbms/sixth"
	"dbms/third"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s <task number> [args...]\n", os.Args[0])
		os.Exit(1)
	}

	task, _ := strconv.Atoi(os.Args[1])
	code := 0
	switch task {
	case 1:
		code = first.Run()
	case 2:
		code = second.Run(os.Args)
	case 3:
		code = third.Run()
	case 4:
		code = fourth.Run()
	case 5:
		code = fifth.Run()
	case 6:
		code = sixth.Run(os.Args)
	case 7:
		code = seventh.Run()
	default:
		fmt.Fprintln(os.Stderr, "unknown task:", os.Args[1])
		code = 1
	}
	os.Exit(code)
}

package second

import (
	"fmt"
	"os"

	"dbms/engine"
)

type args struct {
	file  string
	query string
}

func parseArgs(argv []string) (args, bool) {
	var a args
	for i := 2; i < len(argv); i++ {
		switch {
		case argv[i] == "--file" && i+1 < len(argv):
			i++
			a.file = argv[i]
		case argv[i] == "--query" && i+1 < len(argv):
			i++
			a.query = argv[i]
		default:
			return a, false
		}
	}
	return a, a.file != "" && a.query != ""
}

func Run(argv []string) int {
	a, ok := parseArgs(argv)
	if !ok {
		fmt.Fprintf(os.Stderr, "Usage: %s 2 --file <path> --query '<command>'\n", argv[0])
		return 1
	}

	var storage engine.Storage
	if err := storage.Load(a.file); err != nil {
		fmt.Fprintf(os.Stderr, "error: cannot load %s: %v\n", a.file, err)
		return 1
	}

	tokens, err := engine.Tokenize(a.query)
	if err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		return 1
	}

	result, changed, err := engine.Execute(&storage, tokens)
	if err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		return 1
	}
	if changed {
		if err := storage.Save(a.file); err != nil {
			fmt.Fprintln(os.Stderr, "error:", err)
			return 1
		}
	}
	fmt.Println("-> " + result)
	return 0
}

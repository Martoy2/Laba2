package sixth

import (
	"bufio"
	"fmt"
	"math"
	"math/rand"
	"os"
	"strings"
	"time"

	"dbms/structures"
)

const (
	tableSize   = 10007
	missQueries = 10000
)

type measure struct {
	hitProbes   float64
	missProbes  float64
	nsPerInsert float64
}

func measureTable(probing structures.Probing, alpha float64, keys, misses *structures.Array) measure {
	table := structures.NewHashTable[int](probing, tableSize, 1.0)
	n := int(alpha * tableSize)

	start := time.Now()
	for i := 0; i < n; i++ {
		key, _ := keys.Get(i)
		table.Insert(key, i)
	}
	elapsed := time.Since(start)

	table.ResetProbes()
	for i := 0; i < n; i++ {
		key, _ := keys.Get(i)
		table.Find(key)
	}
	hit := float64(table.Probes()) / float64(n)

	table.ResetProbes()
	for i := 0; i < misses.Size(); i++ {
		key, _ := misses.Get(i)
		table.Find(key)
	}
	miss := float64(table.Probes()) / float64(misses.Size())

	return measure{hit, miss, float64(elapsed.Nanoseconds()) / float64(n)}
}

func analyze() int {
	rng := rand.New(rand.NewSource(2026))
	keys := structures.NewArray()
	misses := structures.NewArray()
	for i := 0; i < tableSize; i++ {
		keys.PushBack(fmt.Sprintf("k%d", rng.Uint64()))
	}
	for i := 0; i < missQueries; i++ {
		misses.PushBack(fmt.Sprintf("m%d", rng.Uint64()))
	}

	fmt.Printf("Table size %d, average probes per search (theory in brackets)\n", tableSize)
	fmt.Println("alpha |      linear: hit        miss   ns/ins |      double: hit        miss   ns/ins")

	for _, alpha := range []float64{0.1, 0.25, 0.5, 0.7, 0.8, 0.9, 0.95} {
		linear := measureTable(structures.Linear, alpha, keys, misses)
		dbl := measureTable(structures.Double, alpha, keys, misses)

		linearHit := 0.5 * (1 + 1/(1-alpha))
		linearMiss := 0.5 * (1 + 1/((1-alpha)*(1-alpha)))
		doubleHit := math.Log(1/(1-alpha)) / alpha
		doubleMiss := 1 / (1 - alpha)

		fmt.Printf("%5.2f | %5.2f (%5.2f) %6.2f (%6.2f) %6.0f | %5.2f (%5.2f) %6.2f (%6.2f) %6.0f\n",
			alpha, linear.hitProbes, linearHit, linear.missProbes, linearMiss, linear.nsPerInsert,
			dbl.hitProbes, doubleHit, dbl.missProbes, doubleMiss, dbl.nsPerInsert)
	}
	return 0
}

func longestSubstring() int { // O(n)
	line, _ := bufio.NewReader(os.Stdin).ReadString('\n')
	symbols := structures.SplitSymbols(strings.TrimRight(line, "\r\n"))
	last := structures.NewHashTable[int](structures.Double, 11, 0.7)
	left, best, bestStart := 0, 0, 0

	for i := 0; i < symbols.Size(); i++ {
		symbol, _ := symbols.Get(i)
		if seen := last.Find(symbol); seen != nil && *seen >= left {
			left = *seen + 1
		}
		last.Insert(symbol, i)

		if i-left+1 > best {
			best = i - left + 1
			bestStart = left
		}
	}

	var substring strings.Builder
	for i := bestStart; i < bestStart+best; i++ {
		symbol, _ := symbols.Get(i)
		substring.WriteString(symbol)
	}
	fmt.Printf("%d (%s)\n", best, substring.String())
	return 0
}

func Run(argv []string) int {
	mode := ""
	if len(argv) > 2 {
		mode = argv[2]
	}
	switch mode {
	case "1":
		return analyze()
	case "2":
		return longestSubstring()
	}

	fmt.Fprintf(os.Stderr, "Usage: %s 6 1   (analysis)  |  %s 6 2   (longest substring)\n", argv[0], argv[0])
	return 1
}

package third

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"

	"dbms/structures"
)

func parseNatural(token string) (int64, bool) {
	if token == "" || len(token) > 15 {
		return 0, false
	}

	var value int64
	for i := 0; i < len(token); i++ {
		if token[i] < '0' || token[i] > '9' {
			return 0, false
		}
		value = value*10 + int64(token[i]-'0')
	}
	return value, value > 0
}

func place(nums []int64, order, group []int, sums []int64, target int64, i int) bool { // O(k^n) в худшем случае
	if i == len(nums) {
		return true
	}

	current := order[i]
	for b := range sums {
		if sums[b]+nums[current] > target {
			continue
		}

		sums[b] += nums[current]
		group[current] = b
		if place(nums, order, group, sums, target, i+1) {
			return true
		}
		sums[b] -= nums[current]

		if sums[b] == 0 {
			break
		}
	}
	return false
}

func Run() int {
	reader := bufio.NewReader(os.Stdin)
	line, _ := reader.ReadString('\n')

	set := structures.NewHashSet()
	elements := structures.NewArray()
	for _, token := range structures.SplitFields(line) {
		value, ok := parseNatural(token)
		if !ok {
			fmt.Fprintln(os.Stderr, "error: not a natural number:", token)
			return 1
		}
		text := strconv.FormatInt(value, 10)
		if set.Add(text) {
			elements.PushBack(text)
		}
	}

	line, _ = reader.ReadString('\n')
	fields := structures.SplitFields(line)
	var target int64
	ok := len(fields) > 0
	if ok {
		target, ok = parseNatural(fields[0])
	}
	if !ok {
		fmt.Fprintln(os.Stderr, "error: expected subset sum")
		return 1
	}

	n := elements.Size()
	if n == 0 {
		fmt.Fprintln(os.Stderr, "error: empty set")
		return 1
	}

	nums := make([]int64, n)
	order := make([]int, n)
	group := make([]int, n)
	var total, maxValue int64
	for i := 0; i < n; i++ {
		text, _ := elements.Get(i)
		nums[i], _ = parseNatural(text)
		order[i] = i
		total += nums[i]
		if nums[i] > maxValue {
			maxValue = nums[i]
		}
	}

	for i := 1; i < n; i++ {
		current := order[i]
		j := i - 1
		for j >= 0 && nums[order[j]] < nums[current] {
			order[j+1] = order[j]
			j--
		}
		order[j+1] = current
	}

	ok = total%target == 0 && maxValue <= target
	k := 0
	if ok {
		k = int(total / target)
		ok = place(nums, order, group, make([]int64, k), target, 0)
	}

	if !ok {
		fmt.Println("Cannot split into subsets with sum", target)
		return 0
	}

	fmt.Printf("Sum %d, subsets: %d\n", target, k)
	for b := 0; b < k; b++ {
		var parts []string
		for i := 0; i < n; i++ {
			if group[i] == b {
				parts = append(parts, strconv.FormatInt(nums[i], 10))
			}
		}
		fmt.Println("{" + strings.Join(parts, ", ") + "}")
	}
	return 0
}

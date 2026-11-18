package first

import "dbms/structures"

func isNameChar(c byte) bool {
	return c >= 'a' && c <= 'z'
}

func IsValidXML(text string) bool { // O(n)
	tags := structures.NewStack()
	i := 0

	for i < len(text) {
		if text[i] == ' ' {
			i++
			continue
		}
		if text[i] != '<' {
			return false
		}
		i++

		closing := false
		if i < len(text) && text[i] == '/' {
			closing = true
			i++
		}

		start := i
		for i < len(text) && isNameChar(text[i]) {
			i++
		}
		if i == start || i == len(text) || text[i] != '>' {
			return false
		}
		name := text[start:i]
		i++

		if !closing {
			tags.Push(name)
		} else if top, err := tags.Pop(); err != nil || top != name {
			return false
		}
	}

	return tags.Empty()
}

func RepairXML(text string) (string, bool) { // O(29 * n^2)
	if IsValidXML(text) {
		return text, true
	}

	const alphabet = "abcdefghijklmnopqrstuvwxyz</>"
	candidate := []byte(text)

	for i := len(text) - 1; i >= 0; i-- {
		if text[i] == ' ' {
			continue
		}

		for j := 0; j < len(alphabet); j++ {
			if alphabet[j] == text[i] {
				continue
			}

			candidate[i] = alphabet[j]
			if IsValidXML(string(candidate)) {
				return string(candidate), true
			}
		}
		candidate[i] = text[i]
	}

	return "", false
}

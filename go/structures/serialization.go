package structures

import "strings"

func EscapeValue(value string) string {
	if value == "" {
		return `\e`
	}

	var b strings.Builder
	for i := 0; i < len(value); i++ {
		switch c := value[i]; c {
		case ' ':
			b.WriteString(`\s`)
		case '\t':
			b.WriteString(`\t`)
		case '\n':
			b.WriteString(`\n`)
		case '\r':
			b.WriteString(`\r`)
		case '\\':
			b.WriteString(`\\`)
		default:
			b.WriteByte(c)
		}
	}
	return b.String()
}

func UnescapeValue(token string) string {
	if token == `\e` {
		return ""
	}

	var b strings.Builder
	for i := 0; i < len(token); i++ {
		if token[i] != '\\' || i+1 == len(token) {
			b.WriteByte(token[i])
			continue
		}

		i++
		switch token[i] {
		case 's':
			b.WriteByte(' ')
		case 't':
			b.WriteByte('\t')
		case 'n':
			b.WriteByte('\n')
		case 'r':
			b.WriteByte('\r')
		default:
			b.WriteByte(token[i])
		}
	}
	return b.String()
}

func IsSpace(c byte) bool {
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
}

// SplitFields делит строку по пробельным символам ASCII, как оператор >> в C++.
func SplitFields(line string) []string {
	return strings.FieldsFunc(line, func(r rune) bool {
		return r < 128 && IsSpace(byte(r))
	})
}

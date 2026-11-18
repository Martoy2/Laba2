package structures

// Container — общий интерфейс всех структур, через него работает хранилище.
type Container interface {
	Type() byte
	Serialize() string
	Deserialize(line string)
	String() string
}

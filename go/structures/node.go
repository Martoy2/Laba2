package structures

type Node struct {
	Value string
	Next  *Node
}

type DNode struct {
	Value string
	Prev  *DNode
	Next  *DNode
}

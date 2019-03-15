# I don't trust this stupid lists of python >:(
#

class Node:
    def __init__(self, value, leftBro, rightBro):
        self.value = value
        self.leftBro = leftBro
        self.rightBro = rightBro


class Stack:
    # stack will be just a doubled linked list, push after last, and pop first...

    def __init__(self):
        self.firstNode = None
        self.lastNode = None

    def push(self, value):
        new_node = Node(value, None, self.firstNode)
        if self.firstNode is None:
            self.firstNode = new_node
            self.lastNode = new_node
        else:  # else, firstNode exists. push before first
            second = self.firstNode
            self.firstNode = new_node
            second.leftBro = self.firstNode

        return new_node  # so hash can keep the node

    def pop(self):
        if self.firstNode is None:
            print("Error: pop( ) empty stack")
            return None
        popped_value = self.firstNode.value
        self.firstNode = self.firstNode.rightBro  # update first
        if self.firstNode is not None:
            self.firstNode.leftBro = None  # his left should now be None
        return popped_value

    # low means, he gives a reference. be careful of that!
    def peek_low(self):
        if self.firstNode is None:
            return None
        # he just wants to see the first node, he doesn't want to pop him :)
        return self.firstNode.value

    def isEmpty(self):
        return self.firstNode is None

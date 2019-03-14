import math


"""
Fibonacci Heap.
We're only considering that we have to support insert and extract_min

structure:
(NOT circular) doubly linked list of binomial trees.
circular doubly linked for all brothers in binomial tree
*binomial tree is a type of binary tree (look in wiki)

we're only going to implement this queries:

insert: O(1) real & amortized
just add it to roots-list as a new tree

extract_min: O(logn) amortized
1. delete min
2. add his sons-list to roots-list
3. squash together roots with the same degree until there's no two trees with the same degree:
*degree of a tree is the degree of his root
**max degree is logn because we're only squashing trees with equal size
create pointer array (with size logn) that at index i points to a tree of degree i
start setting this pointer array and when new tree wants to point to existing cell, squash this two trees together

decreaseValue: O(1) amortized
1. improve key
2. iterative cutoff:
cutoff node
cutoff father if he's marked (and then unmark him)
if we cut the father, cutoff father of father if he's unmarked
and so on... until some parent is unmarked, then mark him and finish
"""


class BinaryTreeNode:
    def __init__(self, value):
        self.value = value
        self.marked = False
        self.father = None  # we need parent reference, for "decreaseValue( )" function
        self.leftBro = None  # can be used as a circular linked list, but for root-list it's not!
        self.rightBro = None  # can be used as a circular linked list, but for root-list it's not!
        self.leftSon = None  # leftest son
        self.degree = 0  # number of sons

    def __lt__(self, other):
        return self.value < other.value

class FibonacciHeap:

    def __init__(self):
        self.min_node = None  # empty linked list
        self.size = 0

    # insert this value, and return his node (we need his pointer)
    def insert(self, value):
        self.size += 1

        new_node = BinaryTreeNode(value)  # creates new tree just for him

        if self.min_node is None:
            self.min_node = new_node
            return new_node  # return created node

        # insert new_node between min_node and his leftBro
        # ... <--> |_left_to_min_| <--> |_min_| <--> ...
        #                           ^
        #                           | insert here

        left_of_min_node = self.min_node.leftBro

        new_node.rightBro = self.min_node
        new_node.leftBro = left_of_min_node

        self.min_node.leftBro = new_node
        if left_of_min_node is not None:
            left_of_min_node.rightBro = new_node

        # update the min node, if needed
        if value < self.min_node.value:
            self.min_node = new_node

        return new_node  # return created node

    # extract the min node, and return only his value
    def extract_min(self):
        if self.min_node is None:
            return None
        # self.min_node is the only node in the heap
        if self.min_node.leftSon is None and self.min_node.leftBro is None and self.min_node.rightBro is None:
            min_node_value = self.min_node.value
            self.min_node = None
            self.size -= 1
            return min_node_value

        # 1. extract min_node, and insert his sons to root-list
        min_node_value = self.min_node.value  # self.min_node will be extracted in the end

        most_left_sons_list = self.min_node.leftSon
        if most_left_sons_list is not None:
            most_right_sons_list = most_left_sons_list.leftBro  # because it's circular linked list
        else:
            most_right_sons_list = None

        left_of_min_node = self.min_node.leftBro
        right_of_min_node = self.min_node.rightBro

        # update sons' father to be None (they're now roots!)
        if most_left_sons_list is not None:  # there's at least one son
            son = most_left_sons_list
            while son is not most_right_sons_list:
                son.father = None
                son = son.rightBro
            son.father = None  # also update for most_right_son

        # insert sons-list to root list between left_of_min & right_of_min
        if most_left_sons_list is not None: # sons-list is not empty
            most_left_sons_list.leftBro = left_of_min_node
            most_right_sons_list.rightBro = right_of_min_node

        if left_of_min_node is not None:
            if most_left_sons_list is not None:
                left_of_min_node.rightBro = most_left_sons_list
            else: # there's no sons to be added
                left_of_min_node.rightBro = right_of_min_node
        if right_of_min_node is not None:
            if most_right_sons_list is not None:
                right_of_min_node.leftBro = most_right_sons_list
            else:  # there's no sons to be added
                right_of_min_node.leftBro = left_of_min_node

        # find someone to switch the min_node that was extracted
        if left_of_min_node is not None:
            self.min_node = left_of_min_node
        elif right_of_min_node is not None:
            self.min_node = right_of_min_node
        elif most_left_sons_list is not None:
            self.min_node = most_left_sons_list

        # 2. now squash roots-list
        array_pointers = [None] * (2*int(math.log2(self.size)))  # logn is enough, but just to make sure

        current_root = self.min_node
        while current_root.leftBro is not None:  # current_root should start as the most lefty
            current_root = current_root.leftBro

        while current_root is not None:
            if array_pointers[current_root.degree] is None:  # first tree with this degree
                array_pointers[current_root.degree] = current_root  # add pointer to this tree
                current_root = current_root.rightBro  # next tree in root-list
            else:
                # merge them. (the bigger root put as the son of the smaller root)
                # remove links of the tree that was put as the son of the other
                # update array_pointers and move to the next tree

                old_degree = current_root.degree  # save it, in case we change it later
                next_tree = None  # next tree may change later

                # merge them!
                old_tree = array_pointers[current_root.degree]
                if old_tree.value < current_root.value:
                    # current_root must be added as a son!
                    # current_root will be added to some other tree. therefor, next tree should be as usual his right
                    next_tree = old_tree
                    smaller_tree = old_tree
                    bigger_tree = current_root
                else:
                    # old_tree mmust be added as a son!
                    # current_root will be changed, therefor, check him again
                    next_tree = current_root
                    smaller_tree = current_root
                    bigger_tree = old_tree

                # I want to put later on the smaller_tree in current's position
                right_of_current = current_root.rightBro
                left_of_current = current_root.leftBro
                if left_of_current is old_tree:
                    left_of_current = left_of_current.leftBro

                # remove bigger_tree from root-list
                if bigger_tree.leftBro is not None:
                    bigger_tree.leftBro.rightBro = bigger_tree.rightBro
                if bigger_tree.rightBro is not None:
                    bigger_tree.rightBro.leftBro = bigger_tree.leftBro

                # add bigger_tree to smaller_tree's sons
                if smaller_tree.leftSon is None:  # he'll be only son
                    smaller_tree.leftSon = bigger_tree
                    bigger_tree.father = smaller_tree
                    bigger_tree.rightBro = bigger_tree  # circular
                    bigger_tree.leftBro = bigger_tree  # circular
                else:
                    smallers_leftson = smaller_tree.leftSon
                    bigger_tree.father = smaller_tree
                    # push bigger between smallers_leftson and his lefty
                    smallers_leftson.leftBro.rightBro = bigger_tree
                    bigger_tree.leftBro = smallers_leftson.leftBro
                    smallers_leftson.leftBro = bigger_tree
                    bigger_tree.rightBro = smallers_leftson

                # update self.min_node (so after this squashing, min_node will still be one of the roots)
                if bigger_tree.value == self.min_node.value:
                    self.min_node = smaller_tree

                # change smaller_tree position to be at current's position
                # first remove smaller_tree from his old position
                if smaller_tree.leftBro is not None:
                    smaller_tree.leftBro.rightBro = smaller_tree.rightBro
                if smaller_tree.rightBro is not None:
                    smaller_tree.rightBro.leftBro = smaller_tree.leftBro

                # push smaller_tree into current's position
                if right_of_current is not None:
                    right_of_current.leftBro = smaller_tree
                if left_of_current is not None:
                    left_of_current.rightBro = smaller_tree
                smaller_tree.leftBro = left_of_current
                smaller_tree.rightBro = right_of_current


                smaller_tree.degree += 1  # bigger was added as a son

                array_pointers[old_degree] = None
                current_root = next_tree # go to next tree!

        # min_node is just someone from the root-list
        # find the min_node between the roots
        current_root = self.min_node
        while current_root.leftBro is not None:  # set current_root to be the leftest
            current_root = current_root.leftBro
        while current_root is not None:  # find min root
            if current_root.value < self.min_node.value:
                self.min_node = current_root
            current_root = current_root.rightBro



        self.size -= 1
        return min_node_value

    # we need reference to this old value (to his node)
    def decreaseValue(self, node, new_value):
        if node is None:
            return None

        if not new_value < node.value:
            return None

        node.value = new_value
        self.iterative_cutoff(node)

        # update self.min_node if needed
        if node.value < self.min_node.value:
            self.min_node = node

    def iterative_cutoff(self, node):  # I dont want it to be recursively, because of memory..

        if node.father is None:  # means he is already a root
            return
        # first, always curoff node.
        # thus, push node to the left of min_node
        father = node.father

        # delete node from father's sons
        if node.rightBro is node:  # means he's only child
            father.leftSon = None
        else:  # he has a brother
            node.rightBro.leftBro = node.leftBro
            node.leftBro.rightBro = node.rightBro  # it's circular, no worries about a leftBro as well
            father.leftSon = node.rightBro
        father.degree -= 1

        node.father = None  # node is now a root
        node.rightBro = self.min_node
        node.leftBro = self.min_node.leftBro
        if self.min_node.leftBro is not None:
            self.min_node.leftBro.rightBro = node
        self.min_node.leftBro = node

        # now start cutting off parents if they're marked
        current = father
        while current.marked is True and current.father is not None:

            father = current.father
            current.father = None

            # delete current from father's sons-list
            if current.rightBro is current:  # means he's only child
                father.leftSon = None
            else:
                current.rightBro.leftBro = current.leftBro
                current.leftBro.rightBro = current.rightBro
                father.leftSon = current.rightBro

            father.degree -= 1

            current.rightBro = self.min_node
            current.leftBro = self.min_node.leftBro
            if self.min_node.leftBro is not None:
                self.min_node.leftBro.rightBro = current
            self.min_node.leftBro = current

            current.marked = False  # unmark him

            current = father  # goto next father

        if current.marked is True:  # then we stopped him because he was a root. but his mark did his work
            current.marked = False
        else:
            current.marked = True



    def checkforbugs(self):
        if self.min_node is None:
            return True
        current_root = self.min_node
        while current_root.leftBro is not None:
            current_root = current_root.leftBro

        while current_root is not None:
            if current_root.rightBro is not None:
                if current_root.rightBro.leftBro is not current_root:
                    print("DEBUG: heap is weird. roots bros are not good")
                    return False
            lson = current_root.leftSon

            if lson is not None:
                if lson.father is not current_root:
                    print("DEBUG: heap is weird. sons' father is not good")
                    return False
                if lson.rightBro.leftBro is not lson:
                    print("DEBUG: heap is weird. sons' bros are not good")
                    return False

                cson = lson.rightBro
                while cson is not lson:
                    if cson is None:
                        print("DEBUG: not circular")
                    if cson.father is not current_root:
                        print("DEBUG: heap is weird. sons' father is not good")
                        return False
                    if cson.rightBro.leftBro is not cson:
                        print("DEBUG: heap is weird. sons' bros are not good")
                        return False
                    cson = cson.rightBro

            current_root = current_root.rightBro

        return True







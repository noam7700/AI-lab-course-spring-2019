
"""
IDEA: almost-full binary tree can be implemented with array:

for every node in index 'i':
left-son: 2i+1
right-son: 2i+2
parent: (i-1)/2

i is leaf iff 2*i+1 > n-1
"""

class BinaryHeap:

    def __init__(self):
        self.data = []

    def isroot(self, index):
        return index is 0

    def isleaf(self, index):
        # if 2 * index + 1 <= n - 1 then it has left leaf
        return 2 * index + 1 > len(self.data) - 1

    def hasOneSon(self, index):
        return 2 * index + 1 == len(self.data) - 1

    def hasTwoSons(self, index):
        return 2 * index + 1 < len(self.data) - 1

    # update BinaryHeap, and return new index
    def sift_up(self, index):
        """
        sifting is done by comparing with parent's value.
        if we're larger than parent or we're the root, finish
        otherwise, swap between curr & curr.parent
        """
        while (not self.isroot(index)) and self.data[index] < self.data[int((index - 1) / 2)]:  # not root and bigger than parent
            # swap between current and parent
            tmp = self.data[index]
            self.data[index] = self.data[int((index - 1) / 2)]
            self.data[int((index - 1) / 2)] = tmp
            index = int((index - 1) / 2)

        return index

    # return new index
    def sift_down(self, index):
        """
        sifting is done by comparing with sons' value (only two)
        if we're smaller than both of them or we're a leaf, finish
        otherwise, swap between curr and max son
        """
        # 3 cases: one son, two sons, and leaf
        cond = self.hasOneSon(index) and self.data[index] > self.data[2 * index + 1]
        cond = cond or (self.hasTwoSons(index) and (
                self.data[index] > self.data[2 * index + 1] or self.data[index] > self.data[2 * index + 2]))
        while cond:
            if self.hasOneSon(index):
                argmin = 2*index+1
            else:
                min_val = min(self.data[2*index + 1], self.data[2*index + 2])
                if self.data[2*index + 1] == min_val:  # if min brother is possible
                    argmin = 2*index + 1
                else:
                    argmin = 2*index + 2
            tmp = self.data[index]
            self.data[index] = self.data[argmin]
            self.data[argmin] = tmp

            index = argmin

            # 3 cases: leaf, one son, two sons
            cond = self.hasOneSon(index) and self.data[index] > self.data[2 * index + 1]
            cond = cond or (self.hasTwoSons(index) and (
                        self.data[index] > self.data[2 * index + 1] or self.data[index] > self.data[2 * index + 2]))

        return index

    def insert(self, x):
        index = len(self.data)
        self.data.append(x)

        self.sift_up(len(self.data) - 1)  # sift-up last element in heap

    def find_min(self):
        return self.data[0]

    def extract_min(self):
        # swap between root & last element
        min = self.extract_byIndex(0)

        return min

    def find(self, element):
        for index in range(len(self.data)):
            if self.data[index] == element:
                result = index
        return result

    def extract_byIndex(self, index):
        result = self.data[index]
        self.data[index] = self.data[len(self.data) - 1]
        self.data[len(self.data) - 1] = result
        # exract last element from data
        del self.data[len(self.data) - 1]

        # sift_down or up
        if index is 0 or self.data[index] <= self.data[int((index - 1)/2)]:
            if index is 4: print("DEBUG: I'm", self.data[index], "and I do sift_down")
            self.sift_down(index)  # go down as much as possible (if needed)
        else:
            if index is 4: print("DEBUG: I'm", self.data[index], "and I do sift_down")
            self.sift_up(index)  # then try to go up as much as possible

        return result

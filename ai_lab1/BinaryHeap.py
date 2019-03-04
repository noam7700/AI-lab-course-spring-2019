
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
        self.data = [];

    def isroot(self, index):
        return index is 0;

    def isleaf(self, index):
        # if 2 * index + 1 <= n - 1 then it has left leaf
        return 2 * index + 1 > len(self.data) - 1;

    def hasOneSon(self, index):
        return 2 * index + 1 == len(self.data) - 1;

    def hasTwoSons(self, index):
        return 2 * index + 1 < len(self.data) - 1;

    # update BinaryHeap, and return new index
    def sift_up(self, index):
        """"
        sifting is done by comparing with parent's value.
        if we're larger than parent or we're the root, finish
        otherwise, swap between curr & curr.parent
        """"
        while self.isroot(index) or self.data[index] >= self.data[int((index - 1) / 2)]:  # not root or bigger than parent
            # swap between current and parent
            tmp = self.data[index];
            self.data[index] = self.data[int((index - 1) / 2)];
            self.data[int((index - 1) / 2)] = tmp;
            index = int((index - 1) / 2);

        return index;

    # return new index
    def sift_down(self, index):
        """
        sifting is done by comparing with sons' value (only two)
        if we're smaller than both of them or we're a leaf, finish
        otherwise, swap between curr and max son
        """
        # 3 cases: leaf, one son, two sons
        cond = self.isleaf(index);
        cond = cond or (self.hasOneSon(index) and self.data[index] > self.data[2*index+1]);
        cond = cond or (self.data[index] > self.data[2*index+1] and self.data[index] > self.data[2*index+2]);
        while cond:
            if self.hasOneSon():
                argmax = 2*index+1;
            else:
                if self.data[2 * index + 1] > self.data[2 * index + 2]:
                    argmax = 2 * index + 1;
                else:
                    argmax = 2 * index + 2;

            tmp = self.data[index];
            self.data[index] = self.data[argmax];
            self.data[argmax] = tmp;

            index = argmax;

            # 3 cases: leaf, one son, two sons
            cond = self.isleaf(index);
            cond = cond or (self.hasOneSon(index) and self.data[index] > self.data[2 * index + 1]);
            cond = cond or (self.data[index] > self.data[2 * index + 1] and self.data[index] > self.data[2 * index + 2]);

        return index;

    def insert(self, x):
        index = len(self.data);
        self.data.append(x);

        self.sift_up(x);

    def find_min(self):
        return self.data[0];

    def extract_min(self):
        # swap between root & last element
        min = self.extract_byIndex(0);

        return min;

    def find(self, element):
        for index in range(len(self.data)):
            if self.data[index] == element:
                result = index;
        return result;

    def exract_byIndex(self, index):
        result = self.data[index];
        self.data[index] = self.data[len(self.data) - 1];
        self.data[len(self.data) - 1] = result;
        # exract last element from data
        del self.data[len(self.data) - 1];
        # sift_down the new root
        self.sift_down(index);

        return result;

# Iterative Deepening DFS
"""
sorry, I know it's code duplication of AStar. But we already did most of the report, and I dont want to change it...

and although there are a lot of similar code, there are a lot of places it differ.
for example,
- node doesn't need to save score or cost_to_root...
- and our data-structure is a little bit different (stack vs heap)
"""
from Stack import Stack
from collections import namedtuple
from time import time
import GameState

class Node:
    def __init__(self, game_state, father_node, created_move, key):
        self.game_state = game_state
        self.father_node = father_node
        self.createdMove = created_move
        self.depth = father_node.depth + 1
        self.key = key


class Stack_with_hashTable:
    def __init__(self):
        self.stack = Stack()
        self.hashTable = {}

    def push(self, value):
        self.stack.push(value)
        self.hashTable[value.key] = value

    def pop(self):
        popped_value = self.stack.pop()
        del popped_value.key

    def __contains__(self, key):
        return key in self.hashTable

    def isEmpty(self):
        return self.stack.isEmpty()


def iddfs(start_game_state, max_time_solution):
    # doesn't contain "avg_h" for obvious reasons..
    Solution = namedtuple("Solution",
                          ["solution_node", "num_searched", "permit", "exec_time", "ebf", "d"])

    if not bool(start_game_state):
        return Solution(None, 0, 0, 0, 0, 0, 0, 0, 0)

    start_time = time()


    cutoff = 0
    while time() - start_time < max_time_solution:  # loop cutoff from 1 to inf
        cutoff += 1
        solution = dls(start_game_state, cutoff)
        if solution.solution_node is not None:
            break
    # found solution! (or it got to max_time, but who cares! :P - it still returns some data...)
    return solution


# implementing dls w/o recursion, but just with stack...
def dls(start_game_state, cutoff):

    Solution = namedtuple("Solution",
                          ["solution_node", "num_searched", "permit", "exec_time", "ebf", "d"])

    start_node = Node(start_game_state, None, None, start_game_state.unique_id_str())
    open_list = Stack_with_hashTable()
    closed_list = {}

    open_list.push(start_node)

    # declare stats
    num_searched = 0
    d = cutoff  # it's always cutoff, because we would have found it before, with cutoff-1


    while not open_list.isEmpty():

        current_node = open_list.pop()

        # check if current is winning state
        if current_node.game_state.board[2][GameState.GameState.dimX - 1] is 'X':
            solution = Solution(current_node, num_searched, d/num_searched, 0, pow(num_searched, 1/d), d)
            return solution

        [sons_game_states, sons_created_move] = current_node.game_state.create_all_possible_sons()
        for i in range(len(sons_game_states)):

            # because the way dfs works, we know we shouldn't touch existing node in open.
            son_key = sons_game_states[i].unique_id_str()
            if current_node.depth < cutoff and son_key not in open_list:
                son_node = Node(sons_game_states[i], current_node, sons_created_move[i], son_key)
                open_list.push(son_node)
                num_searched += 1

    solution = Solution(None, num_searched, d / num_searched, 0, pow(num_searched, 1 / d), d)
    return solution














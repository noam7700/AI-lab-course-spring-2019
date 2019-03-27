from IDDFS import Node
from IDDFS import Stack_with_hashTable
from IDDFS import dls
from AStar import heuristic2
from collections import namedtuple
from time import time

# Iterative Deepening of A*
# iterate on dls with cutoff = new_cutoff. where new_cutoff starts as f(init_node), and each time updates to last max_f

def ida_star(start_game_state, f_func, max_time_solution):


    # N = num_searched* - sum of num_searched in every iteration
    # permit - d/N
    # exec_time ...
    # ebf - N^(1/d)
    N = 0

    Solution = namedtuple("Solution",
                          ["solution_node", "num_searched", "permit", "exec_time", "ebf", "d"])

    if not bool(start_game_state):
        return Solution(None, 0, 0, 0, 0, 0, 0, 0, 0)

    start_time = time()
    start_node = Node(start_game_state, None, None, start_game_state.unique_id_str())
    cutoff = f_func(start_node)

    while time() - start_time < max_time_solution:  # loop cutoff from 1 to inf
        [new_cutoff, solution] = dls(start_game_state, cutoff, f_func, start_time, max_time_solution)

        N += solution.num_searched

        if solution.solution_node is not None:
            break
        cutoff = new_cutoff
    # found solution! (or it got to max_time, but who cares! :P - it still returns some data...)
    return Solution(solution.solution_node, N, solution.d / N, solution.exec_time, pow(N, 1/solution.d), solution.d)

def f_func(node):
    # heuristic2 assumes node.game_state.board
    return heuristic2(node) + node.depth


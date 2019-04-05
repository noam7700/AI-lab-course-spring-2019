from IDDFS import Node
from IDDFS import Stack_with_hashTable
from IDDFS import dls
from AStar import heuristic2
from collections import namedtuple
from time import time

import GameState  # for testing
import AStar

# Iterative Deepening of A*
# iterate on dls with cutoff = new_cutoff. where new_cutoff starts as f(init_node), and each time updates to last max_f

def ida_star(start_game_state, f_func, max_time_solution):

    # as mentioned in class. we prefer much better exec-time over knowing that solution length will be
    # at most OPT + constant
    constant = 4

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
        cutoff = new_cutoff + constant  # as mentioned in class
    # found solution! (or it got to max_time, but who cares! :P - it still returns some data...)
    return Solution(solution.solution_node, N, solution.d / N, solution.exec_time, pow(N, 1/solution.d), solution.d)

def f_func(node):
    # heuristic2 assumes node.game_state.board
    return heuristic2(node) + node.depth


def runIDAStar(path_to_input, max_time):

    rh_input_file = open(path_to_input, 'r')
    solutions_file = open("./solutions.txt", 'w')

    for i in range(40):
        solutions_file.write(
            "----------------------------- Board " + str(i + 1) + ": ----------------------------------\n")
        init_state_line = rh_input_file.readline(GameState.GameState.dimX * GameState.GameState.dimY)
        rh_input_file.readline(1)  # read '\n'

        init_game_state = GameState.GameState(None, None, init_state_line)

        result2 = ida_star(init_game_state, f_func, float(max_time))

        if result2.solution_node is None:
            solutions_file.write("Heuristic - " + "\n" +
                                 "  Number of cars block the path to exit + " + "\n" +
                                 "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                 "Solution - " + "\n   " +
                                 "  FAILED" + "\n" +
                                 "Statistics - " + "\n" +
                                 "  Number of searched nodes: " + str("%.4f" % result2.num_searched) + "\n" +
                                 "  Permittivity: " + str("%.4f" % result2.permit) + "\n" +
                                 "  Searching Time: " + str("%.4f" % result2.exec_time) + "\n" +
                                 "  EBF: " + str("%.4f" % result2.ebf) + "\n" +
                                 "\n")
        else:
            moves = AStar.restore_solution_moves(result2.solution_node)

            solutions_file.write("Heuristic - " + "\n" +
                                 "  Number of cars block the path to exit + " + "\n" +
                                 "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                 "Solution - " + "\n    " +
                                 ' '.join(moves) + "\n" +
                                 "Statistics - " + "\n" +
                                 "  Number of searched nodes: " + str("%.4f" % result2.num_searched) + "\n" +
                                 "  Permittivity: " + str("%.4f" % result2.permit) + "\n" +
                                 "  Searching Time: " + str("%.4f" % result2.exec_time) + "\n" +
                                 "  EBF: " + str("%.4f" % result2.ebf) + "\n" +
                                 "\n")

    solutions_file.write("-------------------------------------------------------------------------------\n")
    solutions_file.close()
    return 0

import sys
import GameState
import AStar
from FibonacciHeap import FibonacciHeap
from BinaryHeap import BinaryHeap
import random


def main():
    path_to_rh = sys.argv[1]  # pathname for rh.txt
    max_time_solution = sys.argv[2]  # if exceeds max_time_solution mark as failed

    # used to read all problems
    rh_input_file = open(path_to_rh, 'r')
    solutions_file = open("./solutions.txt", 'w')

    # read first problem
    for i in range(40):
        solutions_file.write("----------------------------- Board " + str(i) + ": ----------------------------------\n")
        init_state_line = rh_input_file.readline(GameState.GameState.dimX * GameState.GameState.dimY)
        rh_input_file.readline(1)  # read '\n'

        init_game_state = GameState.GameState(None, None, init_state_line)

        result2 = AStar.a_star(init_game_state, AStar.heuristic2, AStar.cost_to_root)

        if result2.solution_node is None:
            solutions_file.write("Heuristic - " + "\n" +
                                 "  Number of cars block the path to exit + " + "\n" +
                                 "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                 "Solution - " + "\n   " +
                                 "  FAILED" + "\n" +
                                 "Statistics - " + "\n" +
                                 "  Permittivity: " + str("%.4f" % result2.permit) + "\n" +
                                 "  Searching Time: " + str("%.4f" % result2.exec_time) + "\n" +
                                 "  Average heuristic score: " + str("%.4f" % result2.avg_h) + "\n" +
                                 "  EBF: " + str("%.4f" % result2.ebf) + "\n" +
                                 "  Minimum depth: " + str("%.4f" % result2.min_d) + "\n" +
                                 "  Maximum depth: " + str("%.4f" % result2.max_d) + "\n" +
                                 "  Average depth: " + str("%.4f" % result2.avg_d) + "\n" +
                                 "\n")
        else:
            moves = AStar.restore_solution_moves(result2.solution_node)

            solutions_file.write("Heuristic - " + "\n" +
                                 "  Number of cars block the path to exit + " + "\n" +
                                 "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                 "Solution - " + "\n    " +
                                 ' '.join(moves) + "\n" +
                                 "Statistics - " + "\n" +
                                 "  Permittivity: " + str("%.4f" % result2.permit) + "\n" +
                                 "  Searching Time: " + str("%.4f" % result2.exec_time) + "\n" +
                                 "  Average heuristic score: " + str("%.4f" % result2.avg_h) + "\n" +
                                 "  EBF: " + str("%.4f" % result2.ebf) + "\n" +
                                 "  Minimum depth: " + str("%.4f" % result2.min_d) + "\n" +
                                 "  Maximum depth: " + str("%.4f" % result2.max_d) + "\n" +
                                 "  Average depth: " + str("%.4f" % result2.avg_d) + "\n" +
                                 "\n")

    solutions_file.write("-------------------------------------------------------------------------------\n")
    solutions_file.close()
    return 0


if __name__ == '__main__':
    main()

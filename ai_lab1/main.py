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
        initstate_line = rh_input_file.readline(GameState.GameState.dimX * GameState.GameState.dimY);
        rh_input_file.readline(1);  # read '\n'

        initGameState = GameState.GameState(None, None, initstate_line)

        final_node = AStar.a_star(initGameState, AStar.heuristic, AStar.cost_to_root)
        if final_node is None:
            solutions_file.write("FAILED")
        else:
            moves = AStar.restore_solution_moves(final_node)
            solutions_file.write(' '.join(moves) + "\n")


if __name__ == '__main__':
    main();

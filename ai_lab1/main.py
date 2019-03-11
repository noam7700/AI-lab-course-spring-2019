import sys
import GameState
import AStar
from FibonacciHeap import FibonacciHeap
from BinaryHeap import BinaryHeap
import random

def main():
    path_to_rh = sys.argv[1]  # pathname for rh.txt
    max_time_solution = sys.argv[2]  # if exceeds max_time_solution mark as failed

    if True:
        myfib = FibonacciHeap()
        for debug in range(1):  # find someone who will be stuck
            size = 20;
            for i in range(size):
                myfib.insert(random.randint(0, 200))
            for i in range(size - 10):
                print(myfib.extract_min())

            for i in range(size):
                myfib.insert(random.randint(0, 200))
            for i in range(size + 10):
                print(myfib.extract_min())
        return 0

    # used to read all problems
    rh_file = open(path_to_rh, 'r')

    # read first problem
    initstate_line = rh_file.readline(GameState.GameState.dimX * GameState.GameState.dimY);
    rh_file.readline(1);  # read '\n'

    initGameState = GameState.GameState(None, None, initstate_line)

    final_state = AStar.a_star(initGameState, AStar.heuristic, AStar.cost_to_root)
    if final_state is not None:
        solution_moves = AStar.restore_solution_moves(final_state)
        for move in solution_moves:
            print(move, end=' ')

    """ Game Loop
    while True:
        car_name = input("car name(insert 'xxx' for next question): ");
        if car_name == "xxx":
            initstate_line = rh_file.readline(GameState.GameState.dimX * GameState.GameState.dimY);
            rh_file.readline(1);  # read '\n'
            initGameState = GameState.GameState(initstate_line);
            initGameState.printBoard();
            
        else:  # continue...
            direction = input("direction: ");
            steps = input("steps: ");

            steps = int(float(steps));

            if direction == "LEFT":
                direction = Direction.LEFT;
            elif direction == 'RIGHT':
                direction = Direction.RIGHT;
            elif direction == 'UP':
                direction = Direction.UP;
            elif direction == 'DOWN':
                direction = Direction.DOWN;

            if initGameState.moveCar_byName_ifpossible(car_name, direction, steps) is 0:
                print("DEBUG: cant move that far");
            else:
                initGameState.printBoard();
    """

if __name__ == '__main__':
    main();

import sys
import GameState
import AStar
from BinaryHeap import BinaryHeap
import random

def heuristic(game_state):
    x_row = 2
    x_end_column = 0;

    # searching for X_car's end_pos because we are no longer keeping Car objects in game_state
    for j in range(GameState.GameState.dimX - 1, -1, -1):  # from dimX - 1 to 0
        if game_state.board[x_row][j] is 'X':  # found X!
            x_end_column = j
            break

    # if winning state heuristic should be 0
    if x_end_column is GameState.GameState.dimX - 1:
        return 0  # we won!

    num_of_blocking_cars = 0
    for j in range(x_end_column + 1, GameState.GameState.dimX):
        if game_state.board[x_row][j] is not '.':
            num_of_blocking_cars += 1

    return num_of_blocking_cars + 1


def main():
    path_to_rh = sys.argv[1]  # pathname for rh.txt
    max_time_solution = sys.argv[2]  # if exceeds max_time_solution mark as failed

    # used to read all problems
    rh_file = open(path_to_rh, 'r')

    # read first problem
    for i in range(3):
        initstate_line = rh_file.readline(GameState.GameState.dimX * GameState.GameState.dimY);
        rh_file.readline(1);  # read '\n'

    print("DEBUG: first line:", initstate_line)
    initGameState = GameState.GameState(None, None, initstate_line)

    final_state = AStar.a_star(initGameState, heuristic)
    if final_state is not None and False:
        solution_path = AStar.restore_solution_path(final_state)

        for i in range(len(solution_path)):
            solution_path[i].printBoard()
            print()


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

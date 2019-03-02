import sys
import GameState
from Direction import Direction

def main():
    path_to_rh = sys.argv[1];  # pathname for rh.txt
    max_time_solution = sys.argv[2]  # if exceeds max_time_solution mark as failed

    # used to read all problems
    rh_file = open(path_to_rh, 'r');

    # read first problem
    initstate_line = rh_file.readline(GameState.GameState.dimX * GameState.GameState.dimY);
    initGameState = GameState.GameState(initstate_line);

    print("Board:");
    initGameState.printBoard();

    print();
    print("first found car:", initGameState.cars[0].name);
    print();

    initGameState.moveCar_byName_ifpossible(initGameState.cars[0].name, Direction.RIGHT, 4);
    print("Board:");
    initGameState.printBoard();


if __name__ == '__main__':
    main();

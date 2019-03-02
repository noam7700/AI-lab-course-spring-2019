import sys
import GameState  # only the class

def main():
    path_to_rh = sys.argv[1];  # pathname for rh.txt
    max_time_solution = sys.argv[2]  # if exceeds max_time_solution mark as failed

    # used to read all problems
    rh_file = open(path_to_rh, 'r');

    initstate_line = rh_file.readline(GameState.GameState.dimX * GameState.GameState.dimY);  # read first problem
    initGameState = GameState.GameState(initstate_line);
    print("initGame.printBoard():");
    initGameState.printBoard();


if __name__ == '__main__':
    main();

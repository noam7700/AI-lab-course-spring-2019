import sys
import GameState
from Direction import Direction
import heapq

def main():
    path_to_rh = sys.argv[1];  # pathname for rh.txt
    max_time_solution = sys.argv[2]  # if exceeds max_time_solution mark as failed

    # used to read all problems
    rh_file = open(path_to_rh, 'r');

    # read first problem
    initstate_line = rh_file.readline(GameState.GameState.dimX * GameState.GameState.dimY);
    rh_file.readline(1);  # read '\n'
    print("DEBUG: first line:", initstate_line);
    initGameState = GameState.GameState(None, initstate_line);

    print("Board:");
    initGameState.printBoard();
    print();

    sons = initGameState.createAllPossibleSons();
    for i in range(len(sons)):
        print("i=",i,"'s board:", sep='');
        sons[i].printBoard();
        print();

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

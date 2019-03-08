import Car
from Direction import Direction
import copy

"""""
takes vector with size dimX * dimY
and returns matrix dimX by dimY
"""""
def vector_to_matrix(vector, dimX, dimY):
    mat = [vector[i:i+dimX] for i in range(0, dimX * dimY, dimX)];
    return mat;

def matrix_to_vector(matrix):
    # uses operator '+' on lists, so it concats them. (second arg so it wouldn't try to add 0 at first)
    vector = sum(matrix, []);
    return vector;

"""""
GameState object holds some state of our game
"""""
class GameState:
    dimX = 6;
    dimY = 6;

    def __init__(self, board=None, line=None, father=None, createdMove=None):

        if board is None and line is None:
            arr1dim = ['.' for x in range(0, GameState.dimX * GameState.dimY)];  # default board
            self.board = vector_to_matrix(arr1dim, GameState.dimX, GameState.dimY);
        elif board is None:  # but line is not..
            arr1dim = list(line);
            self.board = vector_to_matrix(arr1dim, GameState.dimX, GameState.dimY);
        else:
            self.board = board;

        self.key = self.unique_id_str()

        self.father = father;
        self.createdMove = createdMove;

        self.cars = self.findCarsInBoard();  # list of Cars
        self.sons = [];  # list of sons (GameState objects)
        self.father = father;

    # find all cars in self.board and returns array of Cars
    def findCarsInBoard(self):
        cars = [];
        # find horizontal cars
        i, j = 0, 0;
        while i < GameState.dimY:
            searching_start_position = True;
            j = 0;
            while j < GameState.dimX:
                if searching_start_position:
                    if (self.board[i][j] is not '.') and (j is not GameState.dimX - 1):
                        if self.board[i][j+1] == self.board[i][j]:
                            searching_start_position = False;  # entered creating new Car mode
                            start_position = [i, j];

                else:
                    if (j is GameState.dimX - 1) or (self.board[i][j+1] != self.board[i][j]):
                        searching_start_position = True;  # found one. next pixel we'll look for another
                        end_position = [i, j];
                        cars.append(Car.Car(self.board[i][j], end_position[1] - start_position[1] + 1, start_position, end_position, False));

                j += 1;
            i += 1;

        # find vertical cars
        i, j = 0, 0;
        while j < GameState.dimY:
            searching_start_position = True;
            i = 0;
            while i < GameState.dimX:
                if searching_start_position:
                    if (self.board[i][j] is not '.') and (i is not GameState.dimY - 1):
                        if self.board[i+1][j] == self.board[i][j]:
                                searching_start_position = False;  # entered creating new Car mode
                                start_position = [i, j];
                else:
                    if (i is GameState.dimY - 1) or (self.board[i+1][j] != self.board[i][j]):
                        searching_start_position = True;  # found one. next pixel we'll look for another
                        end_position = [i, j];
                        cars.append(Car.Car(self.board[i][j], end_position[0] - start_position[0] + 1, start_position, end_position, True));
                i += 1;
            j += 1;

        return cars;

    # example: self.moveCar_byName('X', 'RIGHT', 3);
    def moveCar_byName_ifpossible(self, carName, direction, steps):
        for car in self.cars:
            if carName == car.name:
                old_start_pos = [car.start_pos[0], car.start_pos[1]];  # deep copy
                old_end_pos = [car.end_pos[0], car.end_pos[1]];  # deep copy

                car_can_move_in_direction = car.move_ifpossible(direction, steps);
                if car_can_move_in_direction is 0:
                    return 0;

                # check if car didn't "hit" borders. means start/end is outside of range
                start_hit_border = not (0 <= car.start_pos[0] < GameState.dimX and 0 <= car.start_pos[1] < GameState.dimY);
                end_hit_border = not (0 <= car.end_pos[0] < GameState.dimX and 0 <= car.end_pos[1] < GameState.dimY);

                if start_hit_border or end_hit_border:  # no need to continue checking other fails
                    car.move_ifpossible(direction, -steps);  # we know it's possible. we moved already
                    return 0;

                # check if car didnt "hit" other cars. means start/end is sitting on other car
                start_hit_other_car = False;
                end_hit_other_car = False;

                if direction is Direction.RIGHT:
                    # check if there's other car between old_end_pos+1 and end_pos
                    for j in range(old_end_pos[1] + 1, car.end_pos[1] + 1):
                        if self.board[car.end_pos[0]][j] is not '.':  # car.end_pos[0] is constant
                            end_hit_other_car = True;
                            break;

                elif direction is Direction.LEFT:
                    # check if there's other car between start_pos and old_start_pos-1
                    for j in range(car.start_pos[1], old_start_pos[1]):
                        if self.board[car.start_pos[0]][j] is not '.':  # car.end_pos[0] is constant
                            start_hit_other_car = True;
                            break;

                elif direction is Direction.UP:
                    # check if there's other car between start_pos and old_start_pos-1
                    for i in range(car.start_pos[0], old_start_pos[0]):
                        if self.board[i][car.end_pos[1]] is not '.':  # car.end_pos[1] is constant
                            end_hit_other_car = True;
                            break;

                elif direction is Direction.DOWN:
                    # check if there's other car between old_end_pos+1 and end_pos
                    for i in range(old_end_pos[0] + 1, car.end_pos[0] + 1):
                        if self.board[i][car.start_pos[1]] is not '.':  # car.start_pos[1] is constant
                            start_hit_other_car = True;

                if start_hit_other_car or end_hit_other_car:
                    car.move_ifpossible(direction, -steps);  # we know it's possible. we moved already
                    return 0;

                # it's possible, just update the board!
                # delete old car (replace with '.') and insert new car (insert his name)
                if car.isVertical is False:
                    for j in range(old_start_pos[1], old_start_pos[1] + car.length):
                        self.board[old_start_pos[0]][j] = '.';
                    for j in range(car.start_pos[1], car.start_pos[1] + car.length):
                        self.board[car.start_pos[0]][j] = car.name;

                elif car.isVertical is True:
                    for i in range(old_start_pos[0], old_start_pos[0] + car.length):
                        self.board[i][old_start_pos[1]] = '.';
                    for i in range(car.start_pos[0], car.start_pos[0] + car.length):
                        self.board[i][car.start_pos[1]] = car.name;

                break;  # cars has unique name

    def findCar_byName(self, carName):
        for car in self.cars:
            if car.name == carName:
                return car;
        return None;

    def createAllPossibleSons(self):
        # needs to do all possible car moves
        sons = [];
        for car in self.cars:
            # do all possible moves with car
            if car.isVertical:
                # all possible UP moves
                for steps in range(1, car.start_pos[0] + 1):  # car.start_pos is pointing UP
                    # create new son by copying all attri's, and changing the attri's that were changed by the move
                    new_son = copy.deepcopy(self);  # deep copy recursively
                    ispossible = new_son.moveCar_byName_ifpossible(car.name, Direction.UP, steps);
                    if ispossible is 0:
                        break;
                    new_son.father = self;
                    new_son.createdMove = car.name + 'U' + str(steps);
                    new_son.key = new_son.unique_id_str();
                    sons.append(new_son);

                # all possible DOWN moves
                for steps in range(1, GameState.dimY-1 - car.end_pos[0] + 1):
                    # create new son by copying all attri's, and changing the attri's that were changed by the move
                    new_son = copy.deepcopy(self);  # deep copy recursively
                    ispossible = new_son.moveCar_byName_ifpossible(car.name, Direction.DOWN, steps);
                    if ispossible is 0:
                        break;
                    new_son.father = self;
                    new_son.createdMove = car.name + 'D' + str(steps);
                    new_son.key = new_son.unique_id_str();
                    sons.append(new_son);
            else:
                # all possible LEFT moves
                for steps in range(1, car.start_pos[0] + 1):
                    new_son = copy.deepcopy(self);  # deep copy recursively
                    ispossible = new_son.moveCar_byName_ifpossible(car.name, Direction.LEFT, steps);
                    if ispossible is 0:
                        break;
                    new_son.father = self;
                    new_son.createdMove = car.name + 'L' + str(steps);
                    new_son.key = new_son.unique_id_str();
                    sons.append(new_son);

                # all possible RIGHT moves
                for steps in range(1, GameState.dimX-1 - car.end_pos[1] + 1):
                    new_son = copy.deepcopy(self);  # deep copy recursively
                    ispossible = new_son.moveCar_byName_ifpossible(car.name, Direction.RIGHT, steps);
                    if ispossible is 0:
                        break;
                    new_son.father = self;
                    new_son.createdMove = car.name + 'R' + str(steps);
                    new_son.key = new_son.unique_id_str();
                    sons.append(new_son);

        self.sons = sons;

    def setBoard(self, line):
        arr1dim = list(line);
        self.board = vector_to_matrix(arr1dim, GameState.dimX, GameState.dimY);

    def printBoard(self):
        for i in range(0, GameState.dimY):
            for j in range(0, GameState.dimY):
                print(self.board[i][j], end='');
            print();  # new line

    def __eq__(self, other):
        if isinstance(other, GameState):
            return self.board == other.board;
        else:
            return False;

    # used for Best-First-Search
    def value(self):
        # f = g + h, when g is amount of work done, and h is optimistic
        return 0;

    def unique_id_str(self):
        # just return str of the board
        arr_str = matrix_to_vector(self.board);
        return ''.join(arr_str);  # char array to string

    def __lt__(self, other):
        return True
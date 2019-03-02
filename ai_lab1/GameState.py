import Car

"""""
takes vector with size dimX * dimY
and returns matrix dimX by dimY
"""""
def vector_to_matrix(vector, dimX, dimY):
    mat = [vector[i:i+dimX] for i in range(0, dimX * dimY, dimX)];
    return mat;


"""""
GameState object holds some state of our game
"""""
class GameState:
    dimX = 6;
    dimY = 6;

    def __init__(self, line=None):
        if line is not None:
            arr1dim = list(line);
        else:
            arr1dim = ['.' for x in range(0, GameState.dimX * GameState.dimY)];  # default board
        self.board = vector_to_matrix(arr1dim, GameState.dimX, GameState.dimY);
        self.cars = self.findCarsInBoard();

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
                        print(self.board[i][j], end_position[1] - start_position[1] + 1);
                        cars.append(Car.Car(self.board[i][j], end_position[1] - start_position[1] + 1, start_position, end_position, False));

                j += 1;
            i += 1;

        print();
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
                        print(self.board[i][j], end_position[0] - start_position[0] + 1);
                        cars.append(Car.Car(self.board[i][j], end_position[0] - start_position[0] + 1, start_position, end_position, False));
                i += 1;
            j += 1;



    def setBoard(self, line):
        arr1dim = list(line);
        self.board = vector_to_matrix(arr1dim, GameState.dimX, GameState.dimY);

    def printBoard(self):
        for i in range(0, GameState.dimY):
            for j in range(0, GameState.dimY):
                print(self.board[i][j], end='');
            print();  # new line

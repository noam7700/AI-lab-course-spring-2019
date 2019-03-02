

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

    def setBoard(self, line):
        arr1dim = list(line);
        self.board = vector_to_matrix(arr1dim, GameState.dimX, GameState.dimY);

    def printBoard(self):
        for i in range(0, GameState.dimY):
            for j in range(0, GameState.dimY):
                print(self.board[i][j], end='');
            print();  # new line

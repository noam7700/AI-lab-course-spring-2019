import GameState
from FibonacciHeap import FibonacciHeap

#
# class Node(object):
#     def __init__(self, key, board):
#         self.key = key
#         self.board = board
#         self.predecessor = None
#         self.sons = []
#
#     def __str__(self):
#         return str(self.key)
#
#     def __eq__(self, another):
#         return hasattr(another, 'board') and self.board == another.board
#
#     def __hash__(self):
#         return self.key
#
#
# def zero_f(board):
#     return board

class Node:

    def __init__(self, game_state, father_node, createdMove, key):
        self.game_state = game_state
        self.father_node = father_node
        self.createdMove = createdMove
        self.key = key
        self.cost_to_root = 0  # will be set later, depends on g_func (user defines cost function)
        self.score = 0  # will be set later, depends on f_func (user defines cost function & heuristic)
    def __lt__(self, other):
        return self.score < other.score


class FibonacciHeap_with_HashTable:
    def __init__(self):
        self.fibonacciHeap = FibonacciHeap()
        self.hashTable = {}

    def insert(self, value):
        inserted_node = self.fibonacciHeap.insert(value)
        self.hashTable[value.key] = inserted_node

    def extract_min(self):
        value = self.fibonacciHeap.extract_min()
        if value is not None:
            self.hashTable[value.key] = None
        return value

    def __contains__(self, key):
        return key in self.hashTable

    def isEmpty(self):
        return self.fibonacciHeap.size == 0

def a_star(start_game_state, h_func, g_func):
    if not bool(start_game_state):
        return False

    start_node = Node(start_game_state, None, None, start_game_state.unique_id_str())

    # open list contain for each cell x: key=x.key, value=(h(x)+g(x), x)
    start_g = g_func(start_node)
    start_h = h_func(start_node)
    start_node.cost_to_root = start_g
    start_node.score = start_g + start_h

    open_list = FibonacciHeap_with_HashTable()
    open_list.insert(start_node)
    closed_list = {}
    while not open_list.isEmpty():
        best = open_list.extract_min()

        # X reached to the right of the board, we won!
        if best.game_state.board[2][GameState.GameState.dimX - 1] is 'X':
            return best

        # add best to close_list, and add all best's sons to open_list
        closed_list[best.key] = best
        [sons_gamestates, sons_createdMove] = best.game_state.createAllPossibleSons()
        for i in range(len(sons_gamestates)):
            # if he is in one of them, then we found longer path, dont re-add it!
            # **needs to check if he's in open_list as well.
            son_key = sons_gamestates[i].unique_id_str()
            if son_key not in closed_list and son_key not in open_list:
                son_node = Node(sons_gamestates[i], best, sons_createdMove[i], son_key)
                son_h = h_func(son_node)
                son_g = g_func(son_node)
                son_node.cost_to_root = son_g
                son_node.score = son_h + son_g
                open_list.insert(son_node)  # add to open_list

    print("There is no solution")
    return None

def restore_solution_path(final_node):
    if final_node is None:
        return None

    solution_path = []
    curr_state = final_node
    # add solution_path from final_state to init_state
    while curr_state is not None:
        solution_path.append(curr_state)
        curr_state = curr_state.father_node

    solution_path.reverse()  # reverse it to be in order (from first to last)
    return solution_path

def restore_solution_moves(final_node):
    solution_path = restore_solution_path(final_node)
    moves = []  # list of moves (strings)
    for i in range(1, len(solution_path)):
        moves.append(solution_path[i].createdMove)
    return moves

# used as h_func
def heuristic(node):
    x_row = 2
    x_end_column = 0;

    # searching for X_car's end_pos because we are no longer keeping Car objects in game_state
    for j in range(GameState.GameState.dimX - 1, -1, -1):  # from dimX - 1 to 0
        if node.game_state.board[x_row][j] is 'X':  # found X!
            x_end_column = j
            break

    num_of_blocking_cars = 0
    for j in range(x_end_column + 1, GameState.GameState.dimX):
        if node.game_state.board[x_row][j] is not '.':
            num_of_blocking_cars += 1

    return num_of_blocking_cars

# used as g_func
def cost_to_root(node):
    if node.father_node is None:  # means we're the root
        return 0
    return node.father_node.cost_to_root + 1



# s_node = Node("A", 3)
# node1 = Node("B", 2)
# node2 = Node("C", 0)
# s_node.sons = {node1.key, node2.key}
# test_graph = {s_node.key: s_node, node1.key: node1, node2.key: node2}
# a_star(s_node, test_graph, zero_f, zero_f)

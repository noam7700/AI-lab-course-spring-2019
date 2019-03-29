import GameState
from time import time
from collections import namedtuple
from FibonacciHeap import FibonacciHeap


class Node:

    def __init__(self, game_state, father_node, created_move, key):
        self.game_state = game_state
        self.father_node = father_node
        self.createdMove = created_move
        self.key = key
        self.cost_to_root = 0  # will be set later, depends on g_func (user defines cost function)
        self.score = 0  # will be set later, depends on f_func (user defines cost function & heuristic)

    def __lt__(self, other):
        return self.score < other.score


class FibonacciHeapWithHashTable:
    def __init__(self):
        self.fibonacciHeap = FibonacciHeap()
        self.hashTable = {}

    # returns [whatChanged, old_value]
    # whatChanged -
    # 1: added as new node. 0: updated existing node (decreased value). -1: didn't do anything
    def insert(self, value):
        if value.key not in self:  # normal case. just add him
            inserted_node = self.fibonacciHeap.insert(value)
            self.hashTable[value.key] = inserted_node
            return [1, None]

        elif value < self.hashTable[value.key].value:  # value is already in open_list, check if we can improve him
            old_value = self.hashTable[value.key].value
            self.fibonacciHeap.decreaseValue(self.hashTable[value.key], value)  # decrease value of old value
            return [0, old_value]
            # no need to update hashTable..

        return [-1, None]

    def extract_min(self):
        value = self.fibonacciHeap.extract_min()
        if value is not None:
            self.hashTable[value.key] = None
        return value

    def __contains__(self, key):
        return key in self.hashTable

    def isEmpty(self):
        return self.fibonacciHeap.size == 0

    def peek(self):
        return self.fibonacciHeap.peek()

"""
Description: Searches an optimal solution using A-Star algorithm
Parameters: start_game_state - start node 
            h_func - an admissible heuristic function
            g_func - cost function
Returns: a tuple of form   (solution node, 
                            depth/nodes fetched, 
                            searching time, 
                            Avg. heuristic score, 
                            Effective Branching Factor,
                            minimum depth, 
                            maximum depth, 
                            average depth)
"""


def a_star(start_game_state, h_func, g_func, max_time_solution):

    Solution = namedtuple("Solution", ["solution_node", "num_searched", "permit", "exec_time", "avg_h", "ebf", "min_d", "max_d", "avg_d"])

    if not bool(start_game_state):
        return Solution(None, 0, 0, 0, 0, 0, 0, 0, 0)

    start_time = time()

    start_node = Node(start_game_state, None, None, start_game_state.unique_id_str())

    # open list contain for each cell x: key=x.key, value=(h(x)+g(x), x)
    start_g = g_func(start_node)
    start_h = h_func(start_node)
    start_node.cost_to_root = start_g
    start_node.score = start_g + start_h
    sum_h = start_h

    open_list = FibonacciHeapWithHashTable()
    open_list.insert(start_node)
    closed_list = {}

    min_depth = float('inf')
    max_depth = 0
    sum_depth = 0
    num_cutoffs = 0
    N = 1
    prev_best = start_node
    prev_depth = 0
    while not open_list.isEmpty():
        best = open_list.extract_min()

        search_time = time() - start_time
        if search_time >= max_time_solution:
            break

        if prev_best is not start_node and best.father_node is not prev_best:  # it's a cutoff!
            min_depth = min(min_depth, prev_depth)
            max_depth = max(max_depth, prev_depth)
            sum_depth = sum_depth + prev_depth
            prev_depth = best.cost_to_root

            num_cutoffs += 1

        else:
            prev_depth = prev_best.cost_to_root + 1
        prev_best = best

        # X reached to the right of the board, we won!
        if best.game_state.board[2][GameState.GameState.dimX - 1] is 'X':
            search_time = time() - start_time

            return Solution(best,
                            N,
                            best.cost_to_root/N,
                            search_time,
                            sum_h/N,
                            pow(N, (1/float(best.cost_to_root))),  # N^(1/d)
                            min_depth,
                            max_depth,
                            sum_depth/num_cutoffs)

        # add best to close_list, and add all best's sons to open_list
        closed_list[best.key] = best
        [sons_game_states, sons_created_move] = best.game_state.create_all_possible_sons()
        for i in range(len(sons_game_states)):
            # if he is in one of them, then we found longer path, dont re-add it!
            # **needs to check if he's in open_list as well.
            son_key = sons_game_states[i].unique_id_str()
            if son_key not in closed_list:
                son_node = Node(sons_game_states[i], best, sons_created_move[i], son_key)
                son_h = h_func(son_node)
                son_g = g_func(son_node)
                son_node.cost_to_root = son_g
                son_node.score = son_h + son_g
                [whatChanged, old_value] = open_list.insert(son_node)

                if whatChanged is 1:  # inserted as new node
                    N = N + 1
                    sum_h = sum_h + son_h
                elif whatChanged is 0:  # updated existing node (decreased value)
                    sum_h = sum_h - (old_value.score - float(old_value.cost_to_root)) + son_h  # sum_h - (old_h) + son_h



    search_time = time() - start_time
    return Solution(None,
                    N,
                    best.cost_to_root/N,  # d will be last best's depth
                    search_time,
                    sum_h/N,
                    pow(N, (1/float(best.cost_to_root))),  # N^(1/d)
                    min_depth,
                    max_depth,
                    sum_depth/num_cutoffs)


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


# used as h_func
def heuristic2(node):
    x_row = 2
    x_end_column = 0

    # searching for X_car's end_pos because we are no longer keeping Car objects in game_state
    for j in range(GameState.GameState.dimX - 1, -1, -1):  # from dimX - 1 to 0
        if node.game_state.board[x_row][j] is 'X':  # found X!
            x_end_column = j
            break

    num_of_blocking_cars = 0
    for j in range(x_end_column + 1, GameState.GameState.dimX):
        if node.game_state.board[x_row][j] is not '.':
            num_of_blocking_cars += 1

    # Adds Manhattan's distance between X and right border,
    # divided by m > 3, thus it preserves admissibility

    m = (5 - x_end_column)/4

    return num_of_blocking_cars + m


# used as h_func
def heuristic3(node):
    return 0


# used as g_func
def cost_to_root(node):
    if node.father_node is None:  # means we're the root
        return 0
    return node.father_node.cost_to_root + 1

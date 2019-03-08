

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


def a_star(start_node, f_func):
    if not bool(start_node):
        return False

    print("A Star algorithm: start_node - " + str(start_node) + ", graph - ")

    open_list = {start_node.key: (f_func(start_node), start_node)}
    closed_list = {}
    while bool(open_list):
        best = min(open_list, key=open_list.get)
        best_score_pair = open_list[best]
        if best_score_pair[0] == 0:
            print("The solution was found: " + best)
            return best_score_pair[1]

        open_list.pop(best)
        best_node = best_score_pair[1]
        closed_list[best_node.key] = best_score_pair
        best_node.createAllPossibleSons()
        for son in best_node.sons:
            if son.key not in closed_list:
                print("f is good")
                curr_son_score = f_func(son)
                open_list[son.key] = (curr_son_score, son)
    print("There is no solution")
    return None


# s_node = Node("A", 3)
# node1 = Node("B", 2)
# node2 = Node("C", 0)
# s_node.sons = {node1.key, node2.key}
# test_graph = {s_node.key: s_node, node1.key: node1, node2.key: node2}
# a_star(s_node, test_graph, zero_f, zero_f)

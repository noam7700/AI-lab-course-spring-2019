class Node(object):
    def __init__(self, key, data):
        self.key = key
        self.data = data
        self.predecessor = None
        self.sons = []

    def __str__(self):
        return str(self.key)

    def __eq__(self, another):
        return hasattr(another, 'data') and self.data == another.data

    def __hash__(self):
        return self.key


def zero_f(data):
    return data


def a_star(start_node, graph, f_func, g_func):
    if not bool(start_node):
        return False

    print("A Star algorithm: start_node - " + str(start_node) + ", graph - " + str(graph))

    open_list = {start_node.key: f_func(start_node.data)}

    while bool(open_list):
        best = min(open_list, key=open_list.get)
        best_score = open_list[best]
        if best_score == 0:
            print("The solution was found: " + best)
            return best

        open_list.pop(best)
        best_node = graph[best]
        closed_list = {best_node.key: f_func(best_node.data)}
        for son in best_node.sons:
            if son not in closed_list and son not in open_list:
                son_node = graph[son]
                son_node.predecessor = best_node.key
                curr_son_score = f_func(son_node.data)
                open_list[son] = curr_son_score
            # if f is not admissible
            else:
                print("Admissible f function is required")
    print("There is no solution")
    return None


s_node = Node("A", 3)
node1 = Node("B", 2)
node2 = Node("C", 0)
s_node.sons = {node1.key, node2.key}
test_graph = {s_node.key: s_node, node1.key: node1, node2.key: node2}
a_star(s_node, test_graph, zero_f, zero_f)

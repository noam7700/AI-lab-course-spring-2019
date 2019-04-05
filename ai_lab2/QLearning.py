
class QLearning:
    # steps
    weights = {}

    def increase(self, path):
        for node in path:
            key = hash(node.game_state.unique_id_str())
            if node.createdMove is not "":
                key = (hash(node.game_state.unique_id_str()), hash(node.createdMove))
            if key in self.weights:
                self.weights[key] += 1
            else:
                self.weights[key] = 1
        # self.normalize_weights()
        print("increase")
        # s = ""
        #         # i = 1
        #         # for w in self.weights:
        #         #     s += str(self.weights[w]) + " "
        #         #     if i%1000 is 0:
        #         #         s += "\n"
        #         #         i = 0
        #         #     i += 1
        #         # print(s)

    def decrease(self, path):
        for node in path:
            key = hash(node.game_state.unique_id_str())
            if node.createdMove is not "":
                key = (hash(node.game_state.unique_id_str()), hash(node.createdMove))
            if key in self.weights:
                self.weights[key] -= 1
            else:
                self.weights[key] = -1
        # self.normalize_weights()
        print("decrease")
        # s = ""
        # i = 1
        # for w in self.weights:
        #     s += str(self.weights[w]) + " "
        #     if i%200 is 0:
        #         s += "\n"
        #         i = 0
        #     i += 1
        # print(s)

    # used as h_func
    def heuristic_h(self, node):
        # print("heuristic_q_learning state dependent")
        # print(self.weights)
        key = hash(node.game_state.unique_id_str())
        if node.createdMove is not "":
            key = (hash(node.game_state.unique_id_str()), hash(node.createdMove))
        if key in self.weights:
            # print(str(node.createdMove))
            return self.weights[key]
        else:
            self.weights[key] = 0
            return 0

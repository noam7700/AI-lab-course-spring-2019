from AStar import FibonacciHeapWithHashTable
from AStar import Node
from time import time
from collections import namedtuple

"""
Bi Directional A*:
------------------
we do A* algorithm starting from init-state (forward A*), and finish-state (backward A*) simultaneously.
when they meet up in some node v we check the following:
g-forward(v) + g-backward(v) <= max(kf, kb)

if the condition checks out 

"""





def bi_directional_a_star(start_game_state, end_game_state, h_func_forward, h_func_backward, g_func, max_time_solution):

    Solution = namedtuple("Solution",
                          ["solution_node_forward", "solution_node_backward", "num_searched", "permit", "exec_time", "avg_h", "ebf", "min_d", "max_d",
                           "avg_d"])

    if not bool(start_game_state):
        return Solution(None, 0, 0, 0, 0, 0, 0, 0, 0)

    start_time = time()

    start_node = Node(start_game_state, None, None, start_game_state.unique_id_str())
    end_node = Node(end_game_state, None, None, end_game_state.unique_id_str())

    # attributes that are not set with constructor
    # set start_node's attris
    start_g = g_func(start_node)
    start_h = h_func_forward(start_node)
    start_node.cost_to_root = start_g
    start_node.score = start_g + start_h
    # set end_node's attris
    end_g = g_func(end_node)
    end_h = h_func_backward(end_node)
    end_node.cost_to_root = end_g
    end_node.score = end_g + end_h

    # OPENf - open forward. OPENb - open backward
    # init data structures
    OPENf = FibonacciHeapWithHashTable()
    OPENf.insert(start_node)
    CLOSEf = {}

    OPENb = FibonacciHeapWithHashTable()
    OPENb.insert(end_node)
    CLOSEb = {}

    # statistics
    min_depth = float('inf')  # between all the cutoffs nodes
    max_depth = 0  # between all the cutoffs nodes
    sum_depth = 0  # between all the cutoffs nodes
    num_cutoffs = 0
    N = 1
    sum_h = 0
    prev_best_forward = start_node  # used to keep track of cutoffs nodes
    prev_depth_forward = 0
    prev_best_backward = end_node  # used to keep track of cutoffs nodes
    prev_depth_backward = 0

    while not OPENf.isEmpty() or not OPENb.isEmpty():
        search_time = time() - start_time
        if search_time >= max_time_solution:
            break  # stop searching!

        if not OPENf.isEmpty():
            bestf = OPENf.extract_min()
            # DEBUG
            if bestf.key == "PAA...P.....P...XXB..Q.OBCCQ.ORRRQ.O":
                print("CMON MAN")
            elif bestf.key == "AA...OP..Q.OPXXQ.OP..Q..B...CCB.RRR.":
                print(("Started searching..."))
            # update statistics of min/max/sum depth
            if prev_best_forward is not start_node and bestf.father_node is not prev_best_forward:  # it's a cutoff!
                min_depth = min(min_depth, prev_depth_forward)
                max_depth = max(max_depth, prev_depth_forward)
                sum_depth = sum_depth + prev_depth_forward
                prev_depth_forward = bestf.cost_to_root
                num_cutoffs += 1
            elif prev_best_forward is not start_node:  # if it's start_node keep prev_depth = 0
                prev_depth_forward = prev_best_forward.cost_to_root + 1  # it's not a cutoff, therefor depth increased by 1
            prev_best_forward = bestf  # update prev to current for next iteration

            # check if finished
            if bestf.key in CLOSEb:  # means the two search trees met in node bestf
                search_time = time() - start_time

                # the larger deepening between the two search-trees (hopefully will be ~d/2 of AStar)
                max_d = max(bestf.cost_to_root, CLOSEb[bestf.key].cost_to_root)

                return Solution(bestf, CLOSEb[bestf.key],
                            N,
                            max_d/N,  # d/N
                            search_time,
                            sum_h/N,  # average h
                            pow(N, (1/float(max_d))),  # N^(1/d)
                            min_depth,
                            max_depth,
                            sum_depth/num_cutoffs)
            # else, open bestf
            # add to close
            CLOSEf[bestf.key] = bestf

            [sons_game_states, sons_created_move] = bestf.game_state.create_all_possible_sons()
            for i in range(len(sons_game_states)):
                # if he is in one of them, then we found longer path, dont re-add it!
                # **needs to check if he's in open_list as well. -- OPENf.insert does it for us
                son_key = sons_game_states[i].unique_id_str()
                if son_key not in CLOSEf:
                    son_node = Node(sons_game_states[i], bestf, sons_created_move[i], son_key)
                    son_h = h_func_forward(son_node)
                    son_g = g_func(son_node)
                    son_node.cost_to_root = son_g
                    son_node.score = son_h + son_g
                    [whatChanged, old_value] = OPENf.insert(son_node)

                    if whatChanged is 1:  # inserted as new node
                        N = N + 1
                        sum_h = sum_h + son_h
                    elif whatChanged is 0:  # updated existing node (decreased value)
                        sum_h = sum_h - (
                                    old_value.score - float(old_value.cost_to_root)) + son_h  # sum_h - (old_h) + son_h
        if not OPENb.isEmpty():
            bestb = OPENb.extract_min()
            # update statistics of min/max/sum depth
            if prev_best_backward is not end_node and bestb.father_node is not prev_best_backward:  # it's a cutoff!
                min_depth = min(min_depth, prev_depth_backward)
                max_depth = max(max_depth, prev_depth_backward)
                sum_depth = sum_depth + prev_depth_backward
                prev_depth_backward = bestb.cost_to_root
                num_cutoffs += 1
            elif prev_best_backward is not end_node:  # if it's end_node keep prev_depth = 0
                prev_depth_backward = prev_best_backward.cost_to_root + 1  # it's not a cutoff, therefor depth increased by 1
            prev_best_backward = bestb  # update prev to current for next iteration

            if bestb.key in CLOSEf:  # means the two search trees met in node bestf
                search_time = time() - start_time

                # the larger deepening between the two search-trees (hopefully will be ~d/2 of AStar)
                max_d = max(bestb.cost_to_root, CLOSEf[bestb.key].cost_to_root)
                return Solution(bestb, CLOSEf[bestb.key],
                                N,
                                max_d / N,  # d/N
                                search_time,
                                sum_h / N,  # average h
                                pow(N, (1 / float(max_d))),  # N^(1/d)
                                min_depth,
                                max_depth,
                                sum_depth / num_cutoffs)
            # else, open bestf
            # add to close
            CLOSEb[bestb.key] = bestb
            [sons_game_states, sons_created_move] = bestb.game_state.create_all_possible_sons()
            for i in range(len(sons_game_states)):
                # if he is in one of them, then we found longer path, dont re-add it!
                # **needs to check if he's in open_list as well. -- OPENb.insert does it for us
                son_key = sons_game_states[i].unique_id_str()
                if son_key not in CLOSEb:
                    son_node = Node(sons_game_states[i], bestb, sons_created_move[i], son_key)
                    son_h = h_func_backward(son_node)
                    son_g = g_func(son_node)
                    son_node.cost_to_root = son_g
                    son_node.score = son_h + son_g
                    [whatChanged, old_value] = OPENb.insert(son_node)

                    if whatChanged is 1:  # inserted as new node
                        N = N + 1
                        sum_h = sum_h + son_h
                    elif whatChanged is 0:  # updated existing node (decreased value)
                        sum_h = sum_h - (
                                    old_value.score - float(old_value.cost_to_root)) + son_h  # sum_h - (old_h) + son_h


    search_time = time() - start_time

    max_d = max(bestf.cost_to_root, bestb.cost_to_root)
    return Solution(None, None,
                    N,
                    max_d / N,  # d will be last best's depth
                    search_time,
                    sum_h / N,
                    pow(N, (1 / float(max_d))),  # N^(1/d)
                    min_depth,
                    max_depth,
                    sum_depth / num_cutoffs)


def heuristic_backward(node):
    return 0  # underestimates :(

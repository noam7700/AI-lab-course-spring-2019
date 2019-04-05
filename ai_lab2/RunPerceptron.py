import sys
import os
import ai_lab1.GameState
import ai_lab1.AStar
from ai_lab1.AStar import QLearning
from ai_lab1.FibonacciHeap import FibonacciHeap
from ai_lab1.BinaryHeap import BinaryHeap
import random
import pickle


def RunPerceptron(path_to_input, max_time, is_training=False):
    path_to_rh = path_to_input  # pathname for rh.txt
    max_time_solution = max_time  # if exceeds max_time_solution mark as failed

    ql = QLearning()
    # Getting back the objects:
    if os.path.exists('weights.pkl'):
        with open('weights.pkl', 'rb') as f:  # Python 3: open(..., 'rb')
            ql.weights = pickle.load(f)

    results = []
    #TRAINING
    for iteration in range(5):
        rh_input_file = open(path_to_rh, 'r')
        # read first problem
        for i in range(30):
            print("----------------------------- TRAINING: Board " + str(i) + ", Iteration: {}".format(iteration) + ": ----------------------------------\n")
            init_state_line = rh_input_file.readline(GameState.GameState.dimX * GameState.GameState.dimY)
            rh_input_file.readline(1)  # read '\n'

            init_game_state = GameState.GameState(None, None, init_state_line)

            golden_train_model = AStar.a_star(init_game_state, AStar.heuristic2, AStar.cost_to_root)
            if golden_train_model.solution_node is None:

                print("----cost - FAILED")
                print("----tries - {}".format(len(golden_train_model.search_path)))
                print("Heuristic - " + "\n" +
                                     "  Number of cars block the path to exit + " + "\n" +
                                     "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                     "Solution - " + "\n   " +
                                     "  FAILED" + "\n" +
                                     "Statistics - " + "\n" +
                                     "  Permittivity: " + str("%.4f" % golden_train_model.permit) + "\n" +
                                     "  Searching Time: " + str("%.4f" % golden_train_model.exec_time) + "\n" +
                                     "  Average heuristic score: " + str("%.4f" % result2.avg_h) + "\n" +
                                     "  EBF: " + str("%.4f" % golden_train_model.ebf) + "\n" +
                                     "  Minimum depth: " + str("%.4f" % golden_train_model.min_d) + "\n" +
                                     "  Maximum depth: " + str("%.4f" % golden_train_model.max_d) + "\n" +
                                     "  Average depth: " + str("%.4f" % golden_train_model.avg_d) + "\n" +
                                     "\n")

            else:
                golden_train_model_moves = AStar.restore_solution_moves(golden_train_model.solution_node)
                golden_train_model_states = AStar.restore_solution_path(golden_train_model.solution_node)
                print("----cost - {}".format(golden_train_model.solution_node.cost_to_root))
                print("----tries - {}".format(len(golden_train_model.search_path)))
                print("Heuristic - " + "\n" +
                                     "  Number of cars block the path to exit + " + "\n" +
                                     "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                     "Solution - " + "\n    " +
                                     ' '.join(golden_train_model_moves) + "\n" +
                                     "Statistics - " + "\n" +
                                     "  Permittivity: " + str("%.4f" % golden_train_model.permit) + "\n" +
                                     "  Searching Time: " + str("%.4f" % golden_train_model.exec_time) + "\n" +
                                     "  Average heuristic score: " + str("%.4f" % golden_train_model.avg_h) + "\n" +
                                     "  EBF: " + str("%.4f" % golden_train_model.ebf) + "\n" +
                                     "  Minimum depth: " + str("%.4f" % golden_train_model.min_d) + "\n" +
                                     "  Maximum depth: " + str("%.4f" % golden_train_model.max_d) + "\n" +
                                     "  Average depth: " + str("%.4f" % golden_train_model.avg_d) + "\n" +
                                     "\n")
                changes = ql.decrease(golden_train_model_states)

            print("-------------------------------------------------------------------------------\n")
            q_train_model = AStar.a_star(init_game_state, ql.heuristic_h, AStar.cost_to_root)
            if q_train_model.solution_node is None:
                print("----cost - FAILED")
                print("----tries - {}".format(len(q_train_model.search_path)))
                print("Heuristic - " + "\n" +
                      "  heuristic_q_learning" + "\n" +
                      "Solution - " + "\n   " +
                      "  FAILED" + "\n" +
                      "Statistics - " + "\n" +
                      "  Permittivity: " + str("%.4f" % q_train_model.permit) + "\n" +
                      "  Searching Time: " + str("%.4f" % q_train_model.exec_time) + "\n" +
                      "  Average heuristic score: " + str("%.4f" % q_train_model.avg_h) + "\n" +
                      "  EBF: " + str("%.4f" % q_train_model.ebf) + "\n" +
                      "  Minimum depth: " + str("%.4f" % q_train_model.min_d) + "\n" +
                      "  Maximum depth: " + str("%.4f" % q_train_model.max_d) + "\n" +
                      "  Average depth: " + str("%.4f" % q_train_model.avg_d) + "\n" +
                      "\n")

            else:
                q_train_model_moves = AStar.restore_solution_moves(q_train_model.solution_node)
                q_train_model_states = AStar.restore_solution_path(q_train_model.solution_node)
                print("----cost - {}".format(q_train_model.solution_node.cost_to_root))
                print("----tries - {}".format(len(q_train_model.search_path)))
                print("Heuristic - " + "\n" +
                      "  heuristic_q_learning" + "\n" +
                      "Solution - " + "\n    " +
                      ' '.join(q_train_model_moves) + "\n" +
                      "Statistics - " + "\n" +
                      "  Permittivity: " + str("%.4f" % q_train_model.permit) + "\n" +
                      "  Searching Time: " + str("%.4f" % q_train_model.exec_time) + "\n" +
                      "  Average heuristic score: " + str("%.4f" % q_train_model.avg_h) + "\n" +
                      "  EBF: " + str("%.4f" % q_train_model.ebf) + "\n" +
                      "  Minimum depth: " + str("%.4f" % q_train_model.min_d) + "\n" +
                      "  Maximum depth: " + str("%.4f" % q_train_model.max_d) + "\n" +
                      "  Average depth: " + str("%.4f" % q_train_model.avg_d) + "\n" +
                      "\n")

            changes = ql.increase(q_train_model.search_path)
            print("---------------------------------- DIFF: {} -----------------------------------\n".format(golden_train_model.solution_node.cost_to_root-q_train_model.solution_node.cost_to_root))
            print("-------------------------------------------------------------------------------\n")
    # TEST:
    rh_input_file = open(path_to_rh, 'r')
    for i in range(10):
        print("----------------------------- TESTING: Board " + str(i) + " : ----------------------------------\n")
        init_state_line = rh_input_file.readline(GameState.GameState.dimX * GameState.GameState.dimY)
        rh_input_file.readline(1)  # read '\n'

        init_game_state = GameState.GameState(None, None, init_state_line)

        golden_test_model = AStar.a_star(init_game_state, AStar.heuristic2, AStar.cost_to_root)
        golden_test_model_path = []

        if golden_test_model.solution_node is None:
            print("----cost - FAILED")
            print("----tries - {}".format(len(golden_test_model.search_path)))
            print("Heuristic - " + "\n" +
                                 "  Number of cars block the path to exit + " + "\n" +
                                 "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                 "Solution - " + "\n   " +
                                 "  FAILED" + "\n" +
                                 "Statistics - " + "\n" +
                                 "  Permittivity: " + str("%.4f" % golden_test_model.permit) + "\n" +
                                 "  Searching Time: " + str("%.4f" % golden_test_model.exec_time) + "\n" +
                                 "  Average heuristic score: " + str("%.4f" % golden_test_model.avg_h) + "\n" +
                                 "  EBF: " + str("%.4f" % golden_test_model.ebf) + "\n" +
                                 "  Minimum depth: " + str("%.4f" % golden_test_model.min_d) + "\n" +
                                 "  Maximum depth: " + str("%.4f" % golden_test_model.max_d) + "\n" +
                                 "  Average depth: " + str("%.4f" % golden_test_model.avg_d) + "\n" +
                                 "\n")

        else:
            golden_test_model_moves = AStar.restore_solution_moves(golden_test_model.solution_node)
            print("----cost - {}".format(golden_test_model.solution_node.cost_to_root))
            print("----tries - {}".format(len(golden_test_model.search_path)))
            print("Heuristic - " + "\n" +
                                 "  Number of cars block the path to exit + " + "\n" +
                                 "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                 "Solution - " + "\n    " +
                                 ' '.join(golden_test_model_moves) + "\n" +
                                 "Statistics - " + "\n" +
                                 "  Permittivity: " + str("%.4f" % golden_test_model.permit) + "\n" +
                                 "  Searching Time: " + str("%.4f" % golden_test_model.exec_time) + "\n" +
                                 "  Average heuristic score: " + str("%.4f" % golden_test_model.avg_h) + "\n" +
                                 "  EBF: " + str("%.4f" % golden_test_model.ebf) + "\n" +
                                 "  Minimum depth: " + str("%.4f" % golden_test_model.min_d) + "\n" +
                                 "  Maximum depth: " + str("%.4f" % golden_test_model.max_d) + "\n" +
                                 "  Average depth: " + str("%.4f" % golden_test_model.avg_d) + "\n" +
                                 "\n")

        print("-------------------------------------------------------------------------------\n")

        q_test_model = AStar.a_star(init_game_state, ql.heuristic_h, AStar.cost_to_root)
        q_test_model_moves = []
        if q_test_model.solution_node is None:
            print("----cost - FAILED")
            print("----tries - {}".format(len(q_test_model.search_path)))
            print("Heuristic - " + "\n" +
                  "  heuristic_q_learning" + "\n" +
                  "Solution - " + "\n   " +
                  "  FAILED" + "\n" +
                  "Statistics - " + "\n" +
                  "  Permittivity: " + str("%.4f" % q_test_model.permit) + "\n" +
                  "  Searching Time: " + str("%.4f" % q_test_model.exec_time) + "\n" +
                  "  Average heuristic score: " + str("%.4f" % q_test_model.avg_h) + "\n" +
                  "  EBF: " + str("%.4f" % q_test_model.ebf) + "\n" +
                  "  Minimum depth: " + str("%.4f" % q_test_model.min_d) + "\n" +
                  "  Maximum depth: " + str("%.4f" % q_test_model.max_d) + "\n" +
                  "  Average depth: " + str("%.4f" % q_test_model.avg_d) + "\n" +
                  "\n")

        else:
            q_test_model_moves = AStar.restore_solution_moves(q_test_model.solution_node)
            print("----cost - {}".format(q_test_model.solution_node.cost_to_root))
            print("----tries - {}".format(len(q_test_model.search_path)))
            print("Heuristic - " + "\n" +
                  "  heuristic_q_learning" + "\n" +
                  "Solution - " + "\n    " +
                  ' '.join(q_test_model_moves) + "\n" +
                  "Statistics - " + "\n" +
                  "  Permittivity: " + str("%.4f" % q_test_model.permit) + "\n" +
                  "  Searching Time: " + str("%.4f" % q_test_model.exec_time) + "\n" +
                  "  Average heuristic score: " + str("%.4f" % q_test_model.avg_h) + "\n" +
                  "  EBF: " + str("%.4f" % q_test_model.ebf) + "\n" +
                  "  Minimum depth: " + str("%.4f" % q_test_model.min_d) + "\n" +
                  "  Maximum depth: " + str("%.4f" % q_test_model.max_d) + "\n" +
                  "  Average depth: " + str("%.4f" % q_test_model.avg_d) + "\n" +
                  "\n")

        test_diff = golden_test_model.solution_node.cost_to_root - q_test_model.solution_node.cost_to_root
        results.append(test_diff)
        print("---------------------------------- DIFF: {} -----------------------------------\n".format(test_diff))
        print("-------------------------------------------------------------------------------\n")
        # Saving the objects:
        with open('weights.pkl', 'wb') as f:  # Python 3: open(..., 'wb')
            pickle.dump(ql.weights, f)
    print(results)
    return 0


if __name__ == '__main__':
    main()

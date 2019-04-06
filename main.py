import sys
import os

import RunPerceptron as run_perc
import BiDirectionalAStar
import IDA_star
import NewHeuristics


if __name__ == '__main__':
    sys.path.insert(0, os.path.abspath("ai_lab1"))
    sys.path.insert(0, os.path.abspath("ai_lab2"))  # lab2 extends lab1

    path_to_rh = sys.argv[1]  # pathname for rh.txt
    max_time_solution = sys.argv[2]  # if exceeds max_time_solution mark as failed
    option = sys.argv[3]

    # Question 1 - New Heuristics
    if option is '1':
        NewHeuristics.runHeuristic2(path_to_rh, max_time_solution)
    elif option is '2':
        NewHeuristics.runSumDistancesofBlockingCars(path_to_rh, max_time_solution)
    elif option is '3':
        NewHeuristics.runHeuristic2Degree2(path_to_rh, max_time_solution)
    elif option is '4':
        NewHeuristics.runFreedom(path_to_rh, max_time_solution)
    elif option is '5':
        NewHeuristics.runFreedomAndHeuristic2Degree2(path_to_rh, max_time_solution)

    # Question 2 - Iterative Deepening A*
    elif option is '6':
        IDA_star.runIDAStar(path_to_rh, max_time_solution)

    # Question 3 - Bi Directional A*
    elif option is '7':
        BiDirectionalAStar.runBiDirectionalAStar(path_to_rh, max_time_solution)

    # Question 4 - Learning
    elif option is '8':
        # run_basic.RunBasicHeuristics(path_to_input=path_to_rh, max_time=max_time_solution)
        run_perc.RunPerceptron(path_to_input=path_to_rh, max_time=max_time_solution)



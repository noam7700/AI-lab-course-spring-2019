import sys
import os
from ai_lab1 import RunBasicHeuristics as run_basic

if __name__ == '__main__':
    sys.path.insert(0, os.path.abspath("ai_lab1"))
    sys.path.insert(0, os.path.abspath("ai_lab2"))  # lab2 extends lab1

    path_to_rh = sys.argv[1]  # pathname for rh.txt
    max_time_solution = sys.argv[2]  # if exceeds max_time_solution mark as failed

    run_basic.RunBasicHeuristics(path_to_input=path_to_rh, max_time=max_time_solution)


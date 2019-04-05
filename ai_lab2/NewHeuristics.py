import sys
sys.path.insert(0, "C:/Users/nir blagovsky/PycharmProjects/ai_lab/ai_lab1")  # lab2 extends lab1

import GameState  # for testing + some implementations
import AStar  # for testing

def sum_distances_of_blocking_cars(node):
    x_row = 2
    x_end_column = 0;

    # searching for X_car's end_pos because we are no longer keeping Car objects in game_state
    for j in range(GameState.GameState.dimX - 1, -1, -1):  # from dimX - 1 to 0
        if node.game_state.board[x_row][j] is 'X':  # found X!
            x_end_column = j
            break

    sum_distances = 0  # return value

    for j in range(x_end_column+1,GameState.GameState.dimX):
        if node.game_state.board[x_row][j] is not '.':  # for each blocking car
            car_name = node.game_state.board[x_row][j]
            row_start = 1
            row_end = 1

            for i in range(x_row-1, -1, -1):  # check how much distance to move the car downwards
                if i is 0:  # special case!
                    if node.game_state.board[i][j] is car_name:
                        row_start = i
                    else:
                        row_start = i + 1

                if node.game_state.board[i][j] is not car_name:
                    row_start = i + 1  # previous pixel was his end
                    break


            for i in range(x_row+1, GameState.GameState.dimY):
                if i is GameState.GameState.dimY - 1:  # special case!
                    if node.game_state.board[i][j] is car_name:
                        row_start = i
                    else:
                        row_start = i + 1

                if node.game_state.board[i][j] is not car_name:
                    row_end = i - 1  # previous pixel was his end
                    break

            # distance to clear the way for the "red car", is min between moving up or moving down to clear the way
            car_length = row_end - row_start + 1

            dist_to_down = x_row - row_start + 1  # x_row >= row_start so it's ok
            dist_to_up = row_end - x_row + 1  # row_end >= x_row so it's ok
            if car_length is 3:  # only option is to go down (cant clear the way by going up)
                sum_distances += dist_to_down  # distance to go down & clear the way

            else:  # check who's better, up or down
                sum_distances += min(dist_to_up, dist_to_down)

    return sum_distances

def heuristic2_degree2(node):
    # result = #blockers + #blockers_of_blockers
    # if a blocker is a trunk, her blockers are only cars that block downwards (only option...)
    # if a blocker is a car, her blockers is her best option (be optimistic)
    blockers = 0
    blockers_of_blockers = 0

    # if blocker of blocker was counted for blocking previous blocker. don't re-count him.
    # for example, if some car, blocks 2 blockers. she can with 1 step, un-block them!
    seen_blockersx2 = {}

    x_row = 2
    x_end_column = 0

    # searching for X_car's end_pos because we are no longer keeping Car objects in game_state
    for j in range(GameState.GameState.dimX - 1, -1, -1):  # from dimX - 1 to 0
        if node.game_state.board[x_row][j] is 'X':  # found X!
            x_end_column = j
            break

    for j in range(x_end_column+1, GameState.GameState.dimX):
        if node.game_state.board[x_row][j] is not '.':  # found a blocker
            blockers += 1
            car_name = node.game_state.board[x_row][j]

            row_start = 0
            row_end = 0

            for i in range(x_row - 1, -1, -1):  # check how much distance to move the car downwards
                if i is 0:  # special case!
                    if node.game_state.board[i][j] is car_name:
                        row_start = i
                    else:
                        row_start = i + 1

                if node.game_state.board[i][j] is not car_name:
                    row_start = i + 1  # previous pixel was his end
                    break

            for i in range(x_row + 1, GameState.GameState.dimY):
                if i is GameState.GameState.dimY - 1:  # special case!
                    if node.game_state.board[i][j] is car_name:
                        row_start = i
                    else:
                        row_start = i - 1

                if node.game_state.board[i][j] is not car_name:
                    row_end = i - 1
                    break

            # to recognize if it's a trunk or a car
            car_length = row_end - row_start + 1

            # blockers of downwards:
            down = 0
            up = 0
            for i in range(row_end+1, GameState.GameState.dimY):
                # blocker of blocker that wasn't counted yet
                if node.game_state.board[i][j] is not '.' and not node.game_state.board[i][j] in seen_blockersx2:
                    down += 1
            for i in range(row_start-1, -1, -1):
                # blocker of blocker that wasn't counted yet
                if node.game_state.board[i][j] is not '.' and not node.game_state.board[i][j] in seen_blockersx2:
                    up += 1

            isUp = -1  # which cars should I add to hash (seen_blockersx2)
            if car_length is 3:  # trunk can only clear the way if it goes down.
                blockers_of_blockers += down
                isUp = 0
            else:  # car can go up/down to clear the way. we're optimistic
                if down < up:
                    blockers_of_blockers += down
                    isUp = 0
                else:
                    blockers_of_blockers += up
                    isUp = 1

            if isUp is 1:
                # add upwards blockers of blockers to hash
                for i in range(row_start - 1, -1, -1):
                    if node.game_state.board[i][j] is not '.':
                        seen_blockersx2[node.game_state.board[i][j]] = 1
            else:
                for i in range(row_end + 1, GameState.GameState.dimY):
                    if node.game_state.board[i][j] is not '.':
                        seen_blockersx2[node.game_state.board[i][j]] = 1

    return blockers + blockers_of_blockers

def freedom(node):
    c = 4  # as explained in the pdf file. definiton of blocking car
    d = 1  # "factor of penalty". the bigger the d, the bigger the impact of the heuristic on the search.
    return freedom_c(node, c, d)

def freedom_c(node, c, d):

    if node.createdMove is None:
        return 0

    last_action = node.createdMove  # for example, AR4
    car_name = last_action[0]
    direction = last_action[1]
    steps = int(last_action[2])

    # find start position. searching up-to-down & left-to-right.
    start_row = 0
    start_column = 0
    didBreak = 0  # break from outside for
    for i in range(GameState.GameState.dimY):
        for j in range(GameState.GameState.dimX):
            if node.game_state.board[i][j] is car_name:
                start_row = i
                start_column = j
                didBreak = 1
                break
        if didBreak is 1:
            break

    # find end position.
    end_column = start_column
    end_row = start_row
    car_length = 0
    if direction is 'R' or direction is 'L':  # search right
        while end_column < GameState.GameState.dimX and node.game_state.board[start_row][end_column] is car_name:
            end_column += 1
        end_column -= 1  # we went one too far with end_column
        car_length = abs(end_column - start_column) + 1
    else:  # search down
        while end_row < GameState.GameState.dimY and node.game_state.board[end_row][start_column] is car_name:
            end_row += 1
        end_row -= 1  # we went one too far with end_row

        car_length = abs(end_row - start_row) + 1

    # prev_start & prev_end
    if direction is 'R':
        prev_start_row = start_row
        prev_end_row = start_row
        prev_start_column = start_column - steps
        prev_end_column = prev_start_column + car_length - 1
    elif direction is 'L':
        prev_start_row = start_row
        prev_end_row = start_row
        prev_start_column = start_column + steps
        prev_end_column = prev_start_column + car_length - 1
    elif direction is 'U':
        prev_start_row = start_row + steps
        prev_end_row = prev_start_row + car_length - 1
        prev_start_column = start_column
        prev_end_column = start_column
    elif direction is 'D':
        prev_start_row = start_row - steps
        prev_end_row = prev_start_row + car_length - 1
        prev_start_column = start_column
        prev_end_column = start_column


    # I know there are some code duplications. but it would complicate it if we would try to save some code
    prev_blocked_cars = 0
    blocked_cars = 0
    if direction is 'R' or direction is 'L':  # horizontal
        # count number of freed cars - pixels of previous position
        for j in range(prev_start_column, prev_end_column + 1):  # exapmle: AA... --AR3--> ...AA
            prev_counted_car = '~'  # don't re-count a car twice
            for i in range(max(prev_start_row-c, 0), prev_start_row):  # freed cars from up
                if node.game_state.board[i][j] is not '.' and node.game_state.board[i][j] is not prev_counted_car:
                    isVertical = (i+1 < GameState.GameState.dimY) and node.game_state.board[i][j] is node.game_state.board[i+1][j]
                    isVertical = isVertical or (i-1 >= 0 and node.game_state.board[i][j] is node.game_state.board[i-1][j])
                    if isVertical:  # it blocks it's movement only if it's vertical
                        prev_blocked_cars += 1
                        prev_counted_car = node.game_state.board[i][j]

            prev_counted_car = '~'  # don't re-count a car twice
            for i in range(min(prev_start_row+c, GameState.GameState.dimY-1), prev_start_row, -1):  # freed cars from down
                if node.game_state.board[i][j] is not '.' and node.game_state.board[i][j] is not prev_counted_car:
                    isVertical = (i + 1 < GameState.GameState.dimY) and node.game_state.board[i][j] is node.game_state.board[i + 1][j]
                    isVertical = isVertical or (i - 1 >= 0 and node.game_state.board[i][j] is node.game_state.board[i - 1][j])
                    if isVertical:  # it blocks it's movement only if it's vertical
                        prev_blocked_cars += 1
                        prev_counted_car = node.game_state.board[i][j]

        # count number of blocked cars - pixels of current position
        for j in range(start_column, end_column + 1):  # exapmle: AA... --AR3--> ...AA
            prev_counted_car = '~'  # don't re-count a car twice
            for i in range(max(start_row-c, 0), start_row):  # blocked cars from up
                if node.game_state.board[i][j] is not '.' and node.game_state.board[i][j] is not prev_counted_car:
                    isVertical = (i + 1 < GameState.GameState.dimY) and node.game_state.board[i][j] is node.game_state.board[i + 1][j]
                    isVertical = isVertical or (i - 1 >= 0 and node.game_state.board[i][j] is node.game_state.board[i - 1][j])
                    if isVertical:  # it blocks it's movement only if it's vertical
                        blocked_cars += 1
                        prev_counted_car = node.game_state.board[i][j]

            prev_counted_car = '~'  # don't re-count a car twice
            for i in range(min(start_row+c, GameState.GameState.dimY-1), start_row, -1):  # blocked cars from down
                if node.game_state.board[i][j] is not '.' and node.game_state.board[i][j] is not prev_counted_car:
                    isVertical = (i + 1 < GameState.GameState.dimY) and node.game_state.board[i][j] is node.game_state.board[i + 1][j]
                    isVertical = isVertical or (i - 1 >= 0 and node.game_state.board[i][j] is node.game_state.board[i - 1][j])
                    if isVertical:  # it blocks it's movement only if it's vertical
                        blocked_cars += 1
                        prev_counted_car = node.game_state.board[i][j]

    elif direction is 'U' or direction is 'D':  # condition isn't really necessary, it's else
        # count number of freed cars - pixels of previous position
        for i in range(prev_start_row, prev_end_row + 1):
            prev_counted_car = '~'  # don't re-count a car twice
            for j in range(max(start_column-c, 0), start_column):  # freed cars from left
                if node.game_state.board[i][j] is not '.' and node.game_state.board[i][j] is not prev_counted_car:
                    isHorizontal = (j+1 < GameState.GameState.dimX) and node.game_state.board[i][j] is node.game_state.board[i][j + 1]
                    isHorizontal = isHorizontal or (j-1 >= 0 and node.game_state.board[i][j] is node.game_state.board[i][j - 1])
                    if isHorizontal:  # it blocks it's movement only if it's horizontal
                        prev_blocked_cars += 1
                        prev_counted_car = node.game_state.board[i][j]

            prev_counted_car = '~'  # don't re-count a car twice
            for j in range(min(start_column+c, GameState.GameState.dimX-1), start_column, -1):  # freed cars from right
                if node.game_state.board[i][j] is not '.' and node.game_state.board[i][j] is not prev_counted_car:
                    isHorizontal = (j+1 < GameState.GameState.dimX) and node.game_state.board[i][j] is node.game_state.board[i][j + 1]
                    isHorizontal = isHorizontal or (j-1 >= 0 and node.game_state.board[i][j] is node.game_state.board[i][j - 1])
                    if isHorizontal:  # it blocks it's movement only if it's horizontal
                        prev_blocked_cars += 1
                        prev_counted_car = node.game_state.board[i][j]

        # count number of blocked cars - pixels of current position
        for i in range(start_row, end_row + 1):
            prev_counted_car = '~'  # don't re-count a car twice
            for j in range(max(start_column - c, 0), start_column):  # blocked cars from left
                if node.game_state.board[i][j] is not '.' and node.game_state.board[i][j] is not prev_counted_car:
                    isHorizontal = (j+1 < GameState.GameState.dimX) and node.game_state.board[i][j] is node.game_state.board[i][j + 1]
                    isHorizontal = isHorizontal or (j-1 >= 0 and node.game_state.board[i][j] is node.game_state.board[i][j - 1])
                    if isHorizontal:  # it blocks it's movement only if it's horizontal
                        blocked_cars += 1
                        prev_counted_car = node.game_state.board[i][j]

            prev_counted_car = '~'  # don't re-count a car twice
            for j in range(min(start_column + c, GameState.GameState.dimX), start_column):  # blocked cars from right
                if node.game_state.board[i][j] is not '.' and node.game_state.board[i][j] is not prev_counted_car:
                    isHorizontal = (j+1 < GameState.GameState.dimX) and node.game_state.board[i][j] is node.game_state.board[i][j + 1]
                    isHorizontal = isHorizontal or (j-1 >= 0 and node.game_state.board[i][j] is node.game_state.board[i][j - 1])
                    if isHorizontal:  # it blocks it's movement only if it's horizontal
                        blocked_cars += 1
                        prev_counted_car = node.game_state.board[i][j]

    last_action_heuristic_cost = blocked_cars - prev_blocked_cars
    prev_heuristic = node.score - node.cost_to_root
    # d is the factor of penalty
    return prev_heuristic + d * last_action_heuristic_cost

def freedom_and_heu2deg2(node):
    return 0.3 * freedom(node) + heuristic2_degree2(node)

def runSumDistancesofBlockingCars(path_to_input, max_time):
    runBasicAStar(path_to_input, max_time, sum_distances_of_blocking_cars)

def runHeuristic2(path_to_input, max_time):
    runBasicAStar(path_to_input, max_time, AStar.heuristic2)

def runHeuristic2Degree2(path_to_input, max_time):
    runBasicAStar(path_to_input, max_time, heuristic2_degree2)

def runFreedom(path_to_input, max_time):
    runBasicAStar(path_to_input, max_time, freedom)

def runFreedomAndHeuristic2Degree2(path_to_input, max_time):
    runBasicAStar(path_to_input, max_time, freedom_and_heu2deg2)

def runBasicAStar(path_to_input, max_time, heuristic):

    rh_input_file = open(path_to_input, 'r')
    solutions_file = open("./solutions.txt", 'w')

    for i in range(5):
        print(i+1)
        solutions_file.write(
            "----------------------------- Board " + str(i + 1) + ": ----------------------------------\n")
        init_state_line = rh_input_file.readline(GameState.GameState.dimX * GameState.GameState.dimY)
        rh_input_file.readline(1)  # read '\n'

        init_game_state = GameState.GameState(None, None, init_state_line)

        result2 = AStar.a_star(init_game_state, heuristic, AStar.cost_to_root, float(max_time))

        if result2.solution_node is None:
            solutions_file.write("Heuristic - " + "\n" +
                                 "  Number of cars block the path to exit + " + "\n" +
                                 "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                 "Solution - " + "\n   " +
                                 "  FAILED" + "\n" +
                                 "Statistics - " + "\n" +
                                 "  Number of searched nodes: " + str("%.4f" % result2.num_searched) + "\n" +
                                 "  Permittivity: " + str("%.4f" % result2.permit) + "\n" +
                                 "  Searching Time: " + str("%.4f" % result2.exec_time) + "\n" +
                                 "  Average heuristic score: " + str("%.4f" % result2.avg_h) + "\n" +
                                 "  EBF: " + str("%.4f" % result2.ebf) + "\n" +
                                 "  Minimum depth: " + str("%.4f" % result2.min_d) + "\n" +
                                 "  Maximum depth: " + str("%.4f" % result2.max_d) + "\n" +
                                 "  Average depth: " + str("%.4f" % result2.avg_d) + "\n" +
                                 "\n")
        else:
            moves = AStar.restore_solution_moves(result2.solution_node)

            solutions_file.write("Heuristic - " + "\n" +
                                 "  Number of cars block the path to exit + " + "\n" +
                                 "  Normalized Manhattan's distance (Admissible)" + "\n" +
                                 "Solution - " + "\n    " +
                                 ' '.join(moves) + "\n" +
                                 "Statistics - " + "\n" +
                                 "  Number of searched nodes: " + str("%.4f" % result2.num_searched) + "\n" +
                                 "  Permittivity: " + str("%.4f" % result2.permit) + "\n" +
                                 "  Searching Time: " + str("%.4f" % result2.exec_time) + "\n" +
                                 "  Average heuristic score: " + str("%.4f" % result2.avg_h) + "\n" +
                                 "  EBF: " + str("%.4f" % result2.ebf) + "\n" +
                                 "  Minimum depth: " + str("%.4f" % result2.min_d) + "\n" +
                                 "  Maximum depth: " + str("%.4f" % result2.max_d) + "\n" +
                                 "  Average depth: " + str("%.4f" % result2.avg_d) + "\n" +
                                 "\n")

    solutions_file.write("-------------------------------------------------------------------------------\n")
    solutions_file.close()
    return 0


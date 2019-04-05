from ai_lab1 import GameState


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



class Car:

    def __init__(self, name, length, start_pos, end_pos, isVertical):
        self.name = name;
        self.length = length;
        self.start_pos = start_pos;  # position: [i,j]
        self.end_pos = end_pos;  # position: [i,j]
        self.isVertical = isVertical;

    def move(self, direction_str, steps):
        if direction_str == 'UP':
            if not self.isVertical:
                print("Error: can't move up with horizontal car");
                return 0;
            else:
                self.start_pos[0] += 1;  # [i,j] -> [i+1,j]
                self.end_pos[0] += 1;  # [i,j] -> [i+1,j]
                return 1;

        elif direction_str == 'DOWN':
            if not self.isVertical:
                print("Error: can't move down with horizontal car");
                return 0;
            else:
                self.start_pos[0] -= 1;  # [i,j] -> [i-1,j]
                self.end_pos[0] -= 1;  # [i,j] -> [i-1,j]
                return 1;

        elif direction_str == 'RIGHT':
            if self.isVertical:
                print("Error: cant move right with vertical car");
                return 0;
            else:
                self.start_pos[1] += 1;  # [i,j] -> [i,j+1]
                self.end_pos[1] += 1;  # [i,j] -> [i,j+1]
                return 1;

        elif direction_str == 'LEFT':
            if self.isVertical:
                print("Error: cant move left with vertical car");
                return 0;
            else:
                self.start_pos[1] -= 1;  # [i,j] -> [i,j-1]
                self.end_pos[1] -= 1;  # [i,j] -> [i,j-1]
                return 1;

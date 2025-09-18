import random
from collections import deque
import copy

GOAL_STATE = [[1, 2, 3],
              [4, 5, 6],
              [7, 8, 0]]  # 0 represents the blank tile

MOVES = {
    'Up': (-1, 0),
    'Down': (1, 0),
    'Left': (0, -1),
    'Right': (0, 1)
}

# Node class to track the state, path, and depth
class Node:
    def __init__(self, state, parent=None, move=None, depth=0):
        self.state = state
        self.parent = parent
        self.move = move
        self.depth = depth

    def __eq__(self, other):
        return self.state == other.state

    def __hash__(self):
        return hash(str(self.state))

def is_solvable(state):
    flat_state = sum(state, [])
    inv_count = 0
    for i in range(8):
        for j in range(i+1, 9):
            if flat_state[i] != 0 and flat_state[j] != 0 and flat_state[i] > flat_state[j]:
                inv_count += 1
    return inv_count % 2 == 0

def generate_random_state():
    state = list(range(9))
    while True:
        random.shuffle(state)
        board = [state[i:i+3] for i in range(0, 9, 3)]
        if is_solvable(board):
            return board

def get_blank_position(state):
    for i in range(3):
        for j in range(3):
            if state[i][j] == 0:
                return i, j

def move_blank(state, direction):
    x, y = get_blank_position(state)
    dx, dy = MOVES[direction]
    new_x, new_y = x + dx, y + dy
    if 0 <= new_x < 3 and 0 <= new_y < 3:
        new_state = copy.deepcopy(state)
        new_state[x][y], new_state[new_x][new_y] = new_state[new_x][new_y], new_state[x][y]
        return new_state
    return None

def goal_test(state):
    return state == GOAL_STATE

def depth_limited_search(start_node, limit, nodes_expanded):
    stack = deque([start_node])
    visited = set()

    while stack:
        node = stack.pop()
        nodes_expanded[0] += 1
        visited.add(str(node.state))

        if goal_test(node.state):
            return node

        if node.depth < limit:
            for move in MOVES:
                
                new_state = move_blank(node.state, move)
                if new_state and str(new_state) not in visited:
                    child = Node(new_state, node, move, node.depth + 1)
                    stack.append(child)
    return None

def iterative_deepening_search(initial_state):
    depth = 0
    nodes_expanded = [0]

    while True:
        start_node = Node(initial_state)
        result = depth_limited_search(start_node, depth, nodes_expanded)
        if result:
            return result, nodes_expanded[0]
        depth += 1

def reconstruct_path(goal_node):
    path = []
    current = goal_node
    while current.parent:
        path.append(current.move)
        current = current.parent
    path.reverse()
    return path

def print_state(state):
    for row in state:
        print(" ".join(str(x) if x != 0 else " " for x in row))
    print()

def main():
    print("Generating a solvable puzzle...")
    initial_state = generate_random_state()
    print("Initial State:")
    print_state(initial_state)

    goal_node, nodes_expanded = iterative_deepening_search(initial_state)
    moves = reconstruct_path(goal_node)

    print("Goal State Reached!")
    print("Moves to reach goal:")
    print(" -> ".join(moves))
    print("\nTotal Moves:", len(moves))
    print("Nodes Expanded:", nodes_expanded)

if __name__ == "__main__":
    main()

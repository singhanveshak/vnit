#IMPLEMENTATION OF TRAVELLING SALESMAN PROBLEM
import heapq
import math
import random
import numpy as np

# A small value to represent infinity
INF = float('inf')

def create_random_graph(num_nodes, max_weight=100):
    """Generates a complete, symmetric distance matrix for a given number of nodes."""
    np.random.seed(42)  # For reproducibility
    matrix = np.zeros((num_nodes, num_nodes))
    for i in range(num_nodes):
        for j in range(i + 1, num_nodes):
            weight = random.randint(1, max_weight)
            matrix[i][j] = weight
            matrix[j][i] = weight
    return matrix

def prim_mst(graph_adj_matrix, nodes):
    """
    Calculates the MST cost for a given set of nodes using Prim's algorithm.
    This is the core of the MST heuristic.
    """
    if not nodes:
        return 0

    # Map original node indices to a compact 0-based list for the algorithm
    node_map = {orig_node: i for i, orig_node in enumerate(nodes)}
    
    # Prim's algorithm
    num_nodes = len(nodes)
    start_node_idx = 0  # Start with the first node in the list
    visited = [False] * num_nodes
    min_cost = [INF] * num_nodes
    min_cost[start_node_idx] = 0
    total_mst_cost = 0

    pq = [(0, start_node_idx)]  # (cost, node_index)

    while pq:
        cost, u_idx = heapq.heappop(pq)
        
        if visited[u_idx]:
            continue
        
        visited[u_idx] = True
        total_mst_cost += cost
        
        for v_idx in range(num_nodes):
            if not visited[v_idx]:
                original_u = nodes[u_idx]
                original_v = nodes[v_idx]
                edge_weight = graph_adj_matrix[original_u][original_v]
                if edge_weight < min_cost[v_idx]:
                    min_cost[v_idx] = edge_weight
                    heapq.heappush(pq, (edge_weight, v_idx))
                    
    return total_mst_cost

def tsp_a_star(graph_adj_matrix):
    """
    Solves the TSP using the A* search algorithm with the MST heuristic.
    
    State representation: (cost_so_far, current_node, visited_mask, path)
    - cost_so_far: 'g' value
    - current_node: The last node added to the path
    - visited_mask: A bitmask representing visited nodes
    - path: A list of nodes visited so far (for reconstruction)
    
    The heuristic `h` is calculated as the cost of the MST of all unvisited nodes
    plus the cost of connecting the current node to this MST and the MST to the start node.
    """
    num_nodes = len(graph_adj_matrix)
    
    # Priority queue stores tuples: (f_score, g_score, current_node, visited_mask, path)
    # The f_score is the priority
    pq = []
    
    # Initial state: starting at node 0, visited mask is 1 (for node 0)
    start_node = 0
    start_mask = 1 << start_node
    
    # The 'g' score for the start node is 0
    g_start = 0
    
    # MST heuristic for the start node (all nodes are unvisited except start)
    remaining_nodes = list(range(1, num_nodes))
    h_start = prim_mst(graph_adj_matrix, remaining_nodes)
    
    # The path for the start state is just the start node
    path_start = [start_node]
    
    # Push the initial state to the priority queue
    heapq.heappush(pq, (g_start + h_start, g_start, start_node, start_mask, path_start))
    
    # Use a dictionary to store the best g_score for each state (current_node, visited_mask)
    # This prevents redundant computations and cycles
    min_g_scores = {(start_node, start_mask): 0}
    
    # Main A* search loop
    while pq:
        f_score, g_score, current_node, visited_mask, path = heapq.heappop(pq)
        
        # Check if this state has been visited with a better path
        if g_score > min_g_scores.get((current_node, visited_mask), INF):
            continue
            
        # Goal check: If all nodes are visited
        if visited_mask == (1 << num_nodes) - 1:
            # Add the final edge from the last city back to the start
            final_cost = g_score + graph_adj_matrix[current_node][start_node]
            final_path = path + [start_node]
            return final_path, final_cost

        # Explore neighbors (all unvisited nodes)
        for next_node in range(num_nodes):
            if not (visited_mask & (1 << next_node)):
                # Calculate the new path and g_score
                new_g = g_score + graph_adj_matrix[current_node][next_node]
                new_path = path + [next_node]
                new_mask = visited_mask | (1 << next_node)
                
                # Check if this new path is better than any previously found path to this state
                if new_g < min_g_scores.get((next_node, new_mask), INF):
                    min_g_scores[(next_node, new_mask)] = new_g
                    
                    # Calculate the heuristic h for the new state
                    remaining_nodes = []
                    for i in range(num_nodes):
                        if not (new_mask & (1 << i)):
                            remaining_nodes.append(i)
                    
                    h_score = prim_mst(graph_adj_matrix, remaining_nodes)
                    
                    # Connect to the unvisited nodes from the new current node and back to the start
                    if remaining_nodes:
                        h_score += min(graph_adj_matrix[next_node][node] for node in remaining_nodes)
                        h_score += min(graph_adj_matrix[start_node][node] for node in remaining_nodes)

                    new_f = new_g + h_score
                    
                    # Add the new state to the priority queue
                    heapq.heappush(pq, (new_f, new_g, next_node, new_mask, new_path))
    
    return None, INF # No solution found

if __name__ == "__main__":
    # Define a small number of nodes for a feasible test
    num_nodes = 8
    
    print(f"Generating a random complete graph with {num_nodes} nodes...")
    graph = create_random_graph(num_nodes)
    
    print("\nAdjacency Matrix:")
    print(graph)

    print("\nRunning A* search with MST heuristic...")
    
    # Run the A* algorithm
    optimal_path, min_cost = tsp_a_star(graph)

    if optimal_path:
        print("\nOptimal Path Found:")
        path_str = " -> ".join(map(str, optimal_path))
        print(f"Path: {path_str}")
        print(f"Total Cost: {min_cost}")
    else:
        print("\nNo solution found.")

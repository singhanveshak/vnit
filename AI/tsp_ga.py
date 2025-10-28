#!/usr/bin/env python3
import math, random, time
from typing import List, Tuple
import matplotlib.pyplot as plt
random.seed(42)

def euclidean(a: Tuple[float,float], b: Tuple[float,float]) -> float:
    return math.hypot(a[0]-b[0], a[1]-b[1])

def total_distance(tour: List[int], cities: List[Tuple[float,float]]) -> float:
    d = 0.0
    n = len(tour)
    for i in range(n):
        d += euclidean(cities[tour[i]], cities[tour[(i+1)%n]])
    return d

def create_random_tour(n: int) -> List[int]:
    tour = list(range(n))
    random.shuffle(tour)
    return tour

def initial_population(pop_size: int, n_cities: int) -> List[List[int]]:
    return [create_random_tour(n_cities) for _ in range(pop_size)]

def tournament_selection(pop: List[List[int]], cities: List[Tuple[float,float]], k=5) -> List[int]:
    selected = random.sample(pop, k)
    selected.sort(key=lambda t: total_distance(t, cities))
    return selected[0][:]

def ordered_crossover(parent1: List[int], parent2: List[int]) -> Tuple[List[int], List[int]]:
    n = len(parent1)
    a, b = sorted(random.sample(range(n), 2))
    child1 = [-1]*n
    child2 = [-1]*n
    child1[a:b+1] = parent1[a:b+1]
    child2[a:b+1] = parent2[a:b+1]
    def fill(child, donor):
        n = len(child)
        donor_iter = (x for x in donor if x not in child)
        for i in range(n):
            idx = (b+1 + i) % n
            if child[idx] == -1:
                try:
                    child[idx] = next(donor_iter)
                except StopIteration:
                    break
    fill(child1, parent2)
    fill(child2, parent1)
    return child1, child2

def swap_mutation(tour: List[int], mutation_rate: float) -> None:
    n = len(tour)
    for i in range(n):
        if random.random() < mutation_rate:
            j = random.randrange(n)
            tour[i], tour[j] = tour[j], tour[i]

def run_ga(cities, pop_size=200, generations=200, tournament_k=5, mutation_rate=0.02, elite_size=2, verbose=False):
    n = len(cities)
    pop = initial_population(pop_size, n)
    best_history = []
    start = time.time()
    for gen in range(generations):
        pop.sort(key=lambda t: total_distance(t, cities))
        best = pop[0]
        best_dist = total_distance(best, cities)
        best_history.append(best_dist)
        if verbose and (gen%10==0 or gen==generations-1):
            print(f"Gen {gen:4d}: best distance = {best_dist:.4f}")
        new_pop = [pop[i][:] for i in range(elite_size)]
        while len(new_pop) < pop_size:
            p1 = tournament_selection(pop, cities, k=tournament_k)
            p2 = tournament_selection(pop, cities, k=tournament_k)
            if p1 == p2:
                p2 = tournament_selection(pop, cities, k=tournament_k)
            c1, c2 = ordered_crossover(p1, p2)
            swap_mutation(c1, mutation_rate)
            swap_mutation(c2, mutation_rate)
            new_pop.append(c1)
            if len(new_pop) < pop_size:
                new_pop.append(c2)
        pop = new_pop
    duration = time.time()-start
    pop.sort(key=lambda t: total_distance(t, cities))
    best = pop[0]
    best_dist = total_distance(best, cities)
    return {"best_tour": best, "best_distance": best_dist, "history": best_history, "duration": duration}

def generate_cities(n=80, scale=1000):
    return [(random.random()*scale, random.random()*scale) for _ in range(n)]

if __name__ == "__main__":
    cities = generate_cities(80, scale=1000)
    res = run_ga(cities, pop_size=200, generations=200, tournament_k=5, mutation_rate=0.02, elite_size=4, verbose=True)
    print("Best distance:", res["best_distance"], "time:", res["duration"])
    tour = res["best_tour"]
    xs = [cities[i][0] for i in tour] + [cities[tour[0]][0]]
    ys = [cities[i][1] for i in tour] + [cities[tour[0]][1]]
    plt.figure(figsize=(8,6))
    plt.plot(xs, ys, marker='o')
    plt.title(f"Best TSP tour — distance {res['best_distance']:.2f}")
    plt.tight_layout()
    plt.savefig("tsp_best_tour.png")
    plt.figure(figsize=(8,4))
    plt.plot(res["history"])
    plt.title("GA convergence (best distance per generation)")
    plt.tight_layout()
    plt.savefig("tsp_convergence.png")
    print("Saved plots: tsp_best_tour.png, tsp_convergence.png")

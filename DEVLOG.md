# Development Log - AiZO-Graphs

## About the project

**Topic:** Graph algorithms

**Start date:** 2026-04-24

**End date:** (In progress)

**Author:** Michał Pazurek

**Supervisor:** mgr inż. Damian Mroziński

---

### Icon legend

- [❌] Not finished
- [⚙️] In progress
- [⏩] Finished after "deadline"
- [✅] Finished

## 2026-04-27/28 - The initial idea and graph representations

### The plan

This time make it look professional at least when it comes to the API and try to follow the Single Responsibility Principle because last time (especially at the end) it was a chaos.

### Tasks

- [✅] Implement the incidence matrix representation
- [✅] Implement the adjacency list representation
- [✅] Implement a file parser (better than last time)
- [✅] Implement a unified logging system (Singleton class?? - last time it was a mess) (ref: [Singleton design patterns](https://www.geeksforgeeks.org/system-design/singleton-pattern-c-design-patterns/))

---

## 2026-04-29 - Structures for algorithms

### Plan for the structures

Basically in order to actually run the algorithms we have to implement some structures that will be used inside them. For now we need a priority queue, a standard queue and later an array and a stack?? It seems like that

### Tasks (probably for more days)

- [✅] Implement a generic data structure API (this time lets split the structures into simple ones and indexable ones)
- [✅] Implement a queue
- [✅] Implement a priority queue
- [❌] Fix the priority queue (it seems it's not optimal because I understood the Floyd algorithm wrong - it's not meant to be used when inserting data)
- [⏩] Implement an array

---

## 2026-05-10 - Dynamic array

### Tasks

- [✅] Implement dynamic array
- [✅] Incorporated the parameter library

---

## 2026-05-11 - Graph generation

### Tasks

- [✅] Added edge calculations based on the requested density
- [✅] Implement directed graph generation
- [✅] Implement undirected graph generation
- [✅] Fixed bugs with chechEdge method for undirected graphs

---

## 2026-05-12 - Single File Mode

### Tasks

- [⚙️] Implement Single File mode support
- [✅] Implement dynamic graph representation creation
- [✅] Change the generator to allow filling up multiple representations at once 

---

## 2026-05-12 - First algorithms

### Tasks

- [✅] Implement Pair class
- [✅] Implement a standarized algorithm interface
- [⏩] Implement Prim's MST algorithm

---

## 2026-05-20 - Finish MST Algorithms

### Tasks

- [✅] Implement Prim's MST algorithm
- [✅] Implement Merge Sort for Kruskal' MST
- [✅] Implement Kruskal's MST algorithm

---

## 2026-05-21 - Implement SP algorithms

### Tasks

- [✅] Implement Dijkstra's shortest path algorithm
- [✅] Implement Bellman-Ford's shortest path algorithm
- [❌] Maybe change the incidence matrix to support negative weights (it holds 1 or -1 and stores weights in a separate array)?

---

## 2026-05-23 - Maximum Flow algorithm

### Tasks

- [⏩] Implement Ford-Fulkerson algorithm

---

## 2026-05-25 - Result output

### Tasks

- [✅] Finish Ford-Fulkerson algorithm
- [⚙️] Define a standard result output structure
- [⚙️] Add result printing to a file
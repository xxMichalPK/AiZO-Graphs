# Algorithms and Computational Complexity - Project 2
## Graph Algorithms

## 📌 Project Overview

This project is part of the *Algorithms and Computational Complexity* course at Wrocław University of Science and Technology.  
The goal is to implement and analyze different graph algorithms with a focus on correctness and experimental evaluation.

### 🎯 Project Goals

- familiarize with different graph algorithms
- implement and analyze graph algorithms
- learn basic scripting to speed up experiments
- learn wise project management
- learn how to use and integrate already available datasets to test algorithms

### 🛠️ Technologies Used

- Version control system: **Git + GitHub**
- Build system: **CMake**
- Programming language: **C++17**

## ⚙️ Building the project

### 🔧 Standard Build

To configure and build the project:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

## ▶️ Running the Project

After building, the executable will be located in the `build` directory.

You can run it with:

```sh
./AiZO-Graphs --help
```

to display the help message.

## 📊 Features

### Graph algorithms

- Prim's algorithm
- Kruskal's algorithm
- Dijkstra's algorithm
- Bellman-Ford algorithm
- Ford-Fulkerson algorithm

### Graph representations

- Incidence matrix
- Adjacency list

### Working modes

- Single file mode - you give the program a file with the graph representation given in the format:

```
[num_vertices] [num_edges]
start_vertex1 end_vertex1 weight1
start_vertex2 end_vertex2 weight2
.
.
.
start_vertexN end_vertexN weightN
```

ATTENTION: the start vertices are numbered in order from 0 to num_vertices-1 (every vertex in this range must be represented in the file)

- Benchmark mode - you can run multiple tests on randomly generated graphs. The output will be a csv file with the results of the tests.

- help mode - it just displays the help message

## 👤 Author

- Name: **Michał Pazurek**
- Course: **Algorithms and Computational Complexity**
- University: **Wrocław University of Science and Technology**
- Repository: **[GitHub - AiZO-Graphs](https://github.com/xxMichalPK/AiZO-Graphs)**
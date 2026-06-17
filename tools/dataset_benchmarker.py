import subprocess
import time
import networkx as nx

executable_path = "./AiZO-Graphs"
output_path = "./output.tmp.txt"
results_file = "dataset_results.csv"
dataset_path = "./../datasets/USA-road-d.NY.csv"

# Use 50 iterations for each test case
iterations = 50

problem_set = {
    "MST": 0,
    "SP": 1,
    "MF": 2
}

with open(dataset_path, 'r') as f:
    first_line = f.readline().strip().split()

vertex_count = int(first_line[0])
edge_count = int(first_line[1])
density_directed = (edge_count / (vertex_count * (vertex_count - 1))) * 10000

def load_graph(path):
    G = nx.DiGraph()
    with open(path, "r") as f:
        for line in f:
            if not line.strip():
                continue

            parts = line.split()
            if len(parts) != 3:
                continue

            u, v, w = parts
            u = int(u)
            v = int(v)
            w = int(w)

            G.add_edge(u, v, capacity=w, weight=w)

    return G

def run_prog_benchmark():
    # algorithms = {
    #     "MST": {"PRIM": 1, "KRUSKAL": 2},
    #     "SP": {"DIJKSTRA": 3, "BELLMAN_FORD": 4},
    #     "MF": {"FORD_FULKERSON": 5}
    # }
    algorithms = {
        "MST": {"KRUSKAL": 2},
        "SP": {},
        "MF": {"FORD_FULKERSON": 5}
    }
    for problem_name, problem_id in problem_set.items():
        print(f"[ INFO ] Running {problem_name} with dataset {dataset_path}")
        for algorithm_name, algorithm_id in algorithms[problem_name].items():
            for i in range(iterations):
                command = f"{executable_path} -f -i {dataset_path} -p {problem_id} -a {algorithm_id} -s 2 -n {iterations} -o {output_path} -r {results_file} -c 0 -e {vertex_count - 1}"
                subprocess.run(command, shell=True)


def specific_benchmark(G, problem_name, algorithm):
    if problem_name == "MST":
        G_undirected = G.to_undirected()
        if algorithm == "PRIM":
            start_time = time.time_ns() / 1000 # Convert to microseconds
            mst = nx.minimum_spanning_tree(G_undirected, algorithm="prim", weight="weight")
            return mst.size(weight="weight"), (time.time_ns() / 1000) - start_time
        elif algorithm == "KRUSKAL":
            start_time = time.time_ns() / 1000 # Convert to microseconds
            mst = nx.minimum_spanning_tree(G_undirected, algorithm="kruskal", weight="weight")
            return mst.size(weight="weight"), (time.time_ns() / 1000) - start_time
    elif problem_name == "SP":
        source = 0
        target = vertex_count - 1
        if algorithm == "DIJKSTRA":
            start_time = time.time_ns() / 1000 # Convert to microseconds
            length = nx.dijkstra_path_length(G, source, target, weight="weight")
            return length, (time.time_ns() / 1000) - start_time
        elif algorithm == "BELLMAN_FORD":
            start_time = time.time_ns() / 1000 # Convert to microseconds
            length = nx.bellman_ford_path_length(G, source, target, weight="weight")
            return length, (time.time_ns() / 1000) - start_time
    elif problem_name == "MF":
        source = 0
        sink = vertex_count - 1
        if algorithm == "FORD_FULKERSON":
            start_time = time.time_ns() / 1000 # Convert to microseconds
            flow_value, _ = nx.maximum_flow(G, source, sink, capacity="capacity", flow_func=nx.algorithms.flow.edmonds_karp)
            return flow_value, (time.time_ns() / 1000) - start_time

def run_python_benchmark():
    # algorithms = {
    #     "MST": ["PRIM", "KRUSKAL"],
    #     "SP": ["DIJKSTRA", "BELLMAN_FORD"],
    #     "MF": ["FORD_FULKERSON"]
    # }
    algorithms = {
        "MST": ["KRUSKAL"],
        "SP": [],
        "MF": ["FORD_FULKERSON"]
    }
    G = load_graph(dataset_path)

    for problem_name, problem_id in problem_set.items():
        print(f"[ INFO ] Running {problem_name} with dataset {dataset_path}")
        for algorithm in algorithms[problem_name]:
            for i in range(iterations):
                res, duration = specific_benchmark(G, problem_name, algorithm)
                print(f"{problem_name} - {algorithm}: {int(res)} Finished in {int(duration)} microseconds")
                with open(results_file, 'a') as f:
                    current_date = time.strftime("%Y-%m-%d", time.localtime())
                    current_time = time.strftime("%H:%M:%S", time.localtime())
                    current_density = density_directed if problem_name != "MST" else density_directed * 2
                    f.write(f"{current_date},{current_time},NOT_APPLICABLE,NOT_APPLICABLE,NETWORKX_GRAPH,{algorithm},{int(vertex_count)},{int(current_density)},PYTHON_RUN,{int(duration)}us,,,\n")

def main():
    run_prog_benchmark()
    run_python_benchmark()

if __name__ == "__main__":
    main()
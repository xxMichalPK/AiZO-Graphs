import random
import argparse
import subprocess

def generate(vertex_count, edge_count, directed=False):
    if directed:
        return generate_directed(vertex_count, edge_count)
    else:
        return generate_undirected(vertex_count, edge_count)


def generate_directed(vertex_count, edge_count):
    if edge_count > vertex_count * (vertex_count - 1):
        raise ValueError("Too many edges for directed graph with given vertex count.")

    edges = []
    for i in range(vertex_count - 1):
        weight = random.randint(1, vertex_count * 4 // 5)
        edges.append((i, i + 1, weight))
    
    edges.append((vertex_count - 1, 0, random.randint(1, vertex_count * 4 // 5)))

    for _ in range(edge_count - vertex_count):
        u = random.randint(0, vertex_count - 1)
        v = random.randint(0, vertex_count - 1)
        while u == v or (u, v) in [(edge[0], edge[1]) for edge in edges]:
            u = random.randint(0, vertex_count - 1)
            v = random.randint(0, vertex_count - 1)
        
        weight = random.randint(1, vertex_count * 4 // 5)
        edges.append((u, v, weight))
    
    return edges
        

def generate_undirected(vertex_count, edge_count):
    if edge_count > vertex_count * (vertex_count - 1) // 2:
        raise ValueError("Too many edges for undirected graph with given vertex count.")

    edges = []
    edge_set = set()  # stores normalized edge pairs (min(u,v), max(u,v))

    free_vertices = list(range(vertex_count))
    used_vertices = []

    # Select starting vertex
    vertex = free_vertices.pop(random.randrange(len(free_vertices)))
    used_vertices.append(vertex)

    remaining_edge_count = edge_count

    # Build a spanning tree (guarantees connectivity)
    while len(used_vertices) < vertex_count:
        u = random.choice(used_vertices)
        v_idx = random.randrange(len(free_vertices))
        v = free_vertices[v_idx]

        edge = (min(u, v), max(u, v))
        if edge in edge_set:
            continue

        max_weight = max(1, int(edge_count * 0.8))
        weight = random.randint(1, max_weight)

        edges.append((u, v, weight))
        edge_set.add(edge)

        used_vertices.append(v)
        free_vertices.pop(v_idx)

        remaining_edge_count -= 1

    # Add remaining random edges
    while remaining_edge_count > 0:
        u = random.randrange(vertex_count)
        v = random.randrange(vertex_count)

        if u == v:
            continue

        edge = (min(u, v), max(u, v))
        if edge in edge_set:
            continue

        max_weight = max(1, int(edge_count * 0.8))
        weight = random.randint(1, max_weight)

        edges.append((u, v, weight))
        edge_set.add(edge)

        remaining_edge_count -= 1

    return edges

def test_python(graph_path, algorithm, source=None, end=None):
    exec_path = "./graph_tester.py"
    subprocess.run(["python3", exec_path, "-g", graph_path, "-a", algorithm, 
                    "-c", str(source) if source is not None else "0", "-e", str(end) if end is not None else "0"])

def test_cpp(graph_path, algorithm, source=None, end=None):
    exec_path = "./AiZO-Graphs"
    subprocess.run([exec_path, "-f", "-i", graph_path, "-o", "test_output.txt", "-a", "0", "-s", "0", "-p", "2",
                    "-c", str(source) if source is not None else "0", "-e", str(end) if end is not None else "0"])

def main():
    parser = argparse.ArgumentParser(description="Generate a random graph.")
    parser.add_argument("--vertex_count", "-v", type=int, help="Number of vertices in the graph.")
    parser.add_argument("--edge_count", "-e", type=int, help="Number of edges in the graph.")
    parser.add_argument("--directed", "-d", action="store_true", help="Generate a directed graph.")
    parser.add_argument("--output", "-o", type=str, help="Output file to save the generated graph.")
    parser.add_argument("--iterations", "-i", type=int, default=1, help="Number of iterations to run the tests.")
    args = parser.parse_args()

    for _ in range(args.iterations):
        edges = generate(args.vertex_count, args.edge_count, args.directed)
        edges.sort(key=lambda x: (x[0], x[1]))  # Sort edges
        
        with open(args.output, "w") as f:
            f.write(f"{args.vertex_count} {args.edge_count}\n")
            for u, v, weight in edges:
                f.write(f"{u} {v} {weight}\n")
            
        test_python(args.output, "ford-fulkerson", source=0, end=args.vertex_count - 1)
        test_cpp(args.output, "ford-fulkerson", source=0, end=args.vertex_count - 1)

if __name__ == "__main__":
    main()
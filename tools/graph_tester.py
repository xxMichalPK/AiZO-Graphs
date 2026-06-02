import networkx as nx
import argparse

def parse_arguments():
    parser = argparse.ArgumentParser(description="Test graph algorithms using a publically available graph library.")
    parser.add_argument("-g", "--graph", type=str, required=True, help="Path to the graph")
    parser.add_argument("-a", "--algorithm", type=str, required=True, help="Graph algorithm to test", choices=[
        "prim", "kruskal", "dijkstra", "bellman-ford", "ford-fulkerson"
    ])
    parser.add_argument("-c", "--source", type=int, help="Source node for path algorithms")
    parser.add_argument("-e", "--end", type=int, help="End node for path algorithms")
    return parser.parse_args()


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
            w = float(w)

            G.add_edge(u, v, capacity=w, weight=w)

    return G


def main():
    args = parse_arguments()
    graph = load_graph(args.graph)

    if len(graph.nodes) == 0:
        print("Empty graph")
        return

    if args.algorithm in ["prim", "kruskal"]:
        UG = graph.to_undirected()

        if args.algorithm == "prim":
            mst = nx.minimum_spanning_tree(UG, algorithm="prim", weight="weight")
        else:
            mst = nx.minimum_spanning_tree(UG, algorithm="kruskal", weight="weight")

        print("Total weight of MST:", mst.size(weight="weight"))
    elif args.algorithm == "dijkstra":
        source = args.source if args.source is not None else min(graph.nodes)
        target = args.end if args.end is not None else max(graph.nodes)
        length = nx.dijkstra_path_length(graph, source, target, weight="weight")
        print(f"Dijkstra {source} -> {target}: {length}")
    elif args.algorithm == "bellman-ford":
        source = args.source if args.source is not None else min(graph.nodes)
        target = args.end if args.end is not None else max(graph.nodes)
        length = nx.bellman_ford_path_length(graph, source, target, weight="weight")
        print(f"Bellman-Ford {source} -> {target}: {length}")
    elif args.algorithm == "ford-fulkerson":
        source = args.source if args.source is not None else min(graph.nodes)
        sink = args.end if args.end is not None else max(graph.nodes)
        flow_value, _ = nx.maximum_flow(graph, source, sink, capacity="capacity")
        print(f"Max flow {source} -> {sink}: {flow_value}")


if __name__ == "__main__":
    main()
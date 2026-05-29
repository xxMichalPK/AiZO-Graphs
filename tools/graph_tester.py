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


def main():
    args = parse_arguments()
    # Load the graph
    graph = nx.read_weighted_edgelist(args.graph, nodetype=int)
    
    if args.algorithm == "prim":
        mst = nx.minimum_spanning_tree(graph, algorithm="prim")
        print("Prim's MST:", mst.edges(data=True))
        print("Total weight of MST:", mst.size(weight='weight'))
    elif args.algorithm == "kruskal":
        mst = nx.minimum_spanning_tree(graph, algorithm="kruskal")
        print("Kruskal's MST:", mst.edges(data=True))
        print("Total weight of MST:", mst.size(weight='weight'))
    elif args.algorithm == "dijkstra":
        source = args.source if args.source is not None else 1
        end = args.end if args.end is not None else max(graph.nodes)
        length = nx.dijkstra_path_length(graph, source, end)
        print("Dijkstra's shortest path from node", source, "to node", end, ":", length)
    elif args.algorithm == "bellman-ford":
        source = args.source if args.source is not None else 1
        end = args.end if args.end is not None else 6004
        length = nx.bellman_ford_path_length(graph, source, end)
        print("Bellman-Ford shortest path from node", source, "to node", end, ":", length)
    elif args.algorithm == "ford-fulkerson":
        source = args.source if args.source is not None else 1
        sink = args.end if args.end is not None else max(graph.nodes)
        flow_value, flow_dict = nx.maximum_flow(graph, source, sink)
        print("Ford-Fulkerson max flow from node", source, "to node", sink, ":", flow_value)
        print("Flow dict:", flow_dict)


if __name__ == "__main__":
    main()
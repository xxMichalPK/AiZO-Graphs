#ifndef GRAPH_REPR_HPP
#define GRAPH_REPR_HPP

class GraphRepr {
    protected:
        GraphRepr() = default;
    
    public:
        virtual ~GraphRepr() = default;

        virtual void addEdge(int startVertex, int endVertex, int weight) = 0;
};

#endif // GRAPH_REPR_HPP
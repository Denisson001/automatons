#include "minimal_automaton.cpp"

typedef typename NMinimalAutomaton::TVertex TVertex;
typedef typename NMinimalAutomaton::TAlpha  TAlpha;
typedef unsigned int                          TEdge;

int main() {
    bool    logging = false;
    TVertex vertex_count;
    TVertex finish_count;
    TAlpha  alphabet_size;
    TEdge   edge_count;

#ifdef LOGGING
    logging = true;
#endif

    std::cin >> vertex_count >> alphabet_size >> finish_count;

    NAutomaton::TAutomaton automaton(vertex_count, alphabet_size);

    for (TVertex i = 0; i < finish_count; ++i) {
        TVertex finish;
        std::cin >> finish;
        automaton.addFinish(finish);
    }

    std::cin >> edge_count;

    for (TEdge i = 0; i < edge_count; ++i) {
        TVertex start, finish;
        TAlpha alpha;
        std::cin >> start >> finish >> alpha;
        automaton.addEdge(start, finish, alpha);
    }

    automaton.print();

    NMinimalAutomaton::TMinimalAutomaton minimal_automaton(automaton, logging);

    const auto result_automaton = minimal_automaton.getResultAutomaton();

    result_automaton.print();
}
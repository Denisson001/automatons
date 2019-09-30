#include "determine_automaton.cpp"

typedef typename NDetermineAutomaton::TVertex TVertex;
typedef typename NDetermineAutomaton::TAlpha  TAlpha;
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

    NDetermineAutomaton::TDetermineAutomaton determine(automaton, logging);

    const auto determined_automaton = determine.getResultAutomaton();

    determined_automaton.print();
}
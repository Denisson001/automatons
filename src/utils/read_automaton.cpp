#include "../headers/automaton.h"

/*  ===  Формат ввода автомата === 
 * В первой строке три числа - число состояний, размер алфавита, число завершающих состояний
 * Во второй строке - номера завершающих состояний
 * В третьей строке - число переходов
 * В остальных строках переходы в формате
 * (начальное состояние, конечное состояние, буква)
 * буква - число от 0 до (размер алфавита - 1)
 */

namespace NReadAutomaton {

typedef typename NAutomaton::TVertex TVertex;
typedef typename NAutomaton::TAlpha  TAlpha;
typedef unsigned int                 TEdge;

NAutomaton::TAutomaton read_automaton() {
    TVertex vertex_count;
    TVertex finish_count;
    TAlpha  alphabet_size;
    TEdge   edge_count;

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

    return automaton;
}

} // end of NReadAutomaton

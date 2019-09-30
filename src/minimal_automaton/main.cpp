#include "minimal_automaton.cpp"
#include "../utils/read_automaton.cpp"

int main() {
    bool logging = false;

#ifdef LOGGING
    logging = true;
#endif

    const auto automaton = read_automaton();

    automaton.print();

    NMinimalAutomaton::TMinimalAutomaton minimal_automaton(automaton, logging);

    const auto result_automaton = minimal_automaton.getResultAutomaton();

    result_automaton.print();
}

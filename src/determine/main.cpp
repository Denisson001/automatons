#include "determine_automaton.cpp"
#include "../utils/read_automaton.cpp"

int main() {
    bool logging = false;

#ifdef LOGGING
    logging = true;
#endif

    const auto automaton = NReadAutomaton::read_automaton();

    automaton.print();

    NDetermineAutomaton::TDetermineAutomaton determine(automaton, logging);

    const auto determined_automaton = determine.getResultAutomaton();

    determined_automaton.print();
}
#include "search_distinguishing_word.cpp"
#include "../utils/read_automaton.cpp"

int main() {
    const auto automaton1 = NReadAutomaton::read_automaton();
    const auto automaton2 = NReadAutomaton::read_automaton();

    NSearchDistinguishingWord::TSearchDistinguishingWord searcher(automaton1, automaton2);

    const auto word = searcher.getDistinguishingWord();

    if (word) {
        std::cout << "Distinguishing Word : ";
        for (const auto& alpha : *word) {
            std::cout << alpha;
        }
        std::cout << "\n";
    } else {
        std::cout << "Word not found\n";
    }
}
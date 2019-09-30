#include "automaton.h"
#include <unordered_map>
#include <stack>

// По НКА с однобуквенными переходами возвращает ДКА
// рассматривает только состояния, достижимые из стартового состояния

namespace NDetermineAutomaton {

typedef NAutomaton::TAutomaton TAutomaton;
typedef NAutomaton::TAlpha     TAlpha;
typedef NAutomaton::TVertex    TVertex;
typedef TVertex                TNode;

class TDetermineAutomaton {
public:
    /*
     * automaton - исходный НКА
     * logging   - флаг для включения логирования
     */
    TDetermineAutomaton(const TAutomaton& automaton, const bool logging) :
        automaton_(automaton),
        logging_(logging)
    {}

    /* Возвращает итоговый ДКА */
    TAutomaton getResultAutomaton() const {
        return determine_();
    }

private:
    typedef unsigned int TMask_;

    const bool        logging_;   // флаг для логирования
    const TAutomaton& automaton_; // исходный автомат

    /* Возвращает итоговый ДКА */
    TAutomaton determine_() const {
        TAutomaton result_automaton;
        result_automaton.increaseAlphabetSizeTo(automaton_.getAlphabetSize());
        addNodes_(result_automaton);
        return result_automaton;
    }

    /* Возвращает ноду, в которую ведут переходы по alpha из vertex (состояние в старом автомате) */
    TMask_ getVertexAlphaMask(TVertex vertex, TAlpha alpha) const {
        TMask_ result = 0;
        for (auto it = automaton_.getEdgeIterator(vertex); !it.isEnd(); ++it) {
            if (it.getAlpha() == alpha) {
                result ^= (TMask_)1 << it.getFinish();
            }
        }
        return result;
    }

    /* Проверяет является ли нода завершающим состоянием в новом автомате */
    bool isFinish_(TNode node) const {
        for (TVertex vertex = 0; vertex < automaton_.getVertexCount(); vertex++) { // проверяем есть ли в ноде
            if (automaton_.isFinish(vertex) && ((node >> vertex) & 1)) {           // завершающее состояние в старом автомате
                return true;
            }
        }
        return false;
    }

    /*
     * Добавляет ноды - подмножество исходных состояний - в новый автомат
     * Нода - битовая маска.
     */
    void addNodes_(TAutomaton& result_automaton) const {
        std::unordered_map<TMask_, TNode>      processed;         // мапа из обработанных нод в номер состояния в новом автомате
        std::stack< std::pair<TMask_, TNode> > nodes_in_process;  // стек еще необработанных нод
        TNode                                  nodes_count = 0;   // количество уже добавленных состояний в новый автомат

        // запускаем алгоритм  со стартового состояния
        TMask_ start_node = (TMask_)1 << automaton_.getStart();
        nodes_in_process.push({start_node, nodes_count++});
        processed[start_node] = 0;

        while(!nodes_in_process.empty()) {
            const auto [cur_node, cur_node_number] = nodes_in_process.top(); // cur_node        - текущая нода
                                                                             // cur_node_number - номер состояния в новом автомате
            nodes_in_process.pop();
            for (TAlpha alpha = 0; alpha < automaton_.getAlphabetSize(); ++alpha) { // перебираем букву для перехода
                TMask_ new_node = 0; // новая нода, в которую будет переход по alpha

                for (TVertex vertex = 0; vertex < automaton_.getVertexCount(); ++vertex) { // по каждому состоянию в ноде
                    if ((cur_node >> vertex) & 1) {                                        // берем все переходы по alpha
                        new_node |= getVertexAlphaMask(vertex, alpha);
                    }
                }

                if (new_node) {
                    if (processed.find(new_node) == processed.end()) { // если новая нода еще не была обработана
                        TNode new_node_number = nodes_count++;
                        processed[new_node] = new_node_number;
                        nodes_in_process.push({new_node, new_node_number}); // добавляем ее в стек
                        if (isFinish_(new_node)) { // проверяем является ли она завершающим состоянием в новом автомате
                            if (logging_) {
                                std::cout << "Finish : " << nodeToString(new_node) << "\n";
                            }
                            result_automaton.addFinish(new_node_number);
                        }
                    }
                    if (logging_) {
                        std::cout << "Edge   : " << nodeToString(cur_node) << ' ' << nodeToString(new_node) << ' ' << alpha << "\n";
                    }
                    result_automaton.addEdge(cur_node_number, processed[new_node], alpha); // добавляем переход в новом автомате
                }
            }
        }
    }

    /* Возвращает представление ноды в human-readable виде */
    std::string nodeToString(const TNode& node) const {
        std::string result;
        for (TVertex vertex = 0; vertex < automaton_.getVertexCount(); vertex++) {
            if ((node >> vertex) & 1) {
                if (result.size()) {
                    result += ',';
                }
                result += std::to_string(vertex);
            }
        }
        return "(" + result + ")";
    }
};

} // end of namespace NDetermineAutomaton

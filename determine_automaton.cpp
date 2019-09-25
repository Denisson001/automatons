#include "automaton.h"
#include <unordered_map>
#include <stack>

// возвращает ДКА
// рассматривает только состояния, достижимые из стартовой вершины

namespace NDetermineAutomaton {

typedef NAutomaton::TAutomaton TAutomaton;
typedef NAutomaton::TAlpha     TAlpha;
typedef NAutomaton::TVertex    TVertex;
typedef TVertex                TNode;

class TDetermineAutomaton {
public:
    TDetermineAutomaton(const TAutomaton& automaton) :
        automaton_(automaton)
    {}

    TAutomaton getResultAutomaton() const {
        return determine_();
    }

private:
    typedef unsigned int TMask_;

    const TAutomaton& automaton_;

    TAutomaton determine_() const {
        TAutomaton result_automaton;
        result_automaton.increaseAlphabetSizeTo(automaton_.getAlphabetSize());
        addNodes_(result_automaton);
        return result_automaton;
    }

    TMask_ getVertexAlphaMask(TVertex vertex, TAlpha alpha) const {
        TMask_ result = 0;
        for (auto it = automaton_.getEdgeIterator(vertex); !it.isEnd(); ++it) {
            if (it.getAlpha() == alpha) {
                result ^= (TMask_)1 << it.getFinish();
            }
        }
        return result;
    }

    void addNodes_(TAutomaton& result_automaton) const {
        std::unordered_map<TMask_, TNode>      processed;
        std::stack< std::pair<TMask_, TNode> > nodes_in_process;
        TNode nodes_count = 0;

        TMask_ start_node = (TMask_)1 << automaton_.getStart();
        nodes_in_process.push({start_node, nodes_count++});
        processed[start_node] = 0;

        while(!nodes_in_process.empty()) {
            const auto [cur_node, cur_node_number] = nodes_in_process.top();
            nodes_in_process.pop();
            for (TAlpha alpha = 0; alpha < automaton_.getAlphabetSize(); alpha++) {
                TMask_ new_node = 0;

                for (TVertex vertex = 0; vertex < automaton_.getVertexCount(); vertex++) {
                    if ((cur_node >> vertex) & 1) {
                        new_node |= getVertexAlphaMask(vertex, alpha);
                    }
                }

                if (new_node) {
                    if (processed.find(new_node) == processed.end()) {
                        processed[new_node] = nodes_count;
                        nodes_in_process.push({new_node, nodes_count++});
                    }

                    result_automaton.addEdge(cur_node_number, processed[new_node], alpha);
                }
            }
        }
    }
};

} // end of namespace NDetermineAutomaton

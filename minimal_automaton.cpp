#include "automaton.h"

#include <algorithm>
#include <unordered_set>

// возвращает минимальный автомат по ПДКА

namespace NMinimalAutomaton {

typedef NAutomaton::TAutomaton TAutomaton;
typedef NAutomaton::TAlpha     TAlpha;
typedef NAutomaton::TVertex    TVertex;
typedef TVertex                TVertexType;

class TMinimalAutomaton {
public:
    TMinimalAutomaton(const TAutomaton& automaton) :
        automaton_(automaton)
    {}

    TAutomaton getResultAutomaton() const {
        return build_minimal_automaton_();
    }

private:
    typedef unsigned int TMask_;

    const TAutomaton& automaton_;

    TAutomaton build_minimal_automaton_() const {
        TAutomaton result_automaton;
        result_automaton.increaseAlphabetSizeTo(automaton_.getAlphabetSize());
        addVertices_(result_automaton);
        return result_automaton;
    }

    void addVertices_(TAutomaton& automaton) const {
        std::vector<TVertexType>            vertex_type(automaton_.getVertexCount());
        std::vector< std::vector<TVertex> > types(2);
        std::vector< std::vector<TVertex> > graph(automaton_.getVertexCount(), std::vector<TVertex>(automaton_.getAlphabetSize()));

        for (TVertex vertex = 0; vertex < automaton_.getVertexCount(); vertex++) {
            for (auto it = automaton_.getEdgeIterator(vertex); !it.isEnd(); ++it) {
                graph[vertex][it.getAlpha()] = it.getFinish();
            }
            vertex_type[vertex] = (TVertex)automaton_.isFinish(vertex);
            types[vertex_type[vertex]].push_back(vertex);
        }

        for (TVertex iter = 1; ; iter++) {
            std::vector< std::vector<TVertex> > new_types;

            for (const auto& type : types) {
                std::vector< std::pair< std::vector<TVertexType>, TVertex> > types_vectors;

                for (const auto& vertex : type) {
                    std::vector<TVertexType> types_vector(automaton_.getAlphabetSize());

                    for (TAlpha alpha = 0; alpha < automaton_.getAlphabetSize(); alpha++) {
                        types_vector[alpha] = vertex_type[graph[vertex][alpha]];
                    }

                    types_vectors.emplace_back(std::make_pair( std::move(types_vector), vertex ));
                }

                std::sort(types_vectors.begin(), types_vectors.end());

                std::size_t index = 0;
                while(index < types_vectors.size()) {
                    auto new_index = index;

                    while(new_index + 1 < types_vectors.size() && types_vectors[index].first == types_vectors[new_index + 1].first) {
                        ++new_index;
                    }

                    TVertexType new_type_number = new_types.size();
                    std::vector<TVertex> new_type;
                    for (auto i = index; i <= new_index; i++) {
                        new_type.push_back(types_vectors[i].second);
                    }
                    new_types.emplace_back(std::move(new_type));

                    index = new_index + 1;
                }
            }

            if (new_types.size() == types.size()) {
                break;
            }
            types = std::move(new_types);
        }


        addEdges_(automaton, graph, vertex_type);
    }

    void addEdges_(TAutomaton& automaton, const std::vector< std::vector<TVertex> >& graph,
                                          const std::vector<TVertexType>& vertex_type) const {
        std::unordered_set<TVertexType> used_types;
        for (TVertex vertex = 0; vertex < automaton_.getVertexCount(); vertex++) {
            TVertexType type = vertex_type[vertex];
            if (used_types.find(type) == used_types.end()) {
                used_types.insert(type);
                for (TAlpha alpha = 0; alpha < automaton_.getAlphabetSize(); alpha++) {
                    automaton.addEdge(type, vertex_type[graph[vertex][alpha]], alpha);
                }
            }
            if (automaton_.isFinish(vertex)) {
                automaton.addFinish(type);
            }
        }
    }
};

} // end of namespace NMinimalAutomaton

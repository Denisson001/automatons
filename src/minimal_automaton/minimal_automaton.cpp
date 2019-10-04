#include "../headers/automaton.h"

#include <algorithm>
#include <unordered_set>

// возвращает МПДКА по ПДКА
// все состояния должны быть достижимы из стартового

namespace NMinimalAutomaton {

typedef NAutomaton::TAutomaton TAutomaton;
typedef NAutomaton::TAlpha     TAlpha;
typedef NAutomaton::TVertex    TVertex;

class TMinimalAutomaton {
public:
    /*
     * automaton - исходный ПДКА
     * logging   - флаг для включения логирования
     */
    TMinimalAutomaton(const TAutomaton& automaton, const bool logging) :
        automaton_(automaton),
        logging_(logging)
    {}

    /* Возвращает итоговый МПДКА */
    TAutomaton getResultAutomaton() const {
        return buildMinimalAutomaton_();
    }

private:
    typedef TVertex TVertexType_;

    const bool        logging_;   // флаг для логирования
    const TAutomaton& automaton_; // исходный автомат

    /* Возвращает итоговый МПДКА */
    TAutomaton buildMinimalAutomaton_() const {
        TAutomaton result_automaton;
        result_automaton.increaseAlphabetSizeTo(automaton_.getAlphabetSize());
        addVertices_(result_automaton);
        return result_automaton;
    }

    /*
     * Добавляет состояния в итоговый автомат;
     * Итеративный алгоритм;
     * На каждом шаге поддерживаем классы эквивалентности на состояниях исходного автомата
     */
    void addVertices_(TAutomaton& automaton) const {
        TVertex                             vertex_count  = automaton_.getVertexCount();
        TAlpha                              alphabet_size = automaton_.getAlphabetSize();
        std::vector<TVertexType_>           vertex_type(vertex_count); // номер класса эквивалентности состояния в исходном автомате
        std::vector< std::vector<TVertex> > types(2);                  // список списоков эквивалентных состояний
        std::vector< std::vector<TVertex> > graph(vertex_count, std::vector<TVertex>(alphabet_size)); // исходный автомат в виде графа

        for (TVertex vertex = 0; vertex < vertex_count; ++vertex) {
            for (auto it = automaton_.getEdgeIterator(vertex); !it.isEnd(); ++it) { // заполняем graph
                graph[vertex][it.getAlpha()] = it.getFinish();
            }
            vertex_type[vertex] = (TVertex)automaton_.isFinish(vertex); // определяем изначальные классы эквивалентности
            types[vertex_type[vertex]].push_back(vertex);               // два класса - завершающие и незавершающие
        }

        for (TVertex iter = 1; ; iter++) {
            if (logging_) {
                std::cout << "iter : " << iter - 1 << "\n";
                for (TVertex vertex = 0; vertex < vertex_count; ++vertex) {
                    std::cout << vertex << " : " << vertex_type[vertex] << " ->";
                    for (TAlpha alpha = 0; alpha < alphabet_size; ++alpha) {
                        std::cout << ' ' << vertex_type[graph[vertex][alpha]];
                    }
                    std::cout << "\n";
                }
            }

            std::vector< std::vector<TVertex> > new_types; // новые классы эквивалентности

            for (const auto& type : types) { // берем один класс эквивалентности
                std::vector< std::pair< std::vector<TVertexType_>, TVertex> > types_vectors; // список для векторов классов эквивалентности
                                                                                            // для каждого состояния

                for (const auto& vertex : type) {
                    std::vector<TVertexType_> types_vector(alphabet_size); // вектор классов эквивалентности, в которые ведут переходы
                                                                          // по каждой букве из одного состояния

                    for (TAlpha alpha = 0; alpha < alphabet_size; ++alpha) {
                        types_vector[alpha] = vertex_type[graph[vertex][alpha]];
                    }

                    types_vectors.emplace_back(std::make_pair( std::move(types_vector), vertex ));
                }

                std::sort(types_vectors.begin(), types_vectors.end()); // после сортировки состояния из одного нового класса эквивалентности идут подряд

                std::size_t index = 0;
                while(index < types_vectors.size()) {
                    auto new_index = index;

                    while(new_index + 1 < types_vectors.size() && types_vectors[index].first == types_vectors[new_index + 1].first) {
                        ++new_index; // выделяем новый класс эквивалентности
                                     // смотрим на равенство векторов, состоящих из старых классов эквивалентности
                                     // в которые ведут переходы по каждой из букв
                    }

                    std::vector<TVertex> new_type; // новый класс эквивалентности
                    for (auto i = index; i <= new_index; ++i) {
                        new_type.push_back(types_vectors[i].second);
                    }
                    new_types.emplace_back(std::move(new_type));

                    index = new_index + 1;
                }
            }

            if (new_types.size() == types.size()) { // если классы эквивалентности не измельчились, то алгоритм можно завершать
                break;
            }

            types = std::move(new_types);

            for (std::size_t type_number = 0; type_number < types.size(); ++type_number) { // меняем классы эквивалентности состояний
                const auto& type = types[type_number];
                for (const auto& vertex : type) {
                    vertex_type[vertex] = type_number;
                }
            }
        }

        addEdges_(automaton, graph, vertex_type);
    	
	automaton.setStart(vertex_type[automaton_.getStart()]);
    }

    /* Добавляет переходы в новый автомат */
    void addEdges_(TAutomaton& automaton, const std::vector< std::vector<TVertex> >& graph,
                                          const std::vector<TVertexType_>& vertex_type) const {
        std::unordered_set<TVertexType_> used_types; // сет уже рассмотренных классов эквивалентности
        for (TVertex vertex = 0; vertex < automaton_.getVertexCount(); ++vertex) {
            TVertexType_ type = vertex_type[vertex]; // класс эквивалентности состояния из старого автомата
            if (used_types.find(type) == used_types.end()) { // если еще не был рассмотрен
                used_types.insert(type);
                for (TAlpha alpha = 0; alpha < automaton_.getAlphabetSize(); ++alpha) { // добавляем переходы
                    automaton.addEdge(type, vertex_type[graph[vertex][alpha]], alpha);
                    std::cout << vertex << ' ' << alpha << ' ' << type << ' ' << graph[vertex][alpha]
					<< ' '<< vertex_type[graph[vertex][alpha]]  << "\n";
		}
            }
            if (automaton_.isFinish(vertex)) { // проверяем является ли состояние в новом автомате завершающим
                automaton.addFinish(type);
            }
        }
    }
};

} // end of namespace NMinimalAutomaton

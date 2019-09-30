#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <optional>


namespace NAutomaton {

typedef unsigned int TVertex;
typedef unsigned int TAlpha;

class TAutomaton {
private:
    struct TEdge_;

public:
    /* Предоставляет доступ извне к переходам автомата */
    class TEdgeIterator {
    friend class TAutomaton;

    public:
        /* Возвращает конечное состояние перехода */
        TVertex getFinish() const {
            return getEdge_().finish;
        }

        /* Возвращает букву перехода */
        TAlpha getAlpha() const {
            return getEdge_().alpha;
        }

        TEdgeIterator& operator++() {
            if (edge_num_ < automaton_->graph_[vertex_].size()) {
                ++edge_num_;
            } else {
                throw std::out_of_range("Iterator out of range");
            }
            return *this;
        }

        bool isEnd() const {
            return edge_num_ == automaton_->graph_[vertex_].size();
        }

    private:
        typedef unsigned int TEdgeNum_;

        const TAutomaton* automaton_;
        TVertex           vertex_;
        TEdgeNum_         edge_num_;

        TEdgeIterator(const TAutomaton* automaton, TVertex vertex) :
                automaton_(automaton),
                vertex_(vertex),
                edge_num_(0)
        {}

        const TEdge_& getEdge_() const {
            if (isEnd()) {
                throw std::out_of_range("Iterator out of range");
            }
            return automaton_->graph_[vertex_][edge_num_];
        }
    };

    /* Возвращает итератор на переходы из вершины vertex */
    TEdgeIterator getEdgeIterator(TVertex vertex) const {
        return TEdgeIterator(this, vertex);
    }

    /*
     * Изначально можно указать размер автомата и алфавита;
     * По ходу добавления переходов размер может автоматически изменяться;
     * Изначально стартовое состояние имеет номер 0
     */
    TAutomaton(TVertex vertex_count = 1, TAlpha alphabet_size = 0) :
        vertex_count_(vertex_count),
        alphabet_size_(alphabet_size),
        start_(0)
    {
        resize_(vertex_count);
    }

    /* Добавляет переход между состояниями по букве */
    void addEdge(TVertex start, TVertex finish, TAlpha alpha) {
        TVertex max_vertex_number = std::max(start, finish);
        resize_(max_vertex_number + 1);
        graph_[start].emplace_back(finish, alpha);
        alphabet_size_ = std::max(alphabet_size_, alpha);
    }

    /* Проверяет является ли состояние завершающим */
    bool isFinish(TVertex vertex) const {
        if (vertex >= vertex_count_) {
            throw std::out_of_range("Vertex number is too large");
        }
        return is_finish_[vertex];
    }

    /* Помечает состояние как завершающее */
    void addFinish(TVertex vertex) {
        resize_(vertex + 1);
        is_finish_[vertex] = true;
    }

    /* Изменяет стартовую вершину */
    void setStart(TVertex start) {
        resize_(start + 1);
        start_ = start;
    }

    /* Увеличивает размер алфавита */
    void increaseAlphabetSizeTo(TAlpha new_alphabet_size) {
        if (new_alphabet_size < alphabet_size_) {
            throw std::logic_error("New alphabet size is smaller than current alphabet size");
        }
        alphabet_size_ = new_alphabet_size;
    }

    /* Возвращает номер стартового состояния */
    TVertex getStart() const {
        return start_;
    }

    /* Возвращает количество состояний в автомате */
    TVertex getVertexCount() const {
        return vertex_count_;
    }

    /* Возвращает размер алфавита */
    TAlpha getAlphabetSize() const {
        return alphabet_size_;
    }

    /*
     * Проверяет принимается ли слово автоматом;
     * Слово передается как список букв;
     * Корректно работает только для ДКА
     * */
    bool isAcceptable(const std::vector<TAlpha>& word_vector) const {
        TVertex cur_vertex = start_;
        for (const auto& alpha : word_vector) {
            const auto next_vertex = getNextVertex_(cur_vertex, alpha);
            if (!next_vertex) {
                return false;
            }
            cur_vertex = *next_vertex;
        }
        return is_finish_[cur_vertex];
    }

    /* Выводит автомат в human-readable виде */
    void print() const {
        std::cout << "Vertex count  : " << vertex_count_ << "\n";
        std::cout << "Alphabet size : " << alphabet_size_ << "\n";

        std::cout << "Finishes      : ";
        for (TVertex vertex = 0; vertex < vertex_count_; ++vertex) {
            if (is_finish_[vertex]) {
                std::cout << vertex << ' ';
            }
        }
        std::cout << "\n";

        std::cout << "Edges\n";
        std::cout << "Vertex Finish Alpha :\n";
        for (TVertex vertex = 0; vertex < vertex_count_; ++vertex) {
            for (const auto edge : graph_[vertex]) {
                std::cout << vertex << ' ' << edge.finish << ' ' << edge.alpha << "\n";
            }
        }
        std::cout << "\n";
    }

private:
    /* Внутренее представление переходов */
    struct TEdge_ {
        TVertex finish;
        TAlpha  alpha;
        TEdge_(TVertex finish, TAlpha alpha) :
            finish(finish),
            alpha(alpha)
        {}
    };

    std::vector< std::vector<TEdge_> > graph_; // граф переходов
    TVertex            vertex_count_;          // число состояний
    TVertex            start_;                 // номер стартового состояния
    TAlpha             alphabet_size_;         // размер алфавита
    std::vector<bool>  is_finish_;             // флажки завершающих состояний

    /* Изменяет размер автомата */
    void resize_(TVertex vertex) {
        if (vertex >= vertex_count_) {
            vertex_count_ = vertex;
            graph_.resize(vertex_count_);
            is_finish_.resize(vertex_count_, false);
        }
    }

    /*
     * Возвращает любое состояние, в которое есть переход из vertex по alpha;
     * Если такого перехода нет, возвращает null
     * */
    std::optional<TVertex> getNextVertex_(TVertex vertex, TAlpha alpha) const {
        for (auto it = getEdgeIterator(vertex); !it.isEnd(); ++it) {
            if (it.getAlpha() == alpha) {
                return it.getFinish();
            }
        }
        return std::nullopt;
    }
};

} // end of namespace NAutomata
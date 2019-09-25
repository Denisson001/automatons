#pragma once

#include <vector>
#include <stdexcept>


namespace NAutomaton {

typedef unsigned int  TVertex;
typedef unsigned char TAlpha;

class TAutomaton {
private:
    struct TEdge_;

public:
    class TEdgeIterator {
    friend class TAutomaton;

    public:

        TVertex getFinish() const {
            return getEdge_().alpha;
        }

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

    TEdgeIterator getEdgeIterator(TVertex vertex) const {
        return TEdgeIterator(this, vertex);
    }

    TAutomaton(TVertex vertex_count = 1, TAlpha alphabet_size = 0) :
        vertex_count_(vertex_count),
        alphabet_size_(alphabet_size),
        start_(0)
    {
        resize_(vertex_count);
    }

    void addEdge(TVertex start, TVertex finish, TAlpha alpha) {
        TVertex max_vertex_number = std::max(start, finish);
        resize_(max_vertex_number);
        graph_[start].emplace_back(finish, alpha);
        alphabet_size_ = std::max(alphabet_size_, alpha);
    }

    bool isFinish(TVertex vertex) const {
        return is_finish_[vertex];
    }

    void addFinish(TVertex vertex) {
        resize_(vertex);
        is_finish_[vertex] = true;
    }

    void setStart(TVertex start) {
        resize_(start);
        start_ = start;
    }

    void increaseAlphabetSizeTo(TAlpha new_alphabet_size) {
        if (new_alphabet_size < alphabet_size_) {
            throw std::logic_error("New alphabet size is smaller than current alphabet size");
        }
        alphabet_size_ = new_alphabet_size;
    }

    TVertex getStart() const {
        return start_;
    }

    TVertex getVertexCount() const {
        return vertex_count_;
    }

    TAlpha getAlphabetSize() const {
        return alphabet_size_;
    }

private:
    struct TEdge_ {
        TVertex   finish;
        TAlpha alpha;
        TEdge_(TVertex finish, TAlpha alpha) :
            finish(finish),
            alpha(alpha)
        {}
    };

    std::vector< std::vector<TEdge_> > graph_;
    TVertex            vertex_count_;
    TVertex            start_;
    TAlpha             alphabet_size_;
    std::vector<bool>  is_finish_;

    void resize_(TVertex vertex) {
        if (vertex >= vertex_count_) {
            vertex_count_ = vertex;
            graph_.resize(vertex_count_);
            is_finish_.resize(vertex_count_, false);
        }
    }
};

} // end of namespace NAutomata
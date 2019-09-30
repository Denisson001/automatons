#include "../headers/automaton.h"

// Поиск слова небольшой длины, различающего два ДКА

namespace NSearchDistinguishingWord {

    typedef NAutomaton::TAutomaton TAutomaton;
    typedef NAutomaton::TAlpha     TAlpha;
    typedef NAutomaton::TVertex    TVertex;

    class TSearchDistinguishingWord {
    public:
        /*
         * automaton1 - первый ДКА
         * automaton2 - второй ДКА
         */
        TSearchDistinguishingWord(const TAutomaton& automaton1, const TAutomaton& automaton2) :
            automaton1_(automaton1),
            automaton2_(automaton2)
        {}

        /*
         * Возвращает слово небольшой длины, различающее два ДКА.
         * Если слово не найдено, возвращает null
         * */
        std::optional< std::vector<TAlpha> > getDistinguishingWord() const {
            return getDistinguishingWord_();
        }

    private:
        typedef unsigned long long TWord_;

        const TAutomaton&   automaton1_;           // первый ДКА
        const TAutomaton&   automaton2_;           // второй ДКА
        static const TWord_ MAX_WORD_COUNT_ = 1e7; // максимальное количество слов в переборе

        /*
         * Возвращает слово небольшой длины, различающее два ДКА.
         * Если слово не найдено, возвращает null
         * */
        std::optional< std::vector<TAlpha> > getDistinguishingWord_() const {
            TAlpha alphabet_size = std::max(automaton1_.getAlphabetSize(), automaton2_.getAlphabetSize());
            TWord_ word_count    = 1;

            for (TWord_ word_len = 0; ; ++word_len) { // перебираем длину слова
                if (word_count > MAX_WORD_COUNT_) {
                    break;
                }

                for (TWord_ word = 0; word < word_count; ++word) { // перебираем слово
                    const auto word_vector = getWordVector_(word, word_len, alphabet_size);
                    if (automaton1_.isAcceptable(word_vector) != automaton2_.isAcceptable(word_vector)) {
                        return word_vector; // если один автомат принял слово, в другой нет, то слово найдено
                    }
                }

                word_count *= alphabet_size;
            }

            return std::nullopt;
        }

        /* По маске word, возвращает соответствующий список букв */
        std::vector<TAlpha> getWordVector_(TWord_ word, TWord_ word_len, TAlpha alphabet_size) const {
            std::vector<TAlpha> result(word_len);
            for (std::size_t i = 0; i < word_len; ++i) {
                result[i] = word % alphabet_size;
                word /= alphabet_size;
            }
            return result;
        }

    };

} // end of namespace NSearchDistinguishingWord

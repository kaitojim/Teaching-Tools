#ifndef SENTENCE_H
#define SENTENCE_H

#include "element.h"
#include "word.h"

class Sentence : public Element
{
protected:
    std::vector<Word> m_sentenceStorage {};
public:
    Sentence(std::string input);
    Sentence(std::vector<Word>& inputArr);

    std::string scramble() override;
    std::string blank(ProcessMode&) override;
    std::string blank(float opa_per);
    std::string blank(const std::string& word);
    std::string blank(int idx);
};

template<typename T>
int Search (std::vector<T> arr, T value) 
{
    int i {0};
    int lim {static_cast<int> (arr.size())};
    arr.push_back (value);
    while (arr[i] != value)
    {
        i++;
    }
    
    if (i < lim)
    {
        return i;
    }
    
    return -1;
}
#endif
#ifndef WORD_H
#define WORD_H

#include "element.h"

class Word : public Element
{
public:
    Word(std::string input) : Element(input) {}

    std::string scramble(ProcessMode&) override;
    std::string blank(ProcessMode&) override;
    std::string blank(float opa_per);
    std::string blank(int idx);
    std::string blank();
    std::string blank(const std::string& word);

    bool operator!= (Word input) {return (this->m_eleStr != input.m_eleStr);}
};
#endif
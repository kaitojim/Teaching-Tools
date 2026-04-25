#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <stdexcept>
#include <concepts>

#include "random.h"
#include "InputOption.h"
#include "ProcessMode.h"

class Element
{
protected:
    std::string m_eleStr {};
    std::string m_mod_eleStr {};
    std::vector<int> randomGen(int lim, int num);
public:
    Element(std::string& data) : m_eleStr(data) {}
    Element() {}

    std::string getStr() {return m_eleStr;}
    virtual std::string getModStr();
    void setStr (std::string& input) {m_eleStr = input;}

    virtual ~Element();
    virtual std::string blank(ProcessMode&);
    virtual std::string scramble(ProcessMode&); 
    
    friend std::string formatBlank(std::string& input);
    friend std::ostream& operator<< (std::ostream& out, Element& elem) {return out << elem.getModStr();}
};

void sorting(std::vector<int>& num);

template<typename T>
std::vector<std::string> parseToken(const std::string& input, std::vector<T>& wordList, std::string sep)
{
    std::vector<std::string> connotation {};
    int startPos {static_cast<int>(input.find_first_not_of(sep, 0))};
    int lastPos {static_cast<int>(input.find_first_of(sep, 0))};
    while (startPos >= 0)
    {
        if (lastPos != -1) {connotation.emplace_back(input.substr(lastPos, 1));}
        
        std::string temp {input.substr(startPos, lastPos - startPos)};
        wordList.push_back(temp);
        startPos = input.find_first_not_of(sep, lastPos);

        if (startPos != -1 && input.at(startPos) == ' ') {startPos ++;}
    
        lastPos = input.find_first_of(sep, startPos);
    }

    return connotation;
}
#endif
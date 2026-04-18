#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <string>
#include <stdexcept>

#include "..\random.h"
#include "..\InputOption.h"

class ProcessMode
{
public:
    void setProcessId (optionId id) {m_processId = id;}

    template <typename Tp>
    void setProcessValue (Tp value)  {*(Tp*)m_processValue = value;}

    optionId getProcessId () {return m_processId;}

    template <typename returnTp>
    returnTp getProcessValue() {return *(returnTp*)m_processValue;}

    template <typename Tp>
    bool processModeInit(Tp value) 
    {
        Tp* pBuffer = new Tp;
        if (pBuffer == nullptr)
        {
            return false;
        }
        m_processValue = (void*) pBuffer;
        return true;
    }

    ProcessMode() {}
    ProcessMode(const ProcessMode& other) 
    {
        this->m_processId = other.m_processId;
        *(char*)(this->m_processValue) = *(char*)other.m_processValue;
    }
    ProcessMode& operator=(const ProcessMode& source)
    {
        this->m_processId = source.m_processId;
        *(char*)(this->m_processValue) = *(char*)source.m_processValue;
        return *this;
    }
    ~ProcessMode() 
    {
        if (m_processValue == nullptr)
        {
            return;
        }
        delete[] (char*)m_processValue;
    }
private:
    optionId m_processId {};
    void* m_processValue;
};

class Element
{
protected:
    std::string m_eleStr {};
    std::string m_mod_eleStr {};
    std::vector<int> randomGen(int lim, int num);
public:
    Element(std::string data) : m_eleStr(data) {}
    Element() {}

    std::string getStr() {return m_eleStr;}
    std::string getModStr();
    void setStr (std::string input) {m_eleStr = input;}

    virtual ~Element();
    virtual std::string blank(ProcessMode&);
    virtual std::string scramble(); 
    
    friend std::string formatBlank(std::string& input);
    friend std::ostream& operator<< (std::ostream& out, Element& elem) {return out << elem.getModStr();}
};

void sorting(std::vector<int>& num);

template<typename T>
std::vector<T> parseToken(const std::string& input, std::vector<T>& wordList, std::string sep)
{
    int startPos {static_cast<int>(input.find_first_not_of(sep, 0))};
    int lastPos {static_cast<int>(input.find_first_of(sep, 0))};
    while (startPos >= 0)
    {
        std::string temp {input.substr(startPos, lastPos - startPos)};
        wordList.push_back(temp);
        startPos = input.find_first_not_of(sep, lastPos);

        if (input[startPos] == ' ') {startPos ++;}
    
        lastPos = input.find_first_of(sep, startPos);
    }

    return wordList;
}
#endif
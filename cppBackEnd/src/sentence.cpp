#include "..\header\sentence.h"

Sentence::Sentence(std::string input, std::string conno) : m_conno(conno)
{
    if (input == "")
    {
        throw std::invalid_argument("Cannot find string at class 'Sentence'.");
        return;
    }
    
    m_eleStr = input;
    std::vector<Word> arr {};
    parseToken(input, arr, " ");
    m_sentenceStorage = arr;
}

Sentence::Sentence(std::vector<Word>& inputArr) :m_sentenceStorage(inputArr)
{
    for (int i = 0; i < inputArr.size(); i++)
    {
        m_eleStr += inputArr[i].getStr();
        if (i+1 < inputArr.size()) {m_eleStr.push_back(' ');}
    }
}

std::string Sentence::blank(ProcessMode& process)
{
    if (process.getProcessId() == optionId::percent)
    {
        blank(process.getProcessValue<float>());
    }
    else if (process.getProcessId() == optionId::wordChar)
    {
        blank(process.getProcessValue());
    }
    else throw std::invalid_argument("Invalid processId at class 'Sentence'.");
    return m_mod_eleStr;
}

std::string Sentence::blank(float opa_per)
{
    if (!m_mod_eleStr.empty())
    {
        m_mod_eleStr.clear();
    }

    int length {static_cast<int>(m_sentenceStorage.size())};
    float opa_num {opa_per / 100 * length};
    opa_num = int (opa_num);

    std::vector<int> gen_num{randomGen(m_sentenceStorage.size()-1, opa_num)};
    sorting(gen_num);
    std::string temp{};
    for (int i = 0; i < m_sentenceStorage.size(); i++)
    {
        std::string tempStr{m_sentenceStorage[i].getStr()};
        for (int j = 0; j < gen_num.size(); j++)
        {
            if (i == gen_num[j]) {tempStr = "____";}
        }
        
        temp += tempStr;

        if (i+1 < m_sentenceStorage.size()) {temp += ' ';}
    }

    return m_mod_eleStr = temp;
}

std::string Sentence::scramble(ProcessMode& process)
{
    if (!m_mod_eleStr.empty())
    {
        m_mod_eleStr.clear();
    }

    int num {static_cast<int>(m_sentenceStorage.size())};
    std::vector<int> gen_num {randomGen(num-1, num)};
    
    std::string temp{};
    for (int i = 0; i < gen_num.size(); i++)
    {
        temp += m_sentenceStorage[gen_num[i]].getStr();

        if (i+1 < gen_num.size()) {temp.push_back(' ');}
    }

    return m_mod_eleStr = temp;
}

std::string Sentence::blank(const std::string& word)
{
    if (!m_mod_eleStr.empty())
    {
        m_mod_eleStr.clear();
    }

    int idx {};
    for (size_t i = 0; i < m_sentenceStorage.size(); i++)
    {
        std::string temp {m_sentenceStorage[i].getStr()};
        std::transform(temp.begin(), temp.end(), temp.begin(), 
        [] (char c)
        {
            return std::tolower(c);
        });
        if (temp == word)
        {
            idx = i;
            break;
        }
        else idx = -1;
    }

    if (idx == -1) 
    {
        throw std::invalid_argument("Cannot find word/character at class 'Sentence'.");
        return m_eleStr;
    }
    
    std::string tempStr {};
    for (int i = 0; i < m_sentenceStorage.size(); i++)
    {
        if (i != idx) {tempStr += m_sentenceStorage[i].getStr();}
        else {tempStr += "_______";}
        
        if (i+1 < m_sentenceStorage.size()) {tempStr.push_back(' ');}
    }
    
    return m_mod_eleStr = tempStr;
}

std::string Sentence::blank(int idx)
{
    if (!m_mod_eleStr.empty())
    {
        m_mod_eleStr.clear();
    }

    std::string tempStr;
    for (size_t i = 0; i < m_sentenceStorage.size(); i++)
    {
        if (i != idx) {tempStr += m_sentenceStorage[i].getStr();}
        else {tempStr += "_______";}
        
        if (i+1 < m_sentenceStorage.size()) {tempStr.push_back(' ');}
    }
    return m_mod_eleStr = tempStr;
}

std::string Sentence::getModStr()
{
    return m_mod_eleStr + m_conno;
}
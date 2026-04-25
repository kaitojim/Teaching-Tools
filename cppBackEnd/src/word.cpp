#include "..\header\word.h"

std::string Word::blank(ProcessMode& process)
{
    if (process.getProcessId() == optionId::percent)
    {
        blank(process.getProcessValue<float>());
    }

    else if (process.getProcessId() == optionId::idx)
    {
        blank(process.getProcessValue<int>());
    }

    else if (process.getProcessId() == optionId::wordChar)
    {
        blank(process.getProcessValue());  
    }

    else if(process.getProcessId() == optionId::firstWord)
    { 
        blank();
    }
    else throw std::invalid_argument("Invalid process id at class 'Word'.");
    return m_mod_eleStr;
}

std::string Word::blank(float opa_per)
{
    std::string formatEle {m_eleStr + '|'};
    if (m_eleStr != m_mod_eleStr)
    {
        m_mod_eleStr.clear();
    }

    int startPos = formatEle.find_first_not_of(" |", 0);
    int lastPos = formatEle.find_first_of(" |", startPos);

    if (lastPos == std::string::npos)
    {
        lastPos = formatEle.size();
    }

    while (lastPos != std::string::npos)
    {
        int len {lastPos - startPos};

        std::string temp {formatEle.substr(startPos, len)};
        float opa_num {opa_per / 100 * len};
        opa_num = int (opa_num);
        std::vector<int> gen_num {this->randomGen(temp.size()-1, opa_num)};
        sorting(gen_num);

        for (int i = 0; i < gen_num.size(); i++)
        {
            temp.replace (gen_num[i], 1, "_");
        }
        
        m_mod_eleStr += (temp);
        startPos = formatEle.find_first_not_of(" |", lastPos);
        lastPos = formatEle.find_first_of(" |", startPos);
        if (lastPos != std::string::npos)
        {
            m_mod_eleStr += ' ';
        }
    }
    return m_mod_eleStr;
}
std::string Word::scramble(ProcessMode& process) 
{
    std::string formatEle {m_eleStr + '|'};
    if (m_eleStr != m_mod_eleStr)
    {
       m_mod_eleStr.clear();
    }

    int startPos = formatEle.find_first_not_of(" |", 0);
    int lastPos = formatEle.find_first_of(" |", startPos);

    if (lastPos == std::string::npos)
    {
        lastPos = formatEle.size();
    }

    while (lastPos != std::string::npos)
    {
        int len {lastPos - startPos};
        std::string temp {formatEle.substr(startPos, len)};
        std::vector<int> gen_num {this->randomGen(temp.size()-1, temp.size())};

        std::string mod_temp {};
        for (int i = 0; i < gen_num.size(); i++)
        {
            mod_temp.push_back(temp[gen_num[i]]);
        }
        
        m_mod_eleStr += (mod_temp);
        startPos = formatEle.find_first_not_of(" |", lastPos);
        lastPos = formatEle.find_first_of(" |", startPos);
        if (lastPos != std::string::npos)
        {
            m_mod_eleStr += ' ';
        }
    }

    return m_mod_eleStr;
}

std::string Word::blank(int idx)
{
    std::string formatEle {m_eleStr + '|'};
    if (m_eleStr != m_mod_eleStr)
    {
        m_mod_eleStr.clear();
    }

    int startPos = formatEle.find_first_not_of(" |", 0);
    int lastPos = formatEle.find_first_of(" |", startPos);

    if (lastPos == std::string::npos)
    {
        lastPos = formatEle.size();
    }

    while (lastPos != std::string::npos)
    {
        int len {lastPos - startPos};

        std::string temp {formatEle.substr(startPos, len)};
        std::vector<int> gen_num {this->randomGen(temp.size()-1, temp.size()-2)};
        sorting(gen_num);

        for (int i = 0; i < gen_num.size(); i++)
        {
            if (i == idx)
            {
                continue;
            }
            temp.replace (gen_num[i], 1, "_");
        }
        
        m_mod_eleStr += (temp);
        startPos = formatEle.find_first_not_of(" |", lastPos);
        lastPos = formatEle.find_first_of(" |", startPos);
        if (lastPos != std::string::npos)
        {
            m_mod_eleStr += ' ';
        }
    }
    return m_mod_eleStr;
}

std::string Word::blank()
{
    std::string formatEle {m_eleStr + '|'};
    if (m_eleStr != m_mod_eleStr)
    {
        m_mod_eleStr.clear();
    }

    int startPos = formatEle.find_first_not_of(" |", 0);
    int lastPos = formatEle.find_first_of(" |", startPos);

    if (lastPos == std::string::npos)
    {
        lastPos = formatEle.size();
    }

    while (lastPos != std::string::npos)
    {
        int len {lastPos - startPos};

        std::string temp {formatEle.substr(startPos, 1)};
        std::string blankSym {};
        for (size_t i = 0; i < len - 1; i++)
        {
            blankSym.push_back('_');
        }
        
        m_mod_eleStr += (temp + blankSym);
        startPos = formatEle.find_first_not_of(" |", lastPos);
        lastPos = formatEle.find_first_of(" |", startPos);
        if (lastPos != std::string::npos)
        {
            m_mod_eleStr += ' ';
        }
    }
    return m_mod_eleStr;
}

std::string Word::blank(const std::string& word)
{
    if (m_eleStr != m_mod_eleStr)
    {
       m_mod_eleStr.clear();
    }

    auto wordId {m_eleStr.find(word)};
    if (wordId == std::string::npos)
    {
        throw std::runtime_error("No word/character match at class 'Word'.");
        return m_mod_eleStr;
    }

    m_mod_eleStr = m_eleStr;
    m_mod_eleStr.replace(wordId, 1, "_");
    return m_mod_eleStr;
}

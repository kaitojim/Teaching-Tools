#include "..\header\element.h"
#include <cctype>

std::vector<int> Element::randomGen(int lim, int num)
{
    std::vector<int> gen_num{};
    for (int i = 0; i < num; i++)
    {
        bool numAlready {false};
        int mod_pos {Random::get(0, static_cast<int>(lim))};

        for (int i = 0; i < gen_num.size(); i++)
        {
            if (mod_pos == gen_num[i])
            {
                numAlready = true;
                break;
            }
        }

        if (numAlready)
        {
            i--;
            continue;
        }
            
        gen_num.push_back(mod_pos);
    }
    
    return gen_num;
}

std::string formatBlank(std::string& input)
{
    int pos = input.find_first_of('_', 0);
    while (pos > -1)
    {
        if (!isalnum(input[pos+1]))
        {
            input.insert(pos+1, " ");
        }
        
        pos ++;
        pos = input.find_first_of('_', pos);
    }
    
    return input;
}

std::string Element::getModStr()
{
    return formatBlank(m_mod_eleStr);
}

Element::~Element() = default;

std::string Element::blank(ProcessMode&)
{
    return m_mod_eleStr;
}

std::string Element::scramble()
{
    return m_mod_eleStr;
}

void sorting(std::vector<int>& num)
{
    for (int i = 1; i < num.size(); i++)
    {
        int maxIdx {i};
        for (int j = 0; j < num.size(); j++)
        {
            if (num[maxIdx] < num[j])
            {
                maxIdx = j;
            }
            
            if (i != maxIdx)
            {
                int swap = num[maxIdx];
                num[maxIdx] = num[i];
                num[i] = swap;
            }
        }
    }
    return;
}
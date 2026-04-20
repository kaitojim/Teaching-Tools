#include <iostream>
#include <vector>
#include <cctype>
#include <fstream>
#include <limits>
#include <memory>
#include <cstdlib>

#include ".\header\element.h"
#include ".\header\InputOption.h"
#include ".\header\word.h"
#include ".\header\sentence.h"

template <typename Tp>
void additionalBlankProcessMode(ProcessMode& process, optionId inputOptionId, Tp value)
{
    process.processModeInit((value));
    process.setProcessId(inputOptionId);
    process.setProcessValue((value));
}

void functionMode(std::vector<std::unique_ptr<Element>>& inputArr, optionId inputOptionId, ProcessMode& additionalProcesMode)
{   
    // Function (scramble or fill in the blank)
    std::string error {};
    try
    {
        switch (inputOptionId)
        {
        case optionId::scramble:
            for (auto& p : inputArr)
            {
                p->scramble();
            }
            break;
        
        case optionId::fill:
            for (auto& p : inputArr)
            {
                p->blank(additionalProcesMode);
            }
            break;

        default:
            throw error = "Invalid function";
            break;
        }
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
}

void elementProcessMode(const std::string& input, std::vector<std::unique_ptr<Element>>& inputArr, optionId elementMode)
{
    // Array for storing parsed element 
    // - At this stage the programe don't know if it is processing word or sentence
    // - Just parsing based on the input separator
    std::vector<Element> eleArr{}; 
    parseToken(input, eleArr, ",.");
    
    // Handle changing the base class to the choosen element (word or sentence) 
    // Element (word or setence processing)
    for (size_t i = 0; i < eleArr.size(); i++)
    {
        if (elementMode == optionId::word)
        {
            auto ptr = std::make_unique<Word>(eleArr[i].getStr());
            inputArr.push_back(std::move(ptr));
        }
        else if (elementMode == optionId::sentence)
        {
            auto ptr = std::make_unique<Sentence>(eleArr[i].getStr());
            inputArr.push_back(std::move(ptr));
        }
        else throw "Invalid processMode";
    }
}


int main()
{
    std::string test {"word, elephant, ground floor"};
    return 0;
}
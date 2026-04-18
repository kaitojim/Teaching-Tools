#include <iostream>
#include <vector>
#include <cctype>
#include <fstream>
#include <limits>
#include <memory>
#include <cstdlib>
#include <napi.h>

#include "..\header\element.h"
#include "..\header\word.h"
#include "..\header\sentence.h"
#include "..\header\InputOption.h"

#define NO_VALUE 9900

// NAPI function to turn JS obj to inputOption struct

template <typename Tp>
void additionalBlankProcessMode(ProcessMode& process, inputOption<Tp> additionalProcessMode)
{
    process.processModeInit(additionalProcessMode.value);
    process.setProcessId(additionalProcessMode.inputOptionId);
    process.setProcessValue(additionalProcessMode.value);
}

// Handle scramble and fill in the blank requests whether if it's word or sentence array
template <typename Tp1, typename Tp2>
void functionMode(std::vector<std::unique_ptr<Element>>& inputArr, inputOption<Tp1> funcMode, inputOption<Tp2> additionalProcesMode = 0)
{   
    // Function (scramble or fill in the blank)
    ProcessMode processMode {};
    switch (funcMode.inputOptionId)
    {
    case optionId::scramble:
        for (auto& p : inputArr)
        {
            p->scramble();
        }
        break;
    
    case optionId::fill:
        additionalBlankProcessMode<Tp2> (processMode, additionalProcesMode);
        for (auto& p : inputArr)
        {
            p->blank(processMode);
        }
        break;
    }
}

template <typename Tp>
void elementProcessMode(const std::string& input, std::vector<std::unique_ptr<Element>>& inputArr, inputOption<Tp> elementMode)
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
        if (elementMode.inputOptionId == optionId::word)
        {
            auto ptr = std::make_unique<Word>(eleArr[i].getStr());
            inputArr.push_back(std::move(ptr));
        }
        else
        {
            auto ptr = std::make_unique<Sentence>(eleArr[i].getStr());
            inputArr.push_back(std::move(ptr));
        }
    }
}

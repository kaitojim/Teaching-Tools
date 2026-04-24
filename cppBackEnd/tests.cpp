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
void additionalProcessMode(ProcessMode& process, optionId inputOptionId, Tp value)
{
    process.processModeInit((value));
    process.setProcessId(inputOptionId);
    process.setProcessValue((value));
}

void functionMode(std::vector<std::unique_ptr<Element>>& inputArr, optionId inputOptionId, ProcessMode& additionalProcesMode)
{   
    // Function (scramble or fill in the blank)
    switch (inputOptionId)
    {
    case optionId::scramble:
        for (auto& p : inputArr)
        {
            p->scramble(additionalProcesMode);
        }
        break;
        
    case optionId::fill:
        for (auto& p : inputArr)
        {
            p->blank(additionalProcesMode);
        }
        break;

    default:
        throw std::invalid_argument("Invalid function");
        break;
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

int main(int argC, char* arg[])
{
    if (argC < 5)
    {
        std::cerr << "Invalid arg count" << "\n";
        return 1;
    }
    
    std::cout << "Valid arg" << "\n";

    ProcessMode process {};
    std::vector<std::unique_ptr<Element>> inputArr {};
    optionId processMode {stringToId((std::string) arg[1])};
    optionId funcMode {stringToId((std::string) arg[2])};
    optionId addProcessMode {stringToId((std::string) arg[3])};
    try
    {
        elementProcessMode((std::string) arg[5], inputArr, processMode);

        std::cout << "Set element mode sucessfully" << "\n";

        int perIdx {};
        float per {};
        switch (addProcessMode)
        {
        case optionId::wordChar:    
            additionalProcessMode(process, optionId::wordChar, (std::string) arg[4]);                
            break;
        case optionId::idx:
            perIdx = *((int*) arg[4]);   
            additionalProcessMode(process, optionId::idx, perIdx);                        
            break;
        case optionId::percent:
            per = *((float*) arg[4]);
            additionalProcessMode(process, optionId::wordChar, per);    
            break;
        case optionId::firstWord:
            perIdx = *((int*) arg[4]);
            additionalProcessMode(process, optionId::firstWord, perIdx);    
            break;
        case optionId::blank:
            if (funcMode == optionId::scramble)
            {
                perIdx = *((int*) arg[4]);
                additionalProcessMode(process, optionId::blank, perIdx); 
                break;
            }
        default:
            throw std::invalid_argument("Invalid addtional process mode");
            break;
        }

        std::cout << "Set additional process mode sucessfully" << "\n";

        functionMode(inputArr, funcMode, process);

        std::cout << "Set function mode sucessfully" << "\n";
    }
    catch(std::runtime_error& e)
    {
        std::cout << e.what() << "\n";
        return 1;
    }
    catch(const std::invalid_argument& e)
    {   
        std::cout << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Error" << "\n";
        return 1;
    }

    for (auto& p : inputArr)
    {
        std::cout << p->getModStr() << "\n";
    }
    
    return 0;
}
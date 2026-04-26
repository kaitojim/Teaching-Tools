#include <iostream>
#include <vector>
#include <cctype>
#include <fstream>
#include <limits>
#include <memory>
#include <cstdlib>
#include <napi.h>
#include <array>
#include <variant>

#include "..\header\element.h"
#include "..\header\word.h"
#include "..\header\sentence.h"
#include "..\header\InputOption.h"
#include "..\header\transform.h"

#define NO_VALUE 9900
#define DEBUG 11111

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
        throw std::invalid_argument("Invalid function mode");
        break;
    } 
}

void elementProcessMode(const std::string& input, std::vector<std::unique_ptr<Element>>& inputArr, optionId elementMode)
{
    // Array for storing parsed element 
    // - At this stage the programe don't know if it is processing word or sentence
    // - Just parsing based on the input separator
    std::vector<Element> eleArr{}; 
    std::string sep {};
    switch (elementMode)
    {
    case optionId::word:
        sep = ",;.";
        break;

    case optionId::sentence:
        sep = ".?!";
        break;

    default:
        throw std::invalid_argument("Invalid process mode");
        break;
    }

    std::vector<std::string> conno {};
    try
    {
        conno = parseToken(input, eleArr, sep);
    }
    catch(const std::invalid_argument& e)
    {
       throw e;
       return;
    }
    
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
            auto ptr = std::make_unique<Sentence>(eleArr[i].getStr(), conno[i]);
            ptr->setConno(conno[i]);
            inputArr.push_back(std::move(ptr));
        }
        else throw std::invalid_argument("Invalid processMode");
    }
}

void additionalCheck(ProcessMode& process, const Napi::CallbackInfo& info, const std::vector<optionId>& optionIdContain)
{
    Napi::Env env {info.Env()};
    if (optionIdContain[1] == optionId::scramble)
    {
        auto processValue {0};
        additionalProcessMode(process, optionId::blank, processValue);
        return;
    }

    if (optionIdContain[2] == optionId::idx)
    {
        auto processValue {(info[2].As<Napi::Number>().Int32Value())};
        additionalProcessMode(process, optionIdContain[2], processValue);

    }
    else if (optionIdContain[2] == optionId::percent)
    {
        auto processValue {(info[2].As<Napi::Number>().FloatValue())};
        additionalProcessMode(process, optionIdContain[2], processValue);
    }
    else if (optionIdContain[2] == optionId::wordChar)
    {
        auto processValue {(info[2].As<Napi::String>().Utf8Value())};
        additionalProcessMode(process, optionIdContain[2], processValue);
    }
        else if (optionIdContain[2] == optionId::firstWord)
    {
        additionalProcessMode(process, optionIdContain[2], 0);
    }
    else
    {
        Napi::TypeError::New(env, "Invalid additionalProcessMode value").ThrowAsJavaScriptException();
    }
    return;
}

Napi::Value handleInput(const Napi::CallbackInfo& info)
{
    //Init environment
    Napi::Env env = info.Env();

    //Array check and init
    if (!info[1].IsArray())
    {
        Napi::TypeError::New(env, "Need an array").ThrowAsJavaScriptException();
    }

    Napi::Array arrJS = info[1].As<Napi::Array>();// pass array of optionId from JS to c++ 
    std::vector<optionId> optionIdContain {};
    optionIdContain.reserve(arrJS.Length());// request container to be at least enought to store Length amount 

    for (uint32_t i = 0; i < arrJS.Length(); i++)
    {
        Napi::Value val {arrJS[i]};
        if (val.IsObject())
        {
            Napi::Object obj {val.As<Napi::Object>()};
            std::string key {std::to_string(i + 1)};
            if (obj.Has(key))
            {
                std::string value {obj.Get(key).As<Napi::String>().Utf8Value()};
                std::transform(value.begin(), value.end(), value.begin(), [](char temp){
                    return std::tolower(temp);
                });
                std::erase_if(value, [](char temp){return std::isblank(temp);});
                try {optionIdContain.emplace_back(stringToId(value));}
                catch (const std::invalid_argument& e) {Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();}
            }
        }
        else Napi::TypeError::New(env, "Need an object for option id").ThrowAsJavaScriptException();
    }  

    std::string inputStr {info[0].As<Napi::String>().Utf8Value()};
    std::vector<std::unique_ptr<Element>> inputArr {};
    try
    {
        elementProcessMode(inputStr, inputArr, optionIdContain[0]);
        std::cout << "elementMode" << '\n';

        ProcessMode additionalProcessMode {};
        additionalCheck(additionalProcessMode, info, optionIdContain);
        std::cout << "additionalMode" << '\n';

        functionMode(inputArr, optionIdContain[1], additionalProcessMode);
        std::cout << "funcMode" << '\n';

    }
    catch(const std::invalid_argument& e)
    {
        Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();
    }
    catch(const std::runtime_error& e)
    {
        Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();
    }

    Napi::Array outputArr  {Napi::Array::New(env, inputArr.size())};
    for (size_t i = 0; i < inputArr.size(); i++)
    {
        std::cout << inputArr[i].get()->getModStr() << '\n';
        outputArr.Set(i, Napi::String::New(env, inputArr[i].get()->getModStr()));
    }

    return outputArr;
}

Napi::Object init(Napi::Env env, Napi::Object exports)
{
    exports.Set("elementTransform", Napi::Function::New(env, handleInput));
    return exports;
}

NODE_API_MODULE(elementProcess, init)

void LogFromCpp(Napi::Env& env, const std::string& msg) 
{
    // Get the global 'console' object
    Napi::Object console = env.Global().Get("console").As<Napi::Object>();
    
    // Get the 'log' function from the console object
    Napi::Function log = console.Get("log").As<Napi::Function>();
    
    // Call console.log("Hello from C++")
    log.Call({ Napi::String::New(env, msg) });
}

void LogFromCpp(Napi::Env& env, int msg) 
{
    // Get the global 'console' object
    Napi::Object console = env.Global().Get("console").As<Napi::Object>();
    
    // Get the 'log' function from the console object
    Napi::Function log = console.Get("log").As<Napi::Function>();
    
    // Call console.log("Hello from C++")
    log.Call({ Napi::Number::New(env, msg) });
}

void LogFromCpp(Napi::Env& env, float msg) 
{
    // Get the global 'console' object
    Napi::Object console = env.Global().Get("console").As<Napi::Object>();
    
    // Get the 'log' function from the console object
    Napi::Function log = console.Get("log").As<Napi::Function>();
    
    // Call console.log("Hello from C++")
    log.Call({ Napi::Number::New(env, msg) });
}
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

Napi::Value handleInput(const Napi::CallbackInfo& info)
{
    //Init environment
    Napi::Env env = info.Env();

#ifdef DEBUG
    LogFromCpp(env, "env generate");
#endif

    //Array check and init
    if (!info[1].IsArray())
    {
        Napi::TypeError::New(env, "Need an array").ThrowAsJavaScriptException();
    }

#ifdef DEBUG
    LogFromCpp(env, "Array checked");
#endif

    Napi::Array arrJS = info[1].As<Napi::Array>();// pass array from JS to c++ 
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
                catch (std::string& e) {Napi::TypeError::New(env, e).ThrowAsJavaScriptException();}
            }
        }
        else Napi::TypeError::New(env, "Need an object for option id").ThrowAsJavaScriptException();
    }

#ifdef DEBUG 
    for (size_t i = 0; i < optionIdContain.size(); i++)
    {
        LogFromCpp(env, (int)optionIdContain[i]);
    }
#endif   

#ifdef DEBUG
    LogFromCpp(env, "Array passed successfully");
#endif

    std::string inputStr {info[0].As<Napi::String>().Utf8Value()};
    std::vector<std::unique_ptr<Element>> inputArr {};
    try {elementProcessMode(inputStr, inputArr, optionIdContain[0]);}
    catch (std::string& e) {Napi::TypeError::New(env, e).ThrowAsJavaScriptException();}

#ifdef DEBUG
    for (size_t i = 0; i < inputArr.size(); i++)
    {
        LogFromCpp(env, inputArr[i]->getStr());
    }
    
    LogFromCpp(env, "Element init sucessfully");
#endif

    ProcessMode additionalProcessMode {};
    if (optionIdContain[2] == optionId::idx)
    {
        auto processValue {(info[2].As<Napi::Number>().Int32Value())};
        additionalBlankProcessMode(additionalProcessMode, optionIdContain[2], processValue);

#ifdef DEBUG
    LogFromCpp(env, "idx");
    LogFromCpp(env, processValue);

#endif
    }
    else if (optionIdContain[2] == optionId::percent)
    {
        auto processValue {(info[2].As<Napi::Number>().FloatValue())};
        additionalBlankProcessMode(additionalProcessMode, optionIdContain[2], processValue);

#ifdef DEBUG
    LogFromCpp(env, "percent");
    LogFromCpp(env, processValue);
#endif

    }
    else if (optionIdContain[2] == optionId::wordChar)
    {
        auto processValue {(info[2].As<Napi::String>().Utf8Value())};
        additionalBlankProcessMode(additionalProcessMode, optionIdContain[2], processValue);

#ifdef DEBUG
    LogFromCpp(env, "wordChar");
    LogFromCpp(env, processValue);
#endif

    }
    else if (optionIdContain[2] == optionId::firstWord)
    {
        additionalBlankProcessMode(additionalProcessMode, optionIdContain[2], optionId::blank);

#ifdef DEBUG
    LogFromCpp(env, "fistWord");
#endif

    }
    else
    {
        Napi::TypeError::New(env, "Invalid additionalProcessMode value").ThrowAsJavaScriptException();
    }

#ifdef DEBUG
    LogFromCpp(env, "Blank additional mode checked successfully");
#endif

    try {functionMode(inputArr, optionIdContain[1], additionalProcessMode);}
    catch (std::string& e) {Napi::TypeError::New(env, e).ThrowAsJavaScriptException();}
    catch (const std::invalid_argument& e) {Napi::TypeError::New(env, e.what()).ThrowAsJavaScriptException();}

#ifdef DEBUG
    LogFromCpp(env, "Function passed successfully");
#endif

    Napi::Array outputArr  {Napi::Array::New(env, inputArr.size())};
    for (size_t i = 0; i < inputArr.size(); i++)
    {
        outputArr.Set(i, Napi::String::New(env, inputArr[i].get()->getModStr()));
    }

#ifdef DEBUG
    LogFromCpp(env, "Output successfully");
#endif

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
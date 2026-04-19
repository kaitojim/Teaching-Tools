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

template <typename Tp>
void additionalBlankProcessMode(ProcessMode& process, optionId inputOptionId, Tp value)
{
    process.processModeInit((value));
    process.setProcessId(inputOptionId);
    process.setProcessValue((value));
}

// Handle scramble and fill in the blank requests whether if it's word or sentence array
void functionMode(std::vector<std::unique_ptr<Element>>& inputArr, optionId inputOptionId, ProcessMode& additionalProcesMode);
void elementProcessMode(const std::string& input, std::vector<std::unique_ptr<Element>>& inputArr, optionId elementMode);

Napi::Value handleInput(const Napi::CallbackInfo& info);

//Debug helper
void LogFromCpp(Napi::Env& env, const std::string& msg);
void LogFromCpp(Napi::Env& env, int msg);
void LogFromCpp(Napi::Env& env, float msg);
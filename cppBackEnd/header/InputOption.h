#ifndef INPUT_OPTION_H
#define INPUT_OPTION_H

#include <unordered_map>
#include <string>
#include <stdexcept>

enum optionId
{
    // No option
    blank    = 9999U,

    // Element Process Mode
    word     = 0001U,
    sentence = 0002U,

    //Function Mode
    scramble = 1001U,
    fill     = 1002U,

    // Additional Blank Mode
    wordChar = 3001U,
    percent  = 3002U,
    idx      = 3003U,
    firstWord= 3004U
};


namespace optionConvert
{
    inline std::unordered_map<std::string, optionId> convertTable 
    {
        {"word",                                        optionId::word},
        {"sentence",                                    optionId::sentence},
        {"scramble",                                    optionId::scramble},
        {"fillintheblank",                              optionId::fill},
        {"fillintheblank(percentage)",                  optionId::percent},
        {"fillintheblank(wordorcharacter)",             optionId::wordChar},
        {"fillintheblank(index)",                       optionId::idx},
        {"fillintheblank(firstletterorword)",           optionId::firstWord},
        {"blank",                                       optionId::blank}
    };
}

template <typename Tp>
struct inputOption
{
    optionId inputOptionId {};
    Tp value;
};

inline optionId stringToId(const std::string& inputStr, std::unordered_map<std::string, optionId> convertTable = optionConvert::convertTable)
{
    auto it {convertTable.find(inputStr)};
    if (it != convertTable.end())
    {
        return it->second;
    }
    throw std::invalid_argument("Cannot find string at class 'InputOption'");
}
#endif
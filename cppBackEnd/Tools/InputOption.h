#ifndef INPUT_OPTION_H
#define INPUT_OPTION_H

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


template <typename Tp>
struct inputOption
{
    optionId inputOptionId {};
    Tp value;
};

#endif
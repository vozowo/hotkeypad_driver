#pragma once
#include <vector>

class c_hotkey
{
public:
    c_hotkey
    (
         const std::vector<unsigned short> keys_to_press = { }
    );
    std::vector<unsigned short> keys_to_press;
    void press();
    void release();
};


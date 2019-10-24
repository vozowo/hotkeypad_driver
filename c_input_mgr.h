#pragma once
#include <array>
#include <string>

class c_input_mgr
{
public:
    void update();

    class c_key
    {
    public:
        unsigned long down_updates = 0;
        std::string name = "";
    };

    std::array<c_key, 256> keys;
};

extern c_input_mgr input_mgr;
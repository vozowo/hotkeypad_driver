#define _WIN32_WINNT 0x0500
#include "c_hotkey.h"
#include <Windows.h>

c_hotkey::c_hotkey
    (
        const std::vector<unsigned short> keys_to_press
    )
{
    this->keys_to_press = keys_to_press;
}

void c_hotkey::press()
{
    if (!this->keys_to_press.size())
    {
        return;
    }

    INPUT input;
    memset(&input, 0, sizeof(input));
    input.type = INPUT_KEYBOARD;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.dwFlags = KEYEVENTF_SCANCODE;

    for (std::size_t i = 0; i < this->keys_to_press.size(); i++)
    {
        input.ki.wScan = MapVirtualKeyA(this->keys_to_press[i], MAPVK_VK_TO_VSC);
        input.ki.wVk = this->keys_to_press[i];

        SendInput(1, &input, sizeof(input));
    }
}

void c_hotkey::release()
{
    if (!this->keys_to_press.size())
    {
        return;
    }

    INPUT input;
    memset(&input, 0, sizeof(input));

    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    for (std::size_t i = 0; i < this->keys_to_press.size(); i++)
    {
        input.ki.wScan = MapVirtualKeyA(this->keys_to_press[i], MAPVK_VK_TO_VSC);
        input.ki.wVk = this->keys_to_press[i];

        SendInput(1, &input, sizeof(input));
    }
}

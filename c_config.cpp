#include "c_config.h"
#include <Windows.h>
#include <string>

c_config config;

void c_config::load_file()
{
	static const std::string file = ".\\cfg.ini";

	this->reloadkey = GetPrivateProfileIntA("cfg", "reloadkey", this->reloadkey, file.data());

	for (std::size_t i = 0; i < this->hotkeys.size(); i++)
	{
		this->hotkeys[i].keys_to_press.clear();

		std::size_t keys_to_press = 0;
		int last_key = 0;

		do
		{		
			last_key = GetPrivateProfileIntA("cfg", ((std::string)"hotkey" + std::to_string(i) + "_key" + std::to_string(keys_to_press)).data(), 0, file.data());
			
			if (last_key)
			{
				this->hotkeys[i].keys_to_press.push_back(last_key);
			}

			keys_to_press++;
		}
		while (last_key);
	}

	this->led_active = GetPrivateProfileIntA("cfg", "led_active", this->led_active, file.data());
	this->led_reactive = GetPrivateProfileIntA("cfg", "led_reactive", this->led_reactive, file.data());
	this->led_colormode = GetPrivateProfileIntA("cfg", "led_colormode", this->led_colormode, file.data());
	this->led_solidcolor_r = GetPrivateProfileIntA("cfg", "led_solidcolor_r", this->led_solidcolor_r, file.data());
	this->led_solidcolor_g = GetPrivateProfileIntA("cfg", "led_solidcolor_g", this->led_solidcolor_g, file.data());
	this->led_solidcolor_b = GetPrivateProfileIntA("cfg", "led_solidcolor_b", this->led_solidcolor_b, file.data());
}
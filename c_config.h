#pragma once
#include <vector>
#include <array>

class c_config
{
public:
	void load_file();

	unsigned short reloadkey = 0x2d;

	class c_hotkey
	{
	public:
		std::vector<unsigned short> keys_to_press;
	};

	std::array<c_hotkey, 6> hotkeys;
	bool led_active = true;
	int led_reactive = 0; // 0-off 1-on 2-inverted
	int led_colormode = 0; // 0-solid 1-rainbow_fast 2-rainbow_slow 3-rainbow_reactive
	int led_solidcolor_r = 0;
	int led_solidcolor_g = 0;
	int led_solidcolor_b = 255;
};

extern c_config config;
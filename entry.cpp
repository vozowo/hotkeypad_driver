#include "c_serial.h"
#include <stdio.h>
#include <thread>
#include "c_input_mgr.h"
#include "c_hotkey.h"
#include "c_config.h"

std::array<c_hotkey, 6> hotkeys;

void load_and_send_config()
{
	config.load_file();

	for (std::size_t i = 0; i < hotkeys.size(); i++)
	{
		hotkeys[i].keys_to_press = config.hotkeys[i].keys_to_press;
	}

	std::string string =
		"c" +
		std::to_string(config.led_active) + "," +
		std::to_string(config.led_reactive) + "," +
		std::to_string(config.led_colormode) + "," +
		std::to_string(config.led_solidcolor_r) + "," +
		std::to_string(config.led_solidcolor_g) + "," +
		std::to_string(config.led_solidcolor_b) + ",\n";

	serial.send_string(string);
	Beep(200, 50);
}

int main()
{
    serial.open("\\\\.\\COM3");

    std::this_thread::sleep_for(std::chrono::seconds(2));

	load_and_send_config();

    while (true)
    {
		input_mgr.update();

		if (input_mgr.keys[config.reloadkey].down_updates == 1)
		{
			load_and_send_config();
		}

        std::string buffer = serial.read_string_until('\n');

        if (buffer.length())
        {
            printf("%s\n", buffer.data());

			for (std::size_t i = 0; i < hotkeys.size(); i++)
			{
				if (buffer == (std::to_string(i) + "d"))
				{
					hotkeys[i].press();
				}
				else if (buffer == (std::to_string(i) + "u"))
				{
					hotkeys[i].release();
				}
			}
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    return 0;
}

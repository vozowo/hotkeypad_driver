#include "c_input_mgr.h"
#include <Windows.h>

c_input_mgr input_mgr;

void c_input_mgr::update()
{
    for (std::size_t i = 0; i < this->keys.size(); i++)
    {
        if (GetAsyncKeyState(i))
        {
            this->keys[i].down_updates++;

            if (!this->keys[i].name.length())
            {
				int scan_code = MapVirtualKeyA(i, MAPVK_VK_TO_VSC);

                switch (i)
                {
                    case VK_LEFT: case VK_UP:
                    case VK_RIGHT: case VK_DOWN:
                    case VK_RCONTROL: case VK_RMENU:
                    case VK_LWIN: case VK_RWIN:
                    case VK_APPS: case VK_PRIOR:
                    case VK_NEXT: case VK_END:
                    case VK_HOME: case VK_INSERT:
                    case VK_DELETE: case VK_DIVIDE:
                    case VK_NUMLOCK:
						scan_code |= KF_EXTENDED;
                        break;
                }

				char temp_name[128];

                GetKeyNameTextA(scan_code << 16, temp_name, sizeof(temp_name));

                this->keys[i].name = temp_name;
            }
        }
        else
        {
            this->keys[i].down_updates = 0;
        }
    }
}

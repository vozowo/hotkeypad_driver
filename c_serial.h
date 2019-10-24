#pragma once
#include <Windows.h>
#include <string>

class c_serial
{
public:
    c_serial(std::string port_name = "");
    ~c_serial();
    void set_timeout(unsigned long ms);
    int open(std::string port_name);
    void close();
    std::string read_string_until(char character);
	std::string read_fast();
    void send_string(std::string string);
private:
    static const unsigned long max_bytes = 255;
    HANDLE port = nullptr;
    DCB params;
    unsigned long errors = 0;
    COMSTAT status;
    unsigned long timeout_ms = 1000;
};

extern c_serial serial;
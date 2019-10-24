#include "c_serial.h"
#include <time.h>
#include <stdio.h>

c_serial serial;

c_serial::c_serial(std::string port_name)
{
    if (port_name.size())
    {
        this->open(port_name);
    }
}

c_serial::~c_serial()
{
    this->close();
}

void c_serial::set_timeout(unsigned long ms)
{
    this->timeout_ms = ms;

    if (this->timeout_ms < 1)
    {
        this->timeout_ms = 1;
    }
}

int c_serial::open(std::string port_name)
{
    this->port =
        CreateFileA
        (
            port_name.data(), GENERIC_WRITE | GENERIC_READ,
            0, nullptr, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, nullptr
        );

    if (this->port == (HANDLE)-1)
    {
        printf("c_serial::open() - port not found\n");
        system("pause >nul");
        return 1;
    }

    memset(&this->params, 0, sizeof(this->params));

    if (!GetCommState(this->port, &this->params))
    {
        printf("c_serial::open() - can't get params\n");
        this->close();
        system("pause >nul");
        return 1;
    }

    this->params.BaudRate = 9600;
    this->params.ByteSize = 8;
    this->params.StopBits = 0;
    this->params.Parity = 0;
    this->params.fDtrControl = DTR_CONTROL_ENABLE;

    if (!SetCommState(this->port, &this->params))
    {
        printf("c_serial::open() - can't set params\n");
        this->close();
        system("pause >nul");
        return 1;
    }

    PurgeComm(this->port, PURGE_RXCLEAR | PURGE_TXCLEAR);

    printf("c_serial::open() - connected!!!\n");

    return 0;
}

void c_serial::close()
{
    if (this->port)
    {
        CloseHandle(this->port);
    }
}

std::string c_serial::read_string_until(char character)
{
    if (!this->port)
    {
        printf("c_serial::read_string_until() - port is null\n");
        return "";
    }

    static char buffer[this->max_bytes];

    clock_t start = clock();

    std::string final_string = "";

    do
    {
        ClearCommError(this->port, &this->errors, &this->status);

        memset(&buffer, 0, sizeof(buffer));

        unsigned long bytes_to_read = min(status.cbInQue, max_bytes);
        unsigned long bytes_read = 0;

        if (!ReadFile(this->port, (void*)buffer, bytes_to_read, &bytes_read, nullptr))
        {
            printf("c_serial::read_string_until() - readfile() failed\n");
            return "";
        }

        if (bytes_read != bytes_to_read)
        {
            continue;
        }

        final_string += buffer;

        std::size_t pos = final_string.find_last_of(character);

        if (pos != std::string::npos)
        {
            final_string.erase(final_string.begin() + pos, final_string.end());
            break;
        }
    }
    while (abs(clock() - start) <= this->timeout_ms);

    return final_string;
}

std::string c_serial::read_fast()
{
	if (!this->port)
	{
		printf("c_serial::read_string_until() - port is null\n");
		return "";
	}

	static char buffer[this->max_bytes];

	ClearCommError(this->port, &this->errors, &this->status);

	memset(&buffer, 0, sizeof(buffer));

	unsigned long bytes_to_read = min(status.cbInQue, max_bytes);
	unsigned long bytes_read = 0;

	if (!ReadFile(this->port, (void*)buffer, bytes_to_read, &bytes_read, nullptr))
	{
		printf("c_serial::read_string_until() - readfile() failed\n");
		return "";
	}

	return buffer;
}

void c_serial::send_string(std::string string)
{
    if (!string.length())
    {
        return;
    }

    static char buffer[this->max_bytes];
    memset(&buffer, 0, sizeof(buffer));

    for (std::size_t i = 0; i < string.length(); i++)
    {
        buffer[i] = string[i];
    }

    unsigned long bytes_to_send = string.length();
    unsigned long bytes_sent = 0;

	clock_t start = clock();

	do
	{
		ClearCommError(this->port, &this->errors, &this->status);

		if (!WriteFile(this->port, (void*)buffer, bytes_to_send, &bytes_sent, nullptr))
		{
			printf("c_serial::send_string() - writefile() failed\n");
		}

		if (bytes_sent == bytes_to_send)
		{
			break;
		}
	}
	while (abs(clock() - start) <= this->timeout_ms);
}

#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

static const std::string base64 =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

auto encode(const std::string& data)
{
	std::string teamp;
	std::array<unsigned char, 3> char_arr;

	for (auto begin = data.begin(), end = data.end(), it = begin; it != end; ++it)
	{
		auto i = it - begin;

		//Read a group 3 bytes (avoid buffer overrun by replacing with 0)
		
		char_arr[i % char_arr.size()] = *it;

		if (i % char_arr.size() != char_arr.size() - 1) continue;

		//Transform base 64 characters
		teamp += base64[(char_arr[0] & 0xfc) >> 2];
		teamp += base64[((char_arr[0] & 0x03) << 4) + ((char_arr[1] & 0xf0) >> 4)];
		teamp += base64[((char_arr[1] & 0x0f) << 2) + ((char_arr[2] & 0xc0) >> 6)];
		teamp += base64[((char_arr[2] & 0x3f) << 0)];
	}

	return teamp;
}

auto FromFile(const std::string& file)
{
	std::ifstream f;
	std::ostringstream o;
	f.open(file, std::ios::binary);
	o << f.rdbuf();
	f.close();
	return o.str();
}

int main()
{
	std::string enc = encode(FromFile("test.png"));
	std::ofstream file;
	file.open("test.pin");
	file << enc;
	system("PAUSE");
}

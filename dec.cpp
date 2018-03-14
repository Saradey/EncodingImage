#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(const std::string& data)
{
	std::array<unsigned char, 4> char_array;
	std::string ret;

	for (auto begin = data.begin(), end = data.end(), it = begin; it != end; ++it)
	{
		if (!is_base64(*it)) break;

		auto index = it - begin;

		char_array[index % char_array.size()] = *it;

		if (index % char_array.size() != char_array.size() - 1) continue;

		for (auto& character : char_array)
			character = static_cast<unsigned char>(base64_chars.find(character));

		ret += (char_array[0] << 2) + ((char_array[1] & 0x30) >> 4);
		ret += ((char_array[1] & 0xf) << 4) + ((char_array[2] & 0x3c) >> 2);
		ret += ((char_array[2] & 0x3) << 6) + char_array[3];
	}

	return ret;
}

std::string stringFromFile(const std::string& file)
{
	std::ifstream fin;
	fin.open(file, std::ios::binary);
	std::ostringstream out;
	out << fin.rdbuf();
	fin.close();
	return out.str();
}

int main()
{
	std::string decoded = base64_decode(stringFromFile("test.pin"));

	sf::Texture texture;
	if (!texture.loadFromMemory(decoded.c_str(), decoded.length()))
		return 1;

	sf::Sprite sprite(texture);
	sf::Vector2u size(static_cast<unsigned>(sprite.getLocalBounds().width / 2u), static_cast<unsigned>(sprite.getLocalBounds().height / 2u));

	sf::RenderWindow window(sf::VideoMode(size.x, size.y), "SFML");
	window.setVerticalSyncEnabled(true);

	sprite.setScale(window.getSize().x / sprite.getLocalBounds().width, window.getSize().y / sprite.getLocalBounds().height);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}
}

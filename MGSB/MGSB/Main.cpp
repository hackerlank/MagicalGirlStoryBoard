#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>

int main() {
	std::string destinationPath = R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\)"
								  R"(MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry\)"
								  R"(MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry. (osuuci dot com).osb)";
	Storyboard::Instance()->Write(destinationPath);
	std::cout << "Generation complete" << std::endl;

	std::string wait;
	std::cin >> wait;

	return 0;
}
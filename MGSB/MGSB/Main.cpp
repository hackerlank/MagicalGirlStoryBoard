#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>

int main() {
	Storyboard storyboard;
	std::string destinationPath = R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\
								     MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry\
								     MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry. (osuuci dot com) [Storyboard Test].osb)";
	storyboard.Write(destinationPath);

	return 0;
}
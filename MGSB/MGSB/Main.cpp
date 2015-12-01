#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>
#include "LyricInfoManager.cpp"
#include <vector>
#include <time.h>

int main() {
	srand(time(NULL));
	std::string lyricPath("..\\LyricGeneration\\Lyrics");
	std::vector<LyricInfo> lyricInfos = LyricInfoManager::Instance()->Read(lyricPath + "lyricsInfo.txt");
	int ending = 213513;
	double scale = 0.5;
	Vector2 mid(320, 240);

	for (int i = 0; i < lyricInfos.size(); ++i) {
		std::string path = lyricPath + std::to_string(lyricInfos[i].id);
		Vector2 size(lyricInfos[i].width, lyricInfos[i].height);
		Sprite sprite = Sprite(path, size, mid);
		sprite.Scale(0, ending, scale, scale);
		sprite.Fade(lyricInfos[i].timing - 500, lyricInfos[i].timing, 0.0, 1.0);

		double width = sprite.size.x * sprite.scale / 2.0;
		double diameter;

		// Move all the ones in the back
		// Find distance to move
		// Choose random direction
		// 
		for (int j = i - 1; j >= 0; --j) {
			
		}

		

		double base = 0.4;
		double variance = rand() % 20 / 100.0;
		if (i < lyricInfos.size() - 1) {
			base = 0.4;
			variance = rand() % 20 / 100.0;
			double fade = base + variance;
			sprite.Fade(lyricInfos[i + 1].timing - 500, lyricInfos[i].timing, 1.0, fade);
		}
	}

	for (auto sprite : Storyboard::Instance()->sprites) {
		sprite.Fade(ending - 500, ending, sprite.fade, 0.0);
	}

	std::string destinationPath = R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\)"
		R"(MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry\)"
		R"(MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry. (osuuci dot com).osb)";
	Storyboard::Instance()->Write(destinationPath);
	std::cout << "Generation complete" << std::endl;

	return 0;
}

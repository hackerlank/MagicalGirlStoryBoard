#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>
#include "LyricInfoManager.cpp"
#include <vector>

int main() {
  std::string lyricPath("..\\LyricGeneration\\Lyrics");
  std::vector<LyricInfo> lyricInfos = LyricInfoManager::Instance()->Read(lyricPath + "lyricsInfo.txt");

  

  for (auto lyric : lyricInfo) {
    // Place lyric
    // Move to it
    // Move/rotate everything else
  }
  

  

	std::string destinationPath = R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\)"
								  R"(MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry\)"
								  R"(MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry. (osuuci dot com).osb)";
	Storyboard::Instance()->Write(destinationPath);
	std::cout << "Generation complete" << std::endl;

	std::string wait;
	std::cin >> wait;

	return 0;
}

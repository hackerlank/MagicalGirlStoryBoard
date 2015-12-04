#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>
#include "LyricInfoManager.cpp"
#include <vector>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

int main() {
	srand(time(NULL));
	std::string lyricPath("..\\LyricGeneration\\Lyrics\\");
	std::vector<LyricInfo> lyricInfos = LyricInfoManager::Instance()->Read(lyricPath + "lyricsInfo.txt");
	int ending = 213513;
	double scale = 0.5;
	Vector2 mid(320, 240);
	std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

	for (int i = 0; i < lyricInfos.size(); ++i) {
		std::wcout << "Processing: " << lyricInfos[i].id << std::endl;
		std::wcout << lyricInfos[i].kanji << std::endl;
		std::wcout << lyricInfos[i].english << std::endl;

		std::string path = lyricPath + std::to_string(lyricInfos[i].id) + ".png";
		Vector2 size(lyricInfos[i].width, lyricInfos[i].height);
		Sprite sprite = Sprite(path, size, mid, scale);
		sprite.Scale(0, ending, scale, scale);
		sprite.Fade(lyricInfos[i].timing - 500, lyricInfos[i].timing, 0.0, 1.0);

		if (i < lyricInfos.size() - 1) {
			double base = 0.3;
			double variance = rand() % 40 / 100.0;
			double fade = base + variance;
			sprite.Fade(lyricInfos[i + 1].timing - 500, lyricInfos[i].timing, 1.0, fade);
		}

		// Ignore calculations for first lyric
		if (i == 0) {
			continue;
		}

		Sprite previous = Storyboard::Instance()->sprites[i - 1];
		double diameter = sprite.radius + previous.radius;
		bool foundMatch = false;

		while (!foundMatch) {
			double degrees = rand() % 360;
			double radians = degrees * M_PI / 180.0;
			Vector2 move(cos(radians), sin(radians));
			move *= diameter;

			// Spherical collisions by comparing vectors
			for (int j = i - 1; j >= 0; --j) {
				Sprite old = Storyboard::Instance()->sprites[j];
				// Copy
				Vector2 targetPos = old.position + move;
				Vector2 difference = sprite.position - targetPos;

				// Checks overlap by comparing distance between the two
				// sprites with the sum of their radii
				if (difference.magnitude() < sprite.radius + old.radius) {
					break;
				}
				else if (j == 0) {
					foundMatch = true;
				}
			}

			// If collisions are fine, then move everything
			if (foundMatch) {
				std::wcout << "Found Match" << std::endl;
				// Second rotation to account for a rotation of the new sprite
				degrees = rand() % 360 - 180;
				double rotation = degrees * M_PI / 180.0;
				// int iterations = 100;
				int endTime = lyricInfos[i].timing;
				int startTime = endTime - 500;
				
				for (int j = i - 1; j >= 0; --j) {
					Sprite old = Storyboard::Instance()->sprites[j];
					old.Rotate(startTime, endTime, old.rotation, rotation);

					Vector2 movePos = old.position + move;
					Vector2 fromMid = mid - movePos;
					Vector2 endMove(cos(rotation), sin(rotation));
					endMove *= fromMid.magnitude();
					old.Move(startTime, endTime, old.position.x, old.position.y, endMove.x, endMove.y);
					std::cout << "j: " << j << " Move: " << endMove.x << " " << endMove.y << std::endl;

					// Not sure if I have to account for smoother iterations
					// Started on some code that may not be necessary
					//for (int k = 0; k < iterations; ++k) {
					//	double startRotation = k * (rotation / iterations);
					//	double endRotation = (k + 1) * (rotation / iterations);
					//	Vector2 startMove = old.position;
					//	Vector2 endMove(cos(endRotation), sin(endRotation));
					//	Vector2 previousMove = move * k / iterations;
					//	Vector2 previousPos = startMove - endMove * k 
					//	old.Move(startTime, endTime, startMove.x, startMove.y, endMove.x, endMove.y);
					//}
				}
			}
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

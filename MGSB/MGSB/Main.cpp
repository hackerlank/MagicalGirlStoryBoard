#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>
#include "LyricInfoManager.cpp"
#include <vector>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector2Pair.hpp"

// Checks if two sprites collide with each other
// By the nature of this project, s1 is always upright and located at 320,240
bool collision(Sprite s1, Sprite s2) {
	// Quick check that also removes some cross cases
	double diff = (s1.position - s2.position).Magnitude();
	if (diff < fmin(fmin(s1.size.x, s1.size.y),
		fmin(s2.size.x, s2.size.y))) {
		return true;
	}

	// Move everything to (0,0)
	Vector2 translate(s1.position);
	s1.position -= translate;
	s2.position -= translate;

	// Calculate four corners of s2
	double halfWidth = s2.size.x / 2;
	Vector2 right(cos(s2.rotation) * halfWidth, sin(s2.rotation) * halfWidth);
	Vector2 left = right * -1;
	double halfHeight = s2.size.y / 2;
	Vector2 up(cos(s2.rotation + M_PI / 2) * halfHeight, sin(s2.rotation + M_PI / 2) * halfHeight);
	Vector2 down = up * -1;

	Vector2 TR = s2.position + right + up;
	Vector2 TL = s2.position + left + up;
	Vector2 BR = s2.position + right + down;
	Vector2 BL = s2.position + left + down;

	// Four lines
	std::vector<Vector2Pair> lines;
	lines.push_back(Vector2Pair(TR, TL));
	lines.push_back(Vector2Pair(TR, BR));
	lines.push_back(Vector2Pair(TL, BL));
	lines.push_back(Vector2Pair(BL, BR));

	// Boundaries of s1
	double topBoundary = s1.position.y + s1.size.y / 2;
	double bottomBoundary = s1.position.y - s1.size.y / 2;
	double leftBoundary = s1.position.x - s1.size.x / 2;
	double rightBoundary = s1.position.x + s1.size.x / 2;

	for (auto pair : lines) {
		Vector2 p1 = pair.x;
		Vector2 p2 = pair.y;

		if ((p1.x < leftBoundary && p2.x < leftBoundary) ||
			(p2.x > rightBoundary && p2.x > rightBoundary)) {
			continue;
		}

		double slope = (p2.y - p1.y) / (p2.x - p1.x);

		// Point-slope form
		// Compares to left boundary
		double yIntercept = slope * (leftBoundary - p1.x) + p1.y;
		// Indicates collision if the intercept is between top and bottom of s1
		if (yIntercept < topBoundary && yIntercept > bottomBoundary) {
			return true;
		}

		// Compares to right boundary
		yIntercept = slope * (rightBoundary - p1.x) + p1.y;
		if (yIntercept < topBoundary && yIntercept > bottomBoundary) {
			return true;
		}
	}
	return false;
}

int main() {
	srand(time(NULL));
	std::string lyricPath(R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\367782 MikitoP ft Sana - I'm Just an Average Magical Girl, Sorry\Lyrics\)");
	std::vector<LyricInfo> lyricInfos = LyricInfoManager::Instance()->Read(lyricPath + "lyricsInfo.txt");
	int ending = 213513;
	double scale = 0.3;
	Vector2 mid(320, 240);
	std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

	for (int i = 0; i < lyricInfos.size(); ++i) {
		std::wcout << "Processing: " << lyricInfos[i].id << std::endl;
		std::wcout << lyricInfos[i].kanji << std::endl;
		std::wcout << lyricInfos[i].english << std::endl;

		std::string path = "Lyrics/" + std::to_string(lyricInfos[i].id) + ".png";
		Vector2 size(lyricInfos[i].width, lyricInfos[i].height);
		Sprite* sprite = new Sprite(path, mid, size, scale);
		sprite->Scale(0, ending, scale, scale);

		if (i < lyricInfos.size() - 1) {
			sprite->Fade(lyricInfos[i].timing - 500, lyricInfos[i].timing, 0.0, 1.0);
			sprite->Fade(lyricInfos[i].timing, lyricInfos[i + 1].timing - 500, 1.0, 1.0);
			// 30-70% opacity
			double base = 0.3;
			double variance = rand() % 40 / 100.0;
			double fade = base + variance;
			sprite->Fade(lyricInfos[i + 1].timing - 500, lyricInfos[i].timing, 1.0, fade);
		}

		else {
			sprite->Fade(lyricInfos[i].timing - 500, ending, 0.0, 1.0);
		}

		// Ignore calculations for first lyric
		if (i == 0) {
			continue;
		}

		Sprite* previous = Storyboard::Instance()->sprites[i - 1];

		bool foundMatch = false;
		while (!foundMatch) {
			// First rotation to decide where to place
			double degrees = rand() % 360;
			double radians = degrees * M_PI / 180.0;
			Vector2 move(cos(radians), sin(radians));

			// Second rotation that rotates everything around center
			degrees = rand() % 90 - 45;
			double rotation = degrees * M_PI / 180.0;
			int endTime = lyricInfos[i].timing;
			int startTime = endTime - 500;

			// Find distance to move previous sprite, push out
			// Set distance as minimum of width and height
			double distance = fmin(fmin(sprite->size.x, sprite->size.y),
				fmin(previous->size.x, previous->size.y));
			Sprite s1 = *sprite;
			// Copy
			Sprite s2 = *previous;

			while (collision(s1, s2)) {
				++distance;
				s2 = *previous;

				s2.position += move * distance;
				s2.position.RotateAround(mid, rotation);
				s2.rotation += rotation;
			}

			move *= distance;

			// Check if all previous collisions are fine
			for (int j = i - 1; j >= 0; --j) {
				Sprite old = *Storyboard::Instance()->sprites[j];

				old.position += move;
				old.position = old.position.RotateAround(mid, rotation);
				old.rotation += rotation;

				if (collision(s1, old)) {
					break;
				}
				else if (j == 0) {
					foundMatch = true;
				}
			}

			// If collisions are fine, then move everything
			if (foundMatch) {
				std::wcout << "Found Match" << std::endl;

				for (int j = i - 1; j >= 0; --j) {
					Sprite* old = Storyboard::Instance()->sprites[j];

					Vector2 endMove = old->position + move;
					endMove.RotateAround(mid, rotation);
					old->Move(startTime, endTime, old->position.x, old->position.y, endMove.x, endMove.y);
					old->Rotate(startTime, endTime, old->rotation, old->rotation + rotation);
				}
			}
		}
	}

	for (auto sprite : Storyboard::Instance()->sprites) {
		sprite->Fade(ending, ending + 500, sprite->fade, 0.0);
	}

	Sprite* sprite = new Sprite("blank.png", mid, Vector2(1366, 768), 1.0, Layer::Background);
	sprite->Color(0, ending, 0, 0, 0, 0, 0, 0);
	sprite->Fade(-500, 0, 0.0, 1.0);
	sprite->Fade(ending, ending + 500, 1.0, 0.0);


	std::string destinationPath = R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\)"
		R"(367782 MikitoP ft Sana - I'm Just an Average Magical Girl, Sorry\)"
		R"(MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry. (osuuci dot com).osb)";
	Storyboard::Instance()->Write(destinationPath);
	std::cout << "Generation complete" << std::endl;

	// Cleanup
	for (auto sprite : Storyboard::Instance()->sprites) {
		delete sprite;
	}

	//std::cout << "Storyboard generation complete" << std::endl;
	//std::cin.get();

	return 0;
}

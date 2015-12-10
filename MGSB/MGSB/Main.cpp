#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <iostream>
#include <string>
#include "LyricInfoManager.cpp"
#include <vector>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Rectangle.hpp"
#include "Vector2Pair.hpp"

// Base is the base sprite that is always centered at 320,240
// Checks collision with a sprite s that may have an additional rotation applied to it
bool collision(Sprite* base, Sprite* s, double rotation = 0.0) {
	// Made a rectangle class just to handle collisions
	// I should probably put this inside Sprite and have it updated, but I'm a little weary
	// of making changes at this time
	Rectangle r1(base->size, base->position, 0.0);
	Rectangle r2(s->size, s->position, s->rotation + rotation);

	// Move everything to (0,0)
	Vector2 translate(base->position);
	r1.center -= translate;
	r2.center -= translate;

	// Calculate four corners of r2
	double halfWidth = r2.size.x / 2;
	Vector2 right(cos(r2.rotation) * halfWidth, sin(r2.rotation) * halfWidth);
	Vector2 left = right * -1;
	double halfHeight = r2.size.y / 2;
	Vector2 up(cos(r2.rotation + M_PI / 2) * halfHeight, sin(r2.rotation + M_PI / 2) * halfHeight);
	Vector2 down = up * -1;

	Vector2 TR = r2.center + right + up;
	Vector2 TL = r2.center + left + up;
	Vector2 BR = r2.center + right + down;
	Vector2 BL = r2.center + left + down;

	// Four lines
	std::vector<Vector2Pair> lines;
	lines.push_back(Vector2Pair(TR, TL));
	lines.push_back(Vector2Pair(TR, BR));
	lines.push_back(Vector2Pair(TL, BL));
	lines.push_back(Vector2Pair(BL, BR));

	// Boundaries of r1
	double topBoundary = r1.center.y + r1.size.y / 2;
	double bottomBoundary = r1.center.y - r1.size.y / 2;
	double leftBoundary = r1.center.x - r1.size.x / 2;
	double rightBoundary = r1.center.x + r1.size.x / 2;

	for (auto pair : lines) {
		Vector2 p1 = pair.x;
		Vector2 p2 = pair.y;
		double slope = (p2.y - p1.y) / (p2.x - p1.x);

		// Point-slope form
		// Compares to left boundary
		double yIntercept = slope * (leftBoundary - p1.x) + p1.y;
		// Indicates collision if the intercept is between top and bottom of r1
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
		double diameter = sprite->radius + previous->radius;

		bool foundMatch = false;
		while (!foundMatch) {
			double degrees = rand() % 360;
			double radians = degrees * M_PI / 180.0;
			Vector2 move(cos(radians), sin(radians));
			move *= diameter;

			// Second rotation that rotates everything around center
			degrees = rand() % 90 - 45;
			double rotation = degrees * M_PI / 180.0;
			// int iterations = 100;
			int endTime = lyricInfos[i].timing;
			int startTime = endTime - 500;

			// Find distance to move previous sprite, push out

			// Check if all previous collisions are fine
			// Replace below
			// Spherical collisions by comparing vectors
			for (int j = i - 1; j >= 0; --j) {
				Sprite* old = Storyboard::Instance()->sprites[j];

				// Copy
				Vector2 targetPos = old->position + move;
				Vector2 difference = sprite->position - targetPos;

				// Checks overlap by comparing distance between the two
				// sprites with the sum of their radii
				if (difference.magnitude() < sprite->radius + old->radius) {
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

					Vector2 movePos = old->position + move;
					Vector2 fromMid = movePos - mid;

					Vector2 unitVec(1,0);
					double angleFrom0 = fromMid.angleBetween(unitVec);
					if (fromMid.y < 0.0) {
						angleFrom0 *= -1.0;
					}
					angleFrom0 += rotation;
					Vector2 endMove(cos(angleFrom0), sin(angleFrom0));
					endMove *= fromMid.magnitude();
					endMove = mid + endMove;
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

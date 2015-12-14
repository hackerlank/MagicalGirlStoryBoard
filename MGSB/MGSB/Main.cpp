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
	// Performs a quick check comparing radii
	if ((s1.position - s2.position).Magnitude() > s1.radius + s2.radius) {
		return false;
	}

	// Move everything to (0,0)
	Vector2 translate(s1.position);
	s1.position -= translate;
	s2.position -= translate;

	// Corners of s1
	double halfWidth1 = s1.size.x / 2;
	double halfHeight1 = s1.size.y / 2;
	Vector2 TR1 = s1.position + Vector2(halfWidth1, halfHeight1);
	Vector2 TL1 = s1.position + Vector2(-halfWidth1, halfHeight1);
	Vector2 BR1 = s1.position + Vector2(halfWidth1, -halfHeight1);
	Vector2 BL1 = s1.position + Vector2(-halfWidth1, -halfHeight1);
	std::vector<Vector2> s1corners = { TR1, TL1, BR1, BL1 };

	// Corners of s2
	double halfWidth2 = s2.size.x / 2;
	Vector2 right2(cos(s2.rotation) * halfWidth2, sin(s2.rotation) * halfWidth2);
	Vector2 left2 = -right2;
	double halfHeight2 = s2.size.y / 2;
	Vector2 up2(cos(s2.rotation + M_PI / 2) * halfHeight2, sin(s2.rotation + M_PI / 2) * halfHeight2);
	Vector2 down2 = -up2;
	Vector2 TR2 = s2.position + right2 + up2;
	Vector2 TL2 = s2.position + left2 + up2;
	Vector2 BR2 = s2.position + right2 + down2;
	Vector2 BL2 = s2.position + left2 + down2;
	std::vector<Vector2> s2corners = { TR2, TL2, BR2, BL2 };

	// Axes
	Vector2 axis1 = TR2 - TL2;
	Vector2 axis2 = TR2 - BR2;
	std::vector<Vector2> axes = { Vector2(1, 0), Vector2(0, 1), axis1, axis2 };

	for (auto axis : axes) {
		double min1 = s1corners[0].Dot(axis);
		double max1 = s1corners[0].Dot(axis);
		for (auto corner : s1corners) {
			Vector2 projection = corner.Project(axis);
			double scalar = projection.Dot(axis);
			if (scalar < min1) {
				min1 = scalar;
			}
			if (scalar > max1) {
				max1 = scalar;
			}
		}
		
		double min2 = s2corners[0].Dot(axis);
		double max2 = s2corners[0].Dot(axis);
		for (auto corner : s2corners) {
			Vector2 projection = corner.Project(axis);
			double scalar = projection.Dot(axis);
			if (scalar < min2) {
				min2 = scalar;
			}
			if (scalar > max2) {
				max2 = scalar;
			}
		}
		
		// Checks for no overlaps
		if (min2 >= max1 || max2 <= min1) {
			return false;
		}
	}

	return true;
}

int main() {
	srand(time(NULL));
	std::string lyricPath(R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\367782 MikitoP ft Sana - I'm Just an Average Magical Girl, Sorry\Lyrics\)");
	std::vector<LyricInfo> lyricInfos = LyricInfoManager::Instance()->Read(lyricPath + "lyricsInfo.txt");
	int ending = 213513;
	double scale = 0.35;
	double squeeze = scale * 0.97;
	Vector2 mid(320, 240);
	std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
	// Controls which way to spin
	bool leftSpin = true;
	// Millisecond per beat
	double mspb = 60000.0 / 155;

	for (int i = 0; i < lyricInfos.size(); ++i) {
		std::wcout << "Processing: " << lyricInfos[i].id << std::endl;
		std::wcout << lyricInfos[i].kanji << std::endl;
		std::wcout << lyricInfos[i].english << std::endl;

		std::string path = "Lyrics/" + std::to_string(lyricInfos[i].id) + ".png";
		Vector2 size(lyricInfos[i].width, lyricInfos[i].height);
		Sprite* sprite = new Sprite(path, mid, size, scale);
		sprite->Scale(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i].timing, scale * 0.75, scale);

		// For choosing when to squeeze or use normal scale
		bool offBeat = false;

		// Spin applied to base sprite
		// -5 to 0 degrees;
		double degrees = rand() % 5 - 5;
		double spinStart = degrees * M_PI / 180.0;
		// 5 to 10 degrees;
		degrees = rand() % 5 + 5;
		double spinAdd = degrees * M_PI / 180.0;
		if (leftSpin) {
			spinStart *= -1;
			spinAdd *= -1;
			leftSpin = false;
		}
		else {
			leftSpin = true;
		}
		double spinEnd = spinStart + spinAdd;

		if (i < lyricInfos.size() - 1) {
			int scaleTime = lyricInfos[i].timing;
			for (; scaleTime < lyricInfos[i + 1].timing - mspb * 1.5; scaleTime += mspb) {
				if (offBeat) {
					sprite->Scale(scaleTime, scaleTime + mspb, squeeze, scale);
					offBeat = false;
				}
				else {
					sprite->Scale(scaleTime, scaleTime + mspb, scale, squeeze);
					offBeat = true;
				}
			}
			if (offBeat) {
				sprite->Scale(scaleTime, scaleTime + mspb, squeeze, scale);
				offBeat = false;
				sprite->Scale(scaleTime + mspb, ending, scale, scale);
			}
			else {
				sprite->Scale(scaleTime, ending, scale, scale);
			}

			sprite->Rotate(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i + 1].timing - mspb * 1.5, spinStart, spinEnd);

			sprite->Fade(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i].timing, 0.0, 1.0);
			sprite->Fade(lyricInfos[i].timing, lyricInfos[i + 1].timing - mspb * 1.5, 1.0, 1.0);
			// 30-70% opacity
			double base = 0.3;
			double variance = rand() % 40 / 100.0;
			double fade = base + variance;
			sprite->Fade(lyricInfos[i + 1].timing - mspb * 1.5, lyricInfos[i].timing, 1.0, fade);
		}

		else {
			// Only do the scale squeeze if not the last lyric line
			sprite->Scale(lyricInfos[i].timing, ending, scale, scale);

			sprite->Rotate(lyricInfos[i].timing - mspb * 1.5, ending, spinStart, spinEnd);
			sprite->Fade(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i].timing, 0.0, 1.0);
		}

		// Ignore calculations for first lyric
		if (i == 0) {
			continue;
		}

		Sprite* previous = Storyboard::Instance()->sprites[i - 1];

		bool foundMatch = false;
		while (!foundMatch) {
			// First rotation to decide where to place
			degrees = rand() % 360;
			double radians = degrees * M_PI / 180.0;
			Vector2 move(cos(radians), sin(radians));

			// Second rotation that rotates everything around center
			degrees = rand() % 90 - 45;
			double rotation = degrees * M_PI / 180.0;

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
				old.position.RotateAround(mid, rotation);
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

				int lyricTime = lyricInfos[i].timing;
				int startTime = lyricTime - mspb * 1.5;
				int endTime;
				if (i < lyricInfos.size() - 1) {
					endTime = lyricInfos[i + 1].timing - mspb * 1.5;
				}
				else {
					endTime = ending;
				}

				for (int j = i - 1; j >= 0; --j) {
					Sprite* old = Storyboard::Instance()->sprites[j];

					Vector2 endMove = old->position + move;
					endMove.RotateAround(mid, rotation + spinStart);
					old->Move(startTime, lyricTime, old->position.x, old->position.y, endMove.x, endMove.y);
					old->Rotate(startTime, lyricTime, old->rotation, old->rotation + rotation + spinStart);

					endMove = old->position;
					endMove.RotateAround(mid, spinAdd);
					old->Move(lyricTime, endTime, old->position.x, old->position.y, endMove.x, endMove.y);
					old->Rotate(lyricTime, endTime, old->rotation, old->rotation + spinAdd);
				}
			}
		}
	}

	for (auto sprite : Storyboard::Instance()->sprites) {
		sprite->Fade(ending, ending + mspb * 1.5, sprite->fade, 0.0);
	}

	Sprite* sprite = new Sprite("blank.png", mid, Vector2(1366, 768), 1.0, Layer::Background);
	sprite->Color(0, ending, 0, 0, 0, 0, 0, 0);
	sprite->Fade(-mspb * 1.5, 0, 0.0, 1.0);
	sprite->Fade(ending, ending + mspb * 1.5, 1.0, 0.0);

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

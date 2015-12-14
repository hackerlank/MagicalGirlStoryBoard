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
	std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
	
	Vector2 mid(320, 240);
	int ending = 213513;
	// Millisecond per beat
	double mspb = 60000.0 / 155;

	std::string lyricPath(R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\367782 MikitoP ft Sana - I'm Just an Average Magical Girl, Sorry\Lyrics\)");
	std::vector<LyricInfo> lyricInfos = LyricInfoManager::Instance()->Read(lyricPath + "lyricsInfo.txt");

	// Handle background
	Sprite* background = new Sprite("blank.png", mid, Vector2(1366, 768), 1.0, Layer::Background);
	Color backgroundColor = Colors[rand() % Colors.size()];
	while (backgroundColor == Color(0, 0, 0)) {
		backgroundColor = Colors[rand() % Colors.size()];
	}
	background->Fade(-mspb * 1.5, 0, 0.0, 1.0);
	background->Fade(ending, ending + mspb * 1.5, 1.0, 0.0);
	background->Color(-mspb * 1.5, lyricInfos.front().timing - mspb * 1.5, backgroundColor.r, backgroundColor.g, backgroundColor.b, 
																	       backgroundColor.r, backgroundColor.g, backgroundColor.b);

	// Swirl
	Sprite* swirl = new Sprite("swirl.png", mid, Vector2(0, 0), 1.0, Layer::Background);
	Color swirlColor = Colors[rand() % Colors.size()];
	while (swirlColor == backgroundColor || swirlColor == Color(255, 255, 255)) {
		swirlColor = Colors[rand() % Colors.size()];
	}
	double swirlScale = 0.20;
	double startSwirl = 6800;
	double endSwirl = lyricInfos.back().timing;
	double fadeSwirl = 0.25;
	swirl->Scale(0, ending, swirlScale, swirlScale);
	swirl->Move(startSwirl, ending, mid.x, mid.y, mid.x, mid.y);
	swirl->Fade(startSwirl - mspb * 1.5, startSwirl, 0.0, fadeSwirl);
	swirl->Fade(endSwirl, endSwirl + mspb * 1.5, fadeSwirl, 0.0);
	swirl->Color(0, lyricInfos.front().timing - mspb * 1.5, swirlColor.r, swirlColor.g, swirlColor.b,
															swirlColor.r, swirlColor.g, swirlColor.b);
	double swirlLength = endSwirl - startSwirl + 3 * mspb;
	double swirlRate = mspb * 8;
	double swirls = 360 * swirlLength / swirlRate;
	swirls *= M_PI;
	swirls /= 180.0;
	swirl->Rotate(startSwirl - mspb * 1.5, endSwirl + mspb * 1.5, 0, swirls);

	// Handle lyrics
	std::vector<Sprite*> lyrics;
	double scale = 0.4;
	double squeeze = scale * 0.98;
	// Controls which way to spin
	bool leftSpin = true;

	for (int i = 0; i < lyricInfos.size(); ++i) {
		std::wcout << "Processing: " << lyricInfos[i].id << std::endl;
		std::wcout << lyricInfos[i].kanji << std::endl;
		std::wcout << lyricInfos[i].english << std::endl;
		
		std::string path = "Lyrics/" + std::to_string(lyricInfos[i].id) + ".png";
		Vector2 size(lyricInfos[i].width, lyricInfos[i].height);
		Sprite* sprite = new Sprite(path, mid, size, scale);
		lyrics.push_back(sprite);

		// Initital scale
		sprite->Scale(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i].timing, scale * 0.75, scale);
		// For choosing when to squeeze or use normal scale
		bool offBeat = false;

		// Assign colors
		Sprite* previous;
		if (i > 0) {
			previous = lyrics[i - 1];
		}
		else {
			previous = sprite;
		}
		Color spriteColor = previous->color;
		backgroundColor = background->color;
		swirlColor = swirl->color;
		while (backgroundColor == background->color) {
			backgroundColor = Colors[rand() % Colors.size()];
		}
		while (swirlColor == swirl->color || swirlColor == backgroundColor || swirlColor == Color(255, 255, 255)) {
			swirlColor = Colors[rand() % Colors.size()];
		}
		while (spriteColor == previous->color || spriteColor == backgroundColor || spriteColor == swirlColor) {
			spriteColor = Colors[rand() % Colors.size()];
		}
		background->Color(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i].timing, background->color.r, background->color.g, background->color.b, 
																				   backgroundColor.r, backgroundColor.g, backgroundColor.b);
		swirl->Color(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i].timing, swirl->color.r, swirl->color.g, swirl->color.b,
																			  swirlColor.r, swirlColor.g, swirlColor.b);
		sprite->Color(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i].timing, sprite->color.r, sprite->color.g, sprite->color.b,
																			   spriteColor.r, spriteColor.g, spriteColor.b);

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

		// If not last lyric
		if (i < lyricInfos.size() - 1) {
			// Do a squeeze effect
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

			// Fade
			double spriteFade = 0.3;
			sprite->Fade(lyricInfos[i].timing - mspb * 1.5, lyricInfos[i].timing, 0.0, 1.0);
			sprite->Fade(lyricInfos[i].timing, lyricInfos[i + 1].timing - mspb * 1.5, 1.0, 1.0);
			sprite->Fade(lyricInfos[i + 1].timing - mspb * 1.5, lyricInfos[i].timing, 1.0, spriteFade);

			// Since this isn't going to be ranked anyways, I don't really need to worry about SB Load, but
			// since I'm probably not doing any zooming in and out and stuff, this shouldn't affect too much anyways
			int limit = 5;
			if (i < lyricInfos.size() - limit) {
				sprite->Fade(lyricInfos[i + limit].timing - mspb * 1.5, lyricInfos[i + limit].timing, spriteFade, 0.0);
			}
		}
		// Handle last lyric
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
				// Copy since there could be errors here
				// and we don't want to make lasting changes
				Sprite old = *lyrics[j];

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
					Sprite* old = lyrics[j];

					Vector2 endMove = old->position + move;
					endMove.RotateAround(mid, rotation + spinStart);
					old->Move(startTime, lyricTime, old->position.x, old->position.y, endMove.x, endMove.y);
					old->Rotate(startTime, lyricTime, old->rotation, old->rotation + rotation + spinStart);

					endMove = old->position;
					endMove.RotateAround(mid, spinAdd);
					old->Move(lyricTime, endTime, old->position.x, old->position.y, endMove.x, endMove.y);
					old->Rotate(lyricTime, endTime, old->rotation, old->rotation + spinAdd);

					old->Color(startTime, lyricTime, old->color.r, old->color.g, old->color.b, spriteColor.r, spriteColor.g, spriteColor.b);
				}
			}
		}
	}

	// Fade all lyrics at end
	for (auto sprite : lyrics) {
		sprite->Fade(ending, ending + mspb * 1.5, sprite->fade, 0.0);
	}

	std::string destinationPath = R"(C:\Users\Wax Chug da Gwad\AppData\Local\osu!\Songs\)"
		R"(367782 MikitoP ft Sana - I'm Just an Average Magical Girl, Sorry\)"
		R"(MikitoP ft. Sana - I'm Just an Average Magical Girl, Sorry. (Osuuki).osb)";
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

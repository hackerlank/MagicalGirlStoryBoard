#include <pngwriter.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Font.hpp"
#include "Vector2.hpp"

std::string getFontPath(std::string prevFontPath) {
	std::string randomFont = Fonts[rand() % Font::COUNT];
	if (randomFont != prevFontPath)
		return randomFont;
	else
		return getFontPath(prevFontPath);
}

int generateFontSize(int width, std::string fontPath, std::string text, int fontSize = 5) {
	pngwriter test;
	int testWidth = test.get_text_width_utf8(&fontPath[0u], fontSize, &text[0u]);
	if (testWidth < width) {
		return generateFontSize (width, fontPath, text, ++fontSize);
	}
	else {
		return fontSize - 1;
	}
}

int getKanjiFontSize(std::string fontPath, std::string text) {
	// 600-750
	int kanjiWidth = rand() % 150 + 600;
	return generateFontSize(kanjiWidth, fontPath, text);
}

int getEnglishFontSize(std::string fontPath, std::string text) {
	// 70-90% of 800
	int englishWidth = (rand() % 20 + 70) * 800 / 1000;
	return generateFontSize(englishWidth, fontPath, text);
}

void generateImage(std::string fontPath, std::string kanji, int kanjiFontSize, std::string english, int englishFontSize, std::string destinationPath) {
	int buffer = 5;
	pngwriter test;
	int kanjiWidth = test.get_text_width_utf8(&fontPath[0u], kanjiFontSize, &kanji[0u]);
	// pngwriter says get_text_width_utf8 isn't exact, so I'm making a smaller buffer space
	int imageWidth = kanjiWidth + buffer * 2;

	// Not sure if fontSize is 1 to 1 with height <=O
	int kanjiHeight = kanjiFontSize;
	// Space between kanji and english 50-100
	int middleSpace = rand() % 50 + 50;
	int imageHeight = kanjiHeight + middleSpace;
	imageHeight += englishFontSize;
	imageHeight += buffer * 2;
	
	pngwriter image(imageWidth, imageHeight, 0, &destinationPath[0u]);

	Vector2 kanjiPosition(buffer, buffer);
	image.plot_text_utf8(&fontPath[0u], 10, kanjiPosition.x, kanjiPosition.y, 0.0, &kanji[0u], 255, 255, 255);

	int englishWidth = test.get_text_width_utf8(&fontPath[0u], englishFontSize, &english[0u]);
	Vector2 englishPosition;
	englishPosition.x = imageWidth / 2 - englishWidth / 2;
	englishPosition.y = buffer + kanjiHeight + middleSpace;
	image.plot_text_utf8(&fontPath[0u], 10, englishPosition.x, englishPosition.y, 0.0, &english[0u], 255, 255, 255);

	image.close();
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	std::ifstream file("lyrics.txt");
	std::string line;
	std::string prevFontPath = "";

	for (int i = 0; std::getline(file, line); i++) {
		std::cout << "Processing Line: " << line << std::endl;
		std::string fontPath = getFontPath(prevFontPath);

		std::string kanji = line;
		int kanjiFontSize = getKanjiFontSize(fontPath, line);

		std::getline(file, line);
		std::cout << "Processing Line: " << line << std::endl;

		std::string english = line;
		int englishFontSize = getEnglishFontSize(fontPath, line);

		std::string destinationPath("Lyrics\\" + std::to_string(i) + ".png");
		std::cout << "Generating image" << std::endl;
		generateImage(fontPath, kanji, kanjiFontSize, english, englishFontSize, destinationPath);
		std::cout << "Generating image complete" << std::endl;
	}

	std::cout << "Lyric generation complete" << std::endl;
	std::cin.get();

	return 0;
}
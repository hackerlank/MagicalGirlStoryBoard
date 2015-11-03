#include <pngwriter.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Font.hpp"

std::string getFontPath(std::string prevFontPath) {
	std::string randomFont = Fonts[rand() % Font::COUNT];
	if (randomFont != prevFontPath)
		return randomFont;
	else
		return getFontPath(prevFontPath);
}

int getKanjiFontSize(std::string fontPath, std::string text, int fontSize = 5) {
	// 600-750
	int kanjiWidth = rand() % 150 + 600;
	generateFontSize(kanjiWidth, fontPath, text, fontSize = 5);
}

int getEnglishFontSize(std::string fontPath, std::string text, int fontSize = 5) {
	// 70-90% of 800
	int englishWidth = (rand() % 20 + 70) * 800;
	generateFontSize(englishWidth, fontPath, text, fontSize = 5);
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

pngwriter generateImage(std::string fontPath, std::string kanji, int kanjiFontSize, std::string english, int englishFontSize, std::string destinationPath) {
	int buffer = 5;
	pngwriter test;
	int width = test.get_text_width_utf8(&fontPath[0u], kanjiFontSize, &kanji[0u]);
	// pngwriter says get_text_width_utf8 isn't exact, so I'm making a smaller buffer space
	width += buffer * 2;

	// Not sure if fontSize is 1 to 1 with height <=O
	int height = kanjiFontSize;
	// Space between kanji and english 50-100
	height += rand() % 50 + 50;
	height += englishFontSize;
	height += buffer * 2;
	
	pngwriter image(width, height, 0, &destinationPath[0u]);

	// Something something
	image.plot_text_utf8(&fontPath[0u], 10, 0, 0, 0.0, &kanji[0u], 255, 255, 255);
	image.plot_text_utf8(&fontPath[0u], 10, 0, 0, 0.0, &english[0u], 255, 255, 255);


	return pngwriter();
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	std::ifstream file("lyrics.txt");
	std::string line;
	std::string prevFontPath = "";

	for (int i = 0; std::getline(file, line); i++) {
		std::string fontPath = getFontPath(prevFontPath);

		std::string kanji = line;
		int kanjiFontSize = getKanjiFontSize(fontPath, line);

		std::getline(file, line);
		std::string english = line;
		int englishFontSize = getEnglishFontSize(fontPath, line);

		std::string destinationPath = i + ".png";
		generateImage(fontPath, kanji, kanjiFontSize, english, englishFontSize, destinationPath);
	}


	std::cin.get();

	return 0;
}
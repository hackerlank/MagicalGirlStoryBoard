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

// Calculate size that fits for kanji
int getKanjiFontSize(std::string kanji) {

	return 0;
}

int getEnglishFontSize(std::string english) {
	return 0;
}

pngwriter getImage(std::string kanji, int kanjiFontSize, std::string english, int englishFontSize) {

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
		int kanjiFontSize = getKanjiFontSize(line);

		std::getline(file, line);
		std::string english = line;
		int englishFontSize = getEnglishFontSize(line);

		pngwriter image = getImage(kanji, kanjiFontSize, english, englishFontSize);

		pngwriter image(800, 600, 0, i + ".png");

		
		image.plot_text_utf8(&fontPath[0u], 10, 0, 0, 0.0, &kanji[0u], 255, 255, 255);

		image.plot_text_utf8(&fontPath[0u], 10, 0, 0, 0.0, &english[0u], 255, 255, 255);
	}


	std::cin.get();

	return 0;
}
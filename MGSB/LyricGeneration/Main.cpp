#include "SFML\Graphics.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "Font.hpp"

sf::Font getFont(sf::Font prevFont) {
	sf::Font font;
	std::string randomFont = Fonts[rand() % Font::COUNT];
	font.loadFromFile(randomFont);

	if(font.getInfo().family == prevFont.getInfo().family) {
		return getFont(prevFont);
	}
	else {
		return font;
	}
}

int generateFontSize(int width, sf::Font font, std::string text, int fontSize = 5) {
	sf::Text test(sf::String(text), font, fontSize);
	
	while (ceil(test.getLocalBounds().width) < width) {
		test.setCharacterSize(++fontSize);
	}

	return fontSize - 1;
}

int getKanjiFontSize(sf::Font font, std::string text) {
	// 600-750
	int kanjiWidth = rand() % 150 + 600;
	return generateFontSize(kanjiWidth, font, text);
}

int getEnglishFontSize(sf::Font font, std::string text) {
	// 50-70% of 800
	int englishWidth = (rand() % 20 + 50) * 800 / 100;
	return generateFontSize(englishWidth, font, text);
}

void generateImage(sf::Font font, std::string kanji, int kanjiFontSize, std::string english, int englishFontSize, std::string destinationPath) {
	sf::Text kanjiText(sf::String(kanji), font, kanjiFontSize);
	int kanjiWidth = (int) ceil(kanjiText.getLocalBounds().width);
	int kanjiHeight = (int) ceil(kanjiText.getLocalBounds().height);

	sf::Text englishText(sf::String(english), font, englishFontSize);
	int englishWidth = (int) ceil(englishText.getLocalBounds().width);
	int englishHeight = (int) ceil(englishText.getLocalBounds().height);

	// Space between kanji and english 50-100
	int middleSpace = rand() % 50 + 50;
	int imageHeight = kanjiHeight + middleSpace + englishHeight;
	int imageWidth = kanjiWidth;
	
	sf::RenderTexture render;
	render.create(imageWidth, imageHeight);
	render.draw(kanjiText);

	sf::Vector2i englishPosition;
	englishPosition.x = imageWidth / 2 - englishWidth / 2;
	englishPosition.y = kanjiHeight + middleSpace;
	englishText.setPosition(sf::Vector2f(englishPosition));
	render.draw(englishText);
	
	render.display();

	render.getTexture().copyToImage().saveToFile(destinationPath);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	std::ifstream file("lyrics.txt");
	std::string line;

	// Skip byte order mark
	std::getline(file, line);

	sf::Font prevFont;

	for (int i = 0; std::getline(file, line); i++) {
		if (i == 1)
			break;

		std::cout << "Processing Line: " << line << std::endl;
		sf::Font font = getFont(prevFont);
		prevFont = font;

		std::string kanji = line;
		int kanjiFontSize = getKanjiFontSize(font, line);

		std::getline(file, line);
		std::cout << "Processing Line: " << line << std::endl;

		std::string english = line;
		int englishFontSize = getEnglishFontSize(font, line);

		std::string destinationPath("Lyrics\\" + std::to_string(i) + ".png");
		std::cout << "Generating image" << std::endl;
		generateImage(font, kanji, kanjiFontSize, english, englishFontSize, destinationPath);
		std::cout << "Generating image complete" << std::endl;
	}

	std::cout << "Lyric generation complete" << std::endl;
	std::cin.get();

	return 0;
}
#include "SFML\Graphics.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "Font.hpp"
#include <locale>
#include <codecvt>

int TARGET_WIDTH = 800 * 2;
int TARGET_HEIGHT = 600 * 2;

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

int generateFontSize(int width, sf::Font font, std::wstring text, int fontSize = 5) {
	sf::Text test(sf::String(text), font, fontSize);
	
	while (ceil(test.getLocalBounds().width) < width &&
		   ceil(test.getLocalBounds().height) < TARGET_HEIGHT / 2) { // Set general height limit
		test.setCharacterSize(++fontSize);
	}

	return --fontSize;
}

int getKanjiFontSize(sf::Font font, std::wstring text) {
	// 75-85% of TARGET_WIDTH
	double base = 0.75;
	double variance = rand() % 10 / 100.0;
	double percent = base + variance;
	int kanjiWidth = (int) (TARGET_WIDTH * percent);
	return generateFontSize(kanjiWidth, font, text);
}

int getEnglishFontSize(sf::Font font, std::wstring text) {
	// 60-70% of TARGET_WIDTH
	double base = 0.6;
	double variance = rand() % 10 / 100.0;
	double percent = base + variance;
	int englishWidth = (int) (TARGET_WIDTH * percent);
	return generateFontSize(englishWidth, font, text);
}

void generateImage(sf::Font font, std::wstring kanji, int kanjiFontSize, std::wstring english, int englishFontSize, std::string destinationPath) {
	sf::Text kanjiText(sf::String(kanji), font, kanjiFontSize);
	// The way SFML's getLocalBounds() works is pretty weird
	// For reason, you have to add the .left and .top values
	// to get the true values
	sf::FloatRect kanjiRect = kanjiText.getLocalBounds();
	int kanjiWidth = (int) ceil(kanjiRect.width + kanjiRect.left);
	int kanjiHeight = (int) ceil(kanjiRect.height + kanjiRect.top);

	sf::Text englishText(sf::String(english), font, englishFontSize);
	sf::FloatRect englishRect = englishText.getLocalBounds();
	int englishWidth = (int) ceil(englishRect.width + englishRect.left);
	int englishHeight = (int) ceil(englishRect.height + englishRect.top);

	// Space between kanji and english
	// 5-10% of height
	double base = 0.05;
	double variance = rand() % 5 / 100.0;
	double percent = base + variance;
	int middleSpace = (int) (TARGET_HEIGHT * percent);
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
	std::wifstream file("lyrics.txt");
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
	std::wcout.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
	std::wstring line;
	sf::Font prevFont;

	for (int i = 0; std::getline(file, line); i++) {
		std::wcout << "Processing Line " << i << ": " << line << std::endl;
		sf::Font font = getFont(prevFont);
		prevFont = font;

		std::wstring kanji = line;
		int kanjiFontSize = getKanjiFontSize(font, line);

		std::getline(file, line);
		std::wcout << "Processing Line: " << line << std::endl;

		std::wstring english = line;
		int englishFontSize = 0;
		if (english.length() > 0) {
			 englishFontSize = getEnglishFontSize(font, line);
		}

		std::string destinationPath("Lyrics\\" + std::to_string(i) + ".png");
		std::cout << "Generating image" << std::endl;
		generateImage(font, kanji, kanjiFontSize, english, englishFontSize, destinationPath);
		std::cout << "Generating image complete" << std::endl;
	}

	std::cout << "Lyric generation complete" << std::endl;
	std::cin.get();

	return 0;
}
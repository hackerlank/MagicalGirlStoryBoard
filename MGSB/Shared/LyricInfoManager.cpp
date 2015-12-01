#include "LyricInfoManager.hpp"
#include <fstream>
#include <locale>
#include <codecvt>

LyricInfoManager* LyricInfoManager::instance = NULL;

LyricInfoManager* LyricInfoManager::Instance() {
	if (!instance) {
		instance = new LyricInfoManager;
	}
	return instance;
}

void LyricInfoManager::Write(std::vector<LyricInfo> lyricInfos, std::string path) {
	std::wofstream file(path);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
	for (auto lyric : lyricInfos) {
		file << lyric.id << std::endl;
		file << lyric.timing << std::endl;
		file << lyric.kanji << std::endl;
		file << lyric.english << std::endl;
		file << lyric.width << std::endl;
		file << lyric.height << std::endl;
	}
}

std::vector<LyricInfo> LyricInfoManager::Read(std::string path) {
	std::vector<LyricInfo> lyricInfos;
	std::wifstream file(path);
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
	std::wstring line;
	while (std::getline(file, line)) {
		LyricInfo lyricInfo;
		lyricInfo.id = std::stoi(line);
		std::getline(file, line);
		lyricInfo.timing = std::stoi(line);
		std::getline(file, line);
		lyricInfo.kanji = line;
		std::getline(file, line);
		lyricInfo.english = line;
		std::getline(file, line);
		lyricInfo.width = std::stoi(line);
		std::getline(file, line);
		lyricInfo.height = std::stoi(line);

		lyricInfos.push_back(lyricInfo);
	}

	return lyricInfos;
}

#include "LyricInfoManager.hpp"
#include <fstream>

LyricInfoManager* LyricInfoManager::instance = NULL;

LyricInfoManager* LyricInfoManager::Instance() {
	if (!instance) {
		instance = new LyricInfoManager;
	}
	return instance;
}

void LyricInfoManager::Write(std::vector<LyricInfo> lyricInfos, std::string path) {
	std::wofstream file(path);

	for (auto lyric : lyricInfos) {
		file << lyric.id << std::endl;
		file << lyric.timing << std::endl;
		file << lyric.kanji << std::endl;
		file << lyric.english << std::endl;
		file << lyric.width << std::endl;
		file << lyric.height << std::endl;
	}
}

std::vector<LyricInfo> LyricInfoManager::Read(std:: path) {
  std::vector<LyricInfo> lyricInfos;
  std::wifstream file(path);
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

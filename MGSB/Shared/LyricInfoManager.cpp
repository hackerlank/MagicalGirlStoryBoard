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

	for(auto l : lyricInfos) {
		file << l.id << std::endl;
		file << l.timing << std::endl;
		file << l.kanji << std::endl;
		file << l.english << std::endl;
		file << l.width << std::endl;
		file << l.height << std::endl;
	}
}
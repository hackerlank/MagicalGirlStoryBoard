#ifndef LYRICINFOMANAGER_HPP
#define LYRICINFOMANAGER_HPP

#include "LyricInfo.hpp"
#include <vector>

class LyricInfoManager {
public:
	static LyricInfoManager* Instance();
	void Write(std::vector<LyricInfo> lyricInfos, std::string path);
	std::vector<LyricInfo> Read(std::string path);

private:
	LyricInfoManager() {};
	LyricInfoManager(const LyricInfoManager&) {};
	LyricInfoManager& operator=(const LyricInfoManager&) {};
	static LyricInfoManager* instance;
};

#endif//LYRICINFOMANAGER_HPP
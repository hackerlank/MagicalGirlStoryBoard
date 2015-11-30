#ifndef LYRIC_INFO
#define LYRIC_INFO

#include <string>

struct LyricInfo {
	int id;
	int timing;
	std::wstring kanji;
	std::wstring english;
	int width;
	int height;
};

#endif//LYRIC_INFO
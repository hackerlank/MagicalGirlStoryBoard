#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <vector>
#include <string>

class Sprite {
public:
	Sprite();
	void Move();
	void Scale();
	void Rotate();
	void Color();
	std::string Write();
	std::vector<std::string> commands;
};

#endif//SPRITE_HPP
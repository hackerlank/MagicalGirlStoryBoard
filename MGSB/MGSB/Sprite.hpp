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
	void Write();
private:
	std::vector<std::string> commands;
};

#endif//SPRITE_HPP
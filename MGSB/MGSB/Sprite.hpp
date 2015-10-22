#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <vector>
#include <string>
#include "Layer.hpp"
#include "Origin.hpp"
#include "Vector2.hpp"

class Sprite {
public:
	Sprite(Layer layer, Origin origin, std::string filePath, Vector2 position);
	void Move();
	void Scale();
	void Rotate();
	void Color();
	std::string Write();
	std::vector<std::string> commands;
private:
	Layer layer;
	Origin origin;
	std::string filePath;
	Vector2 position;
};

#endif//SPRITE_HPP
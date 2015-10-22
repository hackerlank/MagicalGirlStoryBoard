#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <vector>
#include <string>
#include "Layer.hpp"
#include "Origin.hpp"
#include "Vector2.hpp"
#include <fstream>

class Sprite {
public:
	Sprite(Layer layer, Origin origin, const std::string& filePath, Vector2 position);
	void Move();
	void Fade();
	void Scale();
	void Rotate();
	void Color();
	void Write(std::ofstream& outputFile);
	std::vector<std::string> commands;
private:
	Layer layer;
	Origin origin;
	std::string filePath;
	Vector2 position;
};

#endif//SPRITE_HPP
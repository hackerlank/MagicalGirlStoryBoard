#include "Sprite.hpp"
#include "Storyboard.hpp"

Sprite::Sprite(Layer layer, Origin origin, std::string filePath, Vector2 position) 
	: layer(layer), origin(origin), filePath(filePath), position(position) {}



std::string Sprite::Write() {
	std::string sprite = "Sprite," + Layers[layer] + "," + Origins[origin] + "," + filePath + "," + position.x + "," + position.y;
	for (auto command : commands) {
		sprite += command + "\n";
	}

	return sprite;
}
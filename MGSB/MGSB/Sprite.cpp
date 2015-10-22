#include "Sprite.hpp"
#include "Storyboard.hpp"

Sprite::Sprite(Layer layer, Origin origin, const std::string& filePath, Vector2 position) 
	: layer(layer), origin(origin), filePath(filePath), position(position) {}

void Sprite::Move() {

}

void Sprite::Fade() {

}

void Sprite::Rotate() {

}

void Sprite::Scale() {

}

void Sprite::Color() {

}

void Sprite::Write(std::ofstream& outputFile) {
	outputFile << "Sprite," << Layers[layer] << "," << Origins[origin] << "," << filePath << "," << position.x << "," << position.y << std::endl;;
	for (auto command : commands) {
		outputFile << command << std::endl;
	}
}
#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <vector>
#include <string>
#include "Layer.hpp"
#include "Origin.hpp"
#include "Vector2.hpp"
#include "Easing.hpp"
#include <fstream>

class Sprite {
public:
	Sprite(const std::string& filePath, Vector2 position, Vector2 size, Layer layer = Layer::Foreground, Origin origin = Origin::Centre);
	void Move(int startTime, int endTime, int startX, int startY, int endX, int endY, Easing easing = Easing::Linear);
	void Fade(int startTime, int endTime, double startOpacity, double endOpacity, Easing easing = Easing::Linear);
	void Rotate(int startTime, int endTime, double startRotate, double endRotate, Easing easing = Easing::Linear);
	void Scale(int startTime, int endTime, double startScale, double endScale, Easing easing = Easing::Linear);
	void Color(int startTime, int endTime, int startR, int startG, int startB, int endR, int endG, int endB, Easing easing = Easing::Linear);
	void Write(std::ofstream& outputFile);
	std::vector<std::string> commands;

	double fade;
	Vector2 position;
	double rotation;
	double scale;
	Color color;
	Vector2 size;
private:
	Layer layer;
	Origin origin;
	std::string filePath;
};

#endif//SPRITE_HPP

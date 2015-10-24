#include "Sprite.hpp"
#include "Storyboard.hpp"
#include <sstream>

Sprite::Sprite(Layer layer, Origin origin, const std::string& filePath, Vector2 position) 
	: layer(layer), origin(origin), filePath(filePath), position(position) {}

// _M,<easing>,<starttime>,<endtime>,<start_x>,<start_y>,<end_x>,<end_y>
void Sprite::Move(int startTime, int endTime, int startX, int startY, int endX, int endY, Easing easing) {
	std::ostringstream command;
	command << "_M," << Layers[easing] << "," << startTime << "," << endTime << "," << startX << "," << startY << "," << endX << "," << endY;
	commands.push_back(command.str());
}

// _F,<easing>,<starttime>,<endtime>,<start_opacity>,<end_opacity>
void Sprite::Fade(int startTime, int endTime, double startOpacity, double endOpacity, Easing easing) {
	std::ostringstream command;
	command << "_F," << Layers[easing] << "," << startTime << "," << endTime << "," << startOpacity << "," << endOpacity;
	commands.push_back(command.str());
}

// _R,<easing>,<starttime>,<endtime>,<start_rotate>,<end_rotate>
void Sprite::Rotate(int startTime, int endTime, double startRotate, double endRotate, Easing easing) {
	std::ostringstream command;
	command << "_R," << Layers[easing] << "," << startTime << "," << endTime << "," << startRotate << "," << endRotate;
	commands.push_back(command.str());
}

// _S,<easing>,<starttime>,<endtime>,<start_scale>,<end_scale>
void Sprite::Scale(int startTime, int endTime, double startScale, double endScale, Easing easing) {
	std::ostringstream command;
	command << "_S," << Layers[easing] << "," << startTime << "," << endTime << "," << startScale << "," << endScale;
	commands.push_back(command.str());
}

// _C,<easing>,<starttime>,<endtime>,<start_r>,<start_g>,<start_b>,<end_r>,<end_g>,<end_b>
void Sprite::Color(int startTime, int endTime, int startR, int startG, int startB, int endR, int endG, int endB, Easing easing) {
	std::ostringstream command;
	command << "_C," << Layers[easing] << "," << startTime << "," << endTime << "," << startR << "," << startG << "," << startB << "," << endR << "," << endG << "," << endB;
	commands.push_back(command.str());
}

void Sprite::Write(std::ofstream& outputFile) {
	// Sprite,<layer>,<origin>,"<filepath>",<x>,<y>
	outputFile << "Sprite," << Layers[layer] << "," << Origins[origin] << "," << filePath << "," << position.x << "," << position.y << std::endl;;
	for (auto command : commands) {
		outputFile << command << std::endl;
	}
}
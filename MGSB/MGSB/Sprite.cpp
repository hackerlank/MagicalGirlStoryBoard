#include "Sprite.hpp"
#include "Storyboard.hpp"

std::string Sprite::Write() {
	std::string sprite = "Sprite";
	for (auto command : commands) {
		sprite += command + "\n";
	}

	return sprite;
}
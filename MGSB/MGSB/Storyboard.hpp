#ifndef STORYBOARD_HPP
#define STORYBOARD_HPP

#include "Sprite.hpp"
#include <vector>
#include <string>

class Storyboard {
public:
	void Write(std::string destinationPath);
private:
	std::vector<Sprite> sprites;
};

#endif//STORYBOARDP_HPP
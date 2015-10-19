#ifndef STORYBOARD_HPP
#define STORYBOARD_HPP

#include "Sprite.hpp"
#include <vector>

class Storyboard {
public:
	void Write();
private:
	std::vector<Sprite> sprites;
};

#endif//STORYBOARDP_HPP
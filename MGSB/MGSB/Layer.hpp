#ifndef LAYER_HPP
#define LAYER_HPP

#include <string>

enum Layer {
	Background,
	Fail,
	Pass,
	Foreground,
	Count
};

std::string Layers[Layer::Count] = { 
	"Background",
	"Fail",
	"Pass",
	"Foreground" 
};

#endif//LAYER_HPP
#ifndef ORIGIN_HPP
#define ORIGIN_HPP

enum Origin {
	TopLeft,
	TopCentre,
	TopRight,
	CentreLeft,
	Centre,
	CentreRight,
	BottomLeft,
	BottomCentre,
	BottomRight,
	Count
};


std::string Origins[Origin::Count] = { 
	"TopLeft",
	"TopCentre",
	"TopRight",
	"CentreLeft",
	"Centre",
	"CentreRight",
	"BottomLeft",
	"BottomCentre",
	"BottomRight" 
};

#endif//ORiGIN_HPP
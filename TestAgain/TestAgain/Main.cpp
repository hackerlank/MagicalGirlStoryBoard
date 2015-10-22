#include <iostream>
#include <string>
#include <sstream>

std::string asdf = "a s d f";
int index = 0;

char getNextChar() {

	return asdf[index++];
}

int main() {
	std::string test = "halt12345";
	std::cout << test.substr(0, 4) << std::endl;


	while (1) {

	}
	return 0;
}
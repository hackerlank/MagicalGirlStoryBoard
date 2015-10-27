#include <pngwriter.h>

int main() {
	int i;
	int y;

	pngwriter png(300, 300, 0, "test.png");
	for (int i = 0; i <= 300; i++) {
		y = 150 + 100 * sin((double)i * 9 / 300.0);
		png.plot(i, y, 0.0, 0.0, 1.0);
	}

	png.close();

	return 0;
}
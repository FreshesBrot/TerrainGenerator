#include <iostream>
#include "PNGImage.h"
#include "Generator.h"

int main() {
	try {
		std::vector<int> bb({ 4,5,5 });
		std::vector<std::vector<int>> b;
		b.push_back(bb);
		int g = b[0][1];
		int size = 512;
		std::string out("out\\");
		PNGImage img(size, size);
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				float clr = (float(x) + float(y)) / (size * 2);
				img.setGrayPixel(x, y, clr);
			}
		}
		img.saveImage(out+"gray.png");
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << "\n";
		return 1;
	}
}


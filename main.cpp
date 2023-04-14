#include <iostream>
#include <string>

#define VERSION 1.0

#include "zlib.h"

#include "Image.h"

int main() {
	std::cout << "Data Pre-Processor v" << VERSION << "\n";

	std::cout << "Enter an image filename:\n";

	std::string filename;
	std::cin >> filename;

	Image img;
	if (Image::load(filename, img)) {
		if (img.process()) {
			img.print();
			Image::free(img);
		}
	}

	std::string wait;
	std::cin >> wait;

	return 0;
}
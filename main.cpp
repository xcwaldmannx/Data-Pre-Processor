#include <iostream>
#include <string>

#define VERSION 1.0

#include "Image.h"

void run() {
	std::string response;
	do {
		std::cout << "Type 'exit' to quit the program.\n";
		std::cout << "Type 'go' to process a texture or material.\n";
		std::cin >> response;

		if (response == "exit") {
			exit(0);
		}
		else if (response == "go") {
			std::cout << "Specify a filename or directory:\n";
			std::string filename;
			std::cin >> filename;

			Image img;
			if (Image::load(filename, img)) {
				if (img.process()) {
					img.print();
					Image::free(img);
				}
			}
		}

	} while (response != "exit" && response != "go");

	run();
}

int main() {
	std::cout << "Data Pre-Processor v" << VERSION << "\n";

	run();

	return 0;
}
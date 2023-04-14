#include "Image.h"

#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image() {
}

Image::~Image() {
}

bool Image::load(const std::string& filename, Image& img) {
	img.m_filename = filename;
	img.m_imageData = stbi_load(img.m_filename.c_str(), &img.m_width, &img.m_height, &img.m_channels, STBI_rgb_alpha);
	if (img.m_imageData) {
		return 1;
	}
	return 0;
}

void Image::free(Image& img) {
	stbi_image_free(img.m_imageData);
}

bool Image::process() {
	size_t fus = m_filename.find_first_of("_") + 1;
	size_t lus = m_filename.find_last_of("_");
	m_filename = m_filename.substr(fus, lus - fus);

	std::string outputFilename = m_filename + ".tex";
	std::ofstream output("Assets/" + outputFilename);

	std::cout << "Processing...\n";
	output << "name="     << m_filename << "\n";
	output << "width="    << m_width << "\n";
	output << "height="    << m_height << "\n";
	output << "channels=" << m_channels << "\n";

	size_t dataLength = std::strlen(reinterpret_cast<const char*>(m_imageData));
	size_t processChunk = 8000;
	size_t totalProcessed = 0;

	while (totalProcessed < dataLength) {

		if (totalProcessed + processChunk > dataLength) {
			processChunk = dataLength - totalProcessed;
			std::string chunk = std::string(reinterpret_cast<const char*>(m_imageData)).substr(totalProcessed, processChunk);
			output << chunk << "\n";
			break;
		}

		std::string chunk = std::string(reinterpret_cast<const char*>(m_imageData)).substr(totalProcessed, processChunk);
		output << chunk << "\n";
		totalProcessed += processChunk;
	}

	output.close();

	if (output.fail()) {
		std::cerr << "Error writing " << m_filename << "\n";
		return 0;
	}

	std::cout << "Done.\n";
	return 1;
}

int Image::getWidth() {
	return m_width;
}

int Image::getHeight() {
	return m_height;
}

int Image::getChannels() {
	return m_channels;
}

unsigned char* Image::getImageData() {
	return m_imageData;
}

void Image::print() {
	std::cout << "Name:     " << m_filename << "\n";
	std::cout << "Width:    " << m_width << "\n";
	std::cout << "Height:   " << m_height << "\n";
	std::cout << "Channels: " << m_channels << "\n";
}

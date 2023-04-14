#include "Image.h"

#include <fstream>
#include <iostream>
#include <direct.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "zlib.h"

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
	m_filename = m_filename.substr(m_filename.find_last_of("/") + 1);
	m_type = m_filename.substr(0, 3);
	size_t fus = m_filename.find_first_of("_") + 1;
	size_t lus = m_filename.find_last_of("_");
	m_alias = m_filename.substr(fus, lus - fus);

	std::string outDir = "Assets\\" + m_alias + "\\";

	// create out directory
	int result = _mkdir(outDir.c_str());

	if (result == 0) {
		std::cerr << "Failed to create directory: " << outDir << "\n";
		return 0;
	}

	std::string outputFilename = m_alias + ".tex";
	std::ofstream output(outDir + outputFilename);

	std::cout << "Processing...\n";
	output << "filename=" << m_filename << "\n";
	output << "alias="    << m_alias    << "\n";
	output << "type="     << m_type     << "\n";
	output << "width="    << m_width    << "\n";
	output << "height="   << m_height   << "\n";
	output << "channels=" << m_channels << "\n";

	// compress data

	size_t imageDataLength = std::strlen(reinterpret_cast<const char*>(m_imageData));

	uLong compressed_size = compressBound(imageDataLength); // get the maximum size of the compressed data
	char* compressed_data = new char[compressed_size];

	result = compress(reinterpret_cast<Bytef*>(compressed_data), &compressed_size, reinterpret_cast<const Bytef*>(m_imageData), imageDataLength);
	if (result == Z_OK) {
		std::cout << "Compression successful!" << std::endl;
		std::cout << "Original size: " << imageDataLength << " bytes" << std::endl;
		std::cout << "Compressed size: " << compressed_size << " bytes" << std::endl;
	}
	else {
		std::cerr << "Compression failed with error code " << result << std::endl;
	}

	// write compressed data

	size_t processDataAmount = 4000;
	size_t totalProcessed = 0;

	while (totalProcessed < compressed_size) {

		if (totalProcessed + processDataAmount > compressed_size) {
			processDataAmount = compressed_size - totalProcessed;
			std::string chunk = std::string(reinterpret_cast<const char*>(m_imageData)).substr(totalProcessed, processDataAmount);
			output << chunk << "\n";
			break;
		}

		std::string chunk = std::string(reinterpret_cast<const char*>(m_imageData)).substr(totalProcessed, processDataAmount);
		output << chunk << "\n";
		totalProcessed += processDataAmount;
	}

	output.close();

	if (output.fail()) {
		std::cerr << "Error writing " << m_filename << ".\n";
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
	std::cout << "Filename: " << m_filename << "\n";
	std::cout << "alias:    " << m_alias    << "\n";
	std::cout << "type:     " << m_type     << "\n";
	std::cout << "Width:    " << m_width    << "\n";
	std::cout << "Height:   " << m_height   << "\n";
	std::cout << "Channels: " << m_channels << "\n";
}

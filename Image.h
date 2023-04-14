#pragma once

#include <string>

class Image {
public:
	Image();
	~Image();

	static bool load(const std::string& filename, Image& img);
	static void free(Image& img);

	bool process();

	int getWidth();
	int getHeight();
	int getChannels();
	unsigned char* getImageData();

	void print();

private:
	std::string m_filename = "";
	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;
	unsigned char* m_imageData = nullptr;
};

#pragma once
#include "..\..\\libs\png++-0.2.9\png.hpp"
#define RGBBYTE_SIZE 255

//clarification
typedef char RGBByte;

//a color struct to manage RGB values fomr 0-255
struct RGB {
	RGBByte r, g, b;
};

//color struct to manage RGB values from [0,1]
struct RGBfloat32 {
	float r, g, b;
};

//this class represents a PNG image
//image pixels color range from 0-255 (a char or a RGBByte)
class PNGImage {
public:

	//opens exisiting image
	PNGImage(const char* fileName);
	//creates a new image
	PNGImage(int height, int width);
	~PNGImage();

	//sets the image pixel
	void setPixel(int x, int y, RGB color);
	void setPixel(int x, int y, RGBfloat32 color);

	//retrieves an image pixel
	RGB getPixel(int x, int y) const;
	RGBfloat32 getPixelfloat(int x, int y) const;

	//sets gray
	void setGrayPixel(int x, int y, float gray);
	void setGrayPixel(int x, int y, RGBByte gray);

	//retrieves a gray image pixel
	RGBByte getGrayPixel(int x, int y) const;
	float getGrayPixelfloat(int x, int y) const;

	//saves the image to either constructed file or specified file
	void saveImage();
	void saveImage(const char * fileName);
	void saveImage(std::string fileName);

	//get image dimensions
	int getHeight();
	int getWidth();

private:

	//file this class is linked to
	const char* fileName;
	int imgHeight, imgWidth;

	//internally managed image interface
	png::image< png::rgb_pixel > image;

};


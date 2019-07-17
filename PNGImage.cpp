#include "PNGImage.h"

PNGImage::PNGImage(const char* fileName) : image(fileName) {
	this->fileName = fileName;
	imgHeight = image.get_height();
	imgWidth = image.get_width();
}

PNGImage::PNGImage(int height, int width) : image(width,height) {
	fileName = nullptr;
	imgHeight = height;
	imgWidth = width;
}

PNGImage::~PNGImage() {

}

void PNGImage::setPixel(int x, int y, RGB color) { 
	png::rgb_pixel px(color.r, color.g, color.b);
	image.set_pixel(x, y, px);
}

void PNGImage::setPixel(int x, int y, RGBfloat32 color) {
	RGBByte r, g, b;
	r = RGBByte(color.r * RGBBYTE_SIZE);
	g = RGBByte(color.g * RGBBYTE_SIZE);
	b = RGBByte(color.b * RGBBYTE_SIZE);
	png::rgb_pixel px(r, g, b);
	image.set_pixel(x, y, px);
}

RGB PNGImage::getPixel(int x, int y) const {
	RGBByte r, g, b;
	png::rgb_pixel* px = &image.get_pixel(x, y);
	r = px->red;
	g = px->green;
	b = px->blue;
	return RGB{r,g,b};
}

RGBfloat32 PNGImage::getPixelfloat(int x, int y) const {
	png::rgb_pixel* px = &image.get_pixel(x,y);
	float r, g, b;
	r = px->red;
	g = px->green;
	b = px->blue;
	return RGBfloat32{r/ RGBBYTE_SIZE,g/ RGBBYTE_SIZE,b/ RGBBYTE_SIZE };
}

void PNGImage::setGrayPixel(int x, int y, float gray) {
	RGBByte c = gray * RGBBYTE_SIZE;
	png::rgb_pixel px(c, c, c);
	image.set_pixel(x, y, px);
}

void PNGImage::setGrayPixel(int x, int y, RGBByte gray) {
	png::rgb_pixel px(gray, gray, gray);
	image.set_pixel(x, y, px);
}

RGBByte PNGImage::getGrayPixel(int x, int y) const {
	return image.get_pixel(x, y).red;
}

float PNGImage::getGrayPixelfloat(int x, int y) const {
	png::rgb_pixel px = image.get_pixel(x, y);
	return px.red / float(RGBBYTE_SIZE);
}

void PNGImage::saveImage() {
	image.write(fileName);
}

void PNGImage::saveImage(const char * fileName) {
	image.write(fileName);
}

void PNGImage::saveImage(std::string fileName) {
	const char* file = fileName.c_str();
	image.write(file);
}

int PNGImage::getHeight() {
	return imgHeight;
}

int PNGImage::getWidth() {
	return imgWidth;
}

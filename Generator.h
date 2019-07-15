#pragma once
#include <vector>
#include <random>
#include "GeneratorConfigFile.h"

#define ARRAYINDEXMAX (resolution-1)

//a normal-struct
struct Normal {
	float x, y, z;
};

//this class will generate neccessary files for the terrain
class Generator {
public:

	//constructors
	Generator();
	Generator(GeneratorConfigFile* cfg);
	~Generator();

	//generate the heightfield and retrieve it
	void generateHeightField();
	//smooths the heightfield and returns it
	void smoothHeightField(int iterations);
	//retrieves the stored height map
	float** getHeightField();

	//generates the normal map and retrieves it
	void generateNormalMap();
	//retrieves normal map
	Normal** getNormalMap();

private:

	//current sigma
	float sigma;
	//the random engine and distribution
	std::default_random_engine rnd;
	std::normal_distribution<float> dist;
	
	//get a random float value
	float randomFloat();

	//the terrain map
	float** map;
	//the normal map
	Normal** nMap;

	//size of the map
	int resolution;
	//the index-resolution of the map after calculation
	float cropResolution;
	//the fall-off constant
	float H;

	//initializes the maps
	void init();

	//performs a square step
	void squareStep(int x, int y, float squareSize);
	//performs a diamond step
	void diamondStep();

	//smooths the heightfield
	void smooth();
	void smooth(int iterations);

	//checks if the index is out of bound
	bool isInBounds(int x, int y);

};


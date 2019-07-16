#pragma once
#include <vector>
#include <random>
#include "GeneratorConfigFile.h"
#include <chrono>

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
	
	//get a random float value
	float randomFloat();
	//get a uniform_dist float number
	float uniformFloat();

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
	std::vector<int> squareStep(int x, int y, float squareSize);
	//performs a diamond step
	void diamondStep(int x, int y, float squareSize);
	//calculates value for a single diamond specified by midpoint
	float singleDiamond(int x, int y, int step);

	//smooths the heightfield
	void smooth();
	void smooth(int iterations);

	//checks if the index is out of bound
	bool isInBounds(int x, int y);

	//returns the value at map point (x,y)
	float get(int x, int y);
	//sets the value at map point (x,y)
	void set(int x, int y, float val);
	//clamps the value between 0 and 1
	float clamp(float val);

};


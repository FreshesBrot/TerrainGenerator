#pragma once
#include <vector>
#include "GeneratorConfigFile.h"

//this class will generate neccessary files for the terrain
class Generator {
public:

	Generator();
	Generator(GeneratorConfigFile* cfg);
	~Generator();

private:

	//the terrain map
	std::vector<std::vector<int>> map;
	//size of the map
	int resolution;

	//the index-resolution of the map after calculation
	float cropResolution;

	//initializes the map
	void init();

};


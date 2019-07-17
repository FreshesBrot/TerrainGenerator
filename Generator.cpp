#include "Generator.h"

Generator::Generator() : rnd(0) {
	resolution = 512;
	cropResolution = 0.5f;
	sigma = 0.5f;
	H = 1;
	init();
}

Generator::Generator(GeneratorConfigFile* cfg) : rnd(80) {
	resolution = cfg->getResolution();
	cropResolution = cfg->getCropResolution();
	H = cfg->getFallOff();
	sigma = cfg->getSigma();

	cfg->Close();
	init();
}

Generator::~Generator() {
	delete map;
	delete nMap;
}

void Generator::smoothHeightField(int iterations) {
	smooth(iterations);
}

float* Generator::getHeightField() {
	return map;
}

void Generator::generateNormalMap() {
}

Normal* Generator::getNormalMap() {
	return nMap;
}

float Generator::randomFloat() {
	std::normal_distribution<float> dist(0.0f, sigma);
	return dist(rnd);
}

float Generator::uniformFloat() {
	std::uniform_real_distribution<float> dist(0,1);
	return dist(rnd);
}

void Generator::init() {
	map = new float[ARRAYMAX * ARRAYMAX];
	for (int i = 0; i < ARRAYMAX * ARRAYMAX; i++)
		map[i] = 0.0f;

	//initiate corner values
	set(0, 0,uniformFloat());
	set(resolution, 0, uniformFloat());
	set(0, resolution, uniformFloat());
	set(resolution, resolution, uniformFloat());

	nMap = new Normal[ARRAYMAX * ARRAYMAX];
	for (int i = 0; i < ARRAYMAX * ARRAYMAX; i++)
		nMap[i] = Normal{ 0,0,0 };

}

#pragma region GENERATOR
void Generator::generateHeightField() {

	float itSize = 1.0f;
	float res = float(resolution);
	while (itSize * res > 1.0f) {
		

		//first calculate all square midpoints per iteration
		//struct that holds point information
		struct Point {
			int x, y;
		};
		//vector that stores all calculated points
		std::vector<Point> midPoints;

		//calculate all mid points
		for (int yStep = 0; yStep < resolution; yStep += itSize * res) {
			for (int xStep = 0; xStep < resolution; xStep += itSize * res) {
				std::vector<int> mid = squareStep(xStep, yStep,itSize);
				midPoints.push_back(Point{ mid[0],mid[1] });
			}
		}
		//pass midpoints to diamond step
		//make diamond step for each diamond
		std::vector<Point>::iterator it;
		for (it = midPoints.begin(); it < midPoints.end(); it++) {
			diamondStep(it->x, it->y, itSize);
		}

		sigma = (sigma) / (pow(2,H));
		
		itSize /= 2;
	}

}

std::vector<int> Generator::squareStep(int x, int y, float squareSize) {
	int step = squareSize * resolution;
	int xMid = x + step/2, yMid = y + step/2;
	//calculate average of square cornerpoints
	float avg = (
		get(x,y)+
		get(x+step,y)+
		get(x,y+step)+
		get(x+step,y+step)
		);
	avg /= 4.0f;

	//add random displacement
	avg += randomFloat();
	clamp(avg);
	//set value to calculated midpoint
	set(xMid, yMid, avg);
	return std::vector<int>({ xMid,yMid });
}

void Generator::diamondStep(int x, int y, float squareSize) {
	int step = (squareSize / 2.0) * resolution;
	//values per diamond
	float top, left, bot, right;
	int xStep, yStep;
	
	//get respective average of diamond midpoint
	top = singleDiamond(xStep = x, yStep = y - step, step);
	if(isInBounds(xStep,yStep))
		set(xStep, yStep, top);
	
	left = singleDiamond(xStep = x - step, yStep = y, step);
	if (isInBounds(xStep, yStep))
		set(xStep, yStep, left);
	
	bot = singleDiamond(xStep = x, yStep = y + step, step);
	if (isInBounds(xStep, yStep))
		set(xStep, yStep, bot);
	
	right = singleDiamond(xStep = x + step, yStep = y, step);
	if (isInBounds(xStep, yStep))
		set(xStep, yStep, right);

}

float Generator::singleDiamond(int x, int y, int step) {
	//avg of diamond and "adjacent" points
	float avg = 0;
	float adj = 0;
	
	//check for each sampled point if it is in array bounds
	int xStep, yStep;
	//left
	if (isInBounds(xStep = x - step, yStep = y)) {
		avg += get(xStep, yStep);
		adj++;
	}
	//right
	if (isInBounds(xStep = x + step, yStep)) {
		avg += get(xStep, yStep);
		adj++;
	}
	//top
	if (isInBounds(xStep = x, yStep = y - step)) {
		avg += get(xStep, yStep);
		adj++;
	}
	//bottom
	if (isInBounds(xStep, yStep = y + step)) {
		avg += get(xStep, yStep);
		adj++;
	}
	
	//divide by amount of sampled points
	avg /= adj;
	//add random displacement
	avg += randomFloat();
	clamp(avg);
	return avg;
}

#pragma endregion

void Generator::smooth() {
	for (int x = 0; x < resolution; x++) {
		for (int y = 0; y < resolution; y++) {
			int i = -1;
			int j = -1;

			float avg = 0;
			float adjacents = 0;

			//go through 3x3 area around specified point
			while (j < 2) {
				while (i < 2) {
					if (isInBounds(x + i, y + j)) {
						avg += get(x+i,y+j);
						adjacents++;
					}
					i++;
				}
				i = -1;
				j++;
			}
			//average the values by amount of sampled points
			avg /= adjacents;
			avg = clamp(avg);
			set(x, y, avg);
		}
	}
}

void Generator::smooth(int iterations) {
	while (iterations-- > 0) {
		smooth();
	}
}

bool Generator::isInBounds(int x, int y) {
	if (x > resolution || x < 0 || y > resolution || y < 0)
		return false;
	return true;
}

float Generator::get(int x, int y) {
	return map[IDX(x, y)];
}

void Generator::set(int x, int y, float val) {
	map[IDX(x,y)] = val;
}

float Generator::clamp(float val) {
	if (val > 1) {
		val = 1;
		return val;
	}
	if (val < 0) val = 0;
	return val;
}

#include "Generator.h"

Generator::Generator() : sigma(0.5f),rnd(0) {
	resolution = 512;
	cropResolution = 0.5f;
	H = 1;
	init();
}

Generator::Generator(GeneratorConfigFile* cfg) : sigma(0.5f),rnd(80) {
	resolution = cfg->getResolution();
	cropResolution = cfg->getCropResolution();
	H = cfg->getFallOff();

	cfg->Close();
	init();
}

Generator::~Generator() {
	for (int i = 0; i < resolution+1; i++)
		delete[] map[i];
	delete[] map;
	for (int i = 0; i < resolution+1; i++)
		delete[] nMap[i];
	delete[] nMap;
}

void Generator::smoothHeightField(int iterations) {
	smooth(iterations);
}

float** Generator::getHeightField() {
	return map;
}

void Generator::generateNormalMap() {
}

Normal** Generator::getNormalMap() {
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
	map = new float* [resolution + 1];
	for (int i = 0; i < resolution + 1; i++) {
		map[i] = new float[resolution + 1];
		for (int j = 0; j < resolution + 1; j++) {
			map[i][j] = 0.0f;
		}
	}
	//place random values at corner points
	map[0][0] = uniformFloat();
	map[resolution-1][0] = uniformFloat();
	map[0][resolution-1] = uniformFloat();
	map[resolution-1][resolution-1] = uniformFloat();

	nMap = new Normal * [resolution + 1];
	for (int i = 0; i < resolution + 1; i++) {
		nMap[i] = new Normal[resolution + 1];
		for (int j = 0; j < resolution + 1; j++) {
			nMap[i][j] = Normal{ 0.0f,0.0f,0.0f };
		}
	}
}

void Generator::generateHeightField() {
	//for (int x = 0; x < resolution; x++) {
	//	for (int y = 0; y < resolution; y++)
	//		map[x][y] = randomFloat();
	//}

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

		//calculate all mid points first
		for (int yStep = 0; yStep < resolution; yStep += itSize * res) {
			for (int xStep = 0; xStep < resolution; xStep += itSize * res) {
				std::vector<int> mid = squareStep(xStep, yStep,itSize);
				midPoints.push_back(Point{ mid[0],mid[1] });
			}
		}
		//pass midpoints to diamond step
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
	float avg = (
		get(x,y)+
		get(x+step,y)+
		get(x,y+step)+
		get(x+step,y+step)
		);
	//set(x, y,1);
	//set(x + step, y,1);
	//set(x, y + step,1);
	//set(x + step, y + step,1);

	avg /= 4.0f;
	avg += randomFloat();
	clamp(avg);
	set(xMid, yMid, avg);
	return std::vector<int>({ xMid,yMid });
}

void Generator::diamondStep(int x, int y, float squareSize) {
	int step = (squareSize / 2.0) * resolution;
	//get values per diamond
	float top, left, bot, right;
	int xStep, yStep;
	
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

	avg /= adj;
	avg += randomFloat();
	clamp(avg);
	return avg;
}

void Generator::smooth() {
	for (int x = 0; x < resolution; x++) {
		for (int y = 0; y < resolution; y++) {
			int i = -1;
			int j = -1;

			float avg = 0;
			float adjacents = 0;

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

			set(x, y, avg / adjacents);
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
	return map[y][x];
}

void Generator::set(int x, int y, float val) {
	map[y][x] = val;
}

float Generator::clamp(float val) {
	if (val > 1) val = 1;
	if (val < 0) val = 0;
	return val;
}

#include "Generator.h"

Generator::Generator() : sigma(0.25f),rnd(0), dist(0.0f,sigma) {
	resolution = 512;
	cropResolution = 0.5f;
	H = 1;
	init();
}

Generator::Generator(GeneratorConfigFile* cfg) : sigma(0.25f),rnd(0), dist(0.0f, sigma) {
	resolution = cfg->getResolution();
	cropResolution = cfg->getCropResolution();
	H = cfg->getFallOff();

	cfg->Close();
	init();
}

Generator::~Generator() {
	for (int i = 0; i < resolution; i++)
		delete[] map[i];
	delete[] map;
	for (int i = 0; i < resolution; i++)
		delete[] nMap[i];
	delete[] nMap;
}

void Generator::generateHeightField() {
	//for (int x = 0; x < resolution; x++) {
	//	for (int y = 0; y < resolution; y++)
	//		map[x][y] = randomFloat();
	//}

	float itSize = 1.0f;
	float res = float(resolution);
	while (itSize * res > 1.0f) {
		//first calculate the square midpoint
		int xStep, yStep;
		for (xStep = 0; xStep < resolution; xStep += itSize * resolution) {
			for (yStep = 0; yStep < resolution; yStep += itSize * resolution)
				squareStep(xStep, yStep, itSize);
		}
		sigma = (sigma) / (pow(2,H));
		itSize /= 2;
	}

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
	return dist(rnd);
}

void Generator::init() {
	map = new float*[resolution];
	for (int i = 0; i < resolution; i++) {
		map[i] = new float[resolution];
		for (int j = 0; j < resolution; j++) {
			map[i][j] = 0.0f;
		}
	}
	//place random values at corner points
	float f;
	map[0][0] = (f = randomFloat() + 0.5f);
	map[ARRAYINDEXMAX][0] = (f = randomFloat() + 0.5f);
	map[0][ARRAYINDEXMAX] = (f = randomFloat() + 0.5f);
	map[ARRAYINDEXMAX][ARRAYINDEXMAX] = (f = randomFloat() + 0.5f);

	nMap = new Normal*[resolution];
	for (int i = 0; i < resolution; i++) {
		nMap[i] = new Normal[resolution];
		for (int j = 0; j < resolution; j++) {
			nMap[i][j] = Normal{0.0f,0.0f,0.0f};
		}
	}
}

void Generator::squareStep(int x, int y, float squareSize) {

	//calculate one step
	int xStep = x + squareSize * ARRAYINDEXMAX;
	int yStep = y + squareSize * ARRAYINDEXMAX;
	//calculate midpoint of defined square
	int xMid = xStep / 2;
	int yMid = yStep / 2;
	//calculate average of 4 points
	float avg = (
		map[y][x] +
		map[y][xStep]+
		map[yStep][x]+
		map[yStep][xStep]
		);
	avg /= 4;
	avg += randomFloat();
	//insert value for midpoint
	map[yMid][xMid] = avg;
}

void Generator::smooth() {
	for (int x = 0; x < resolution; x++) {
		for (int y = 0; y < resolution; y++) {
			int i = -1;
			int j = -1;

			float avg = 0;
			int adjacents = 0;

			while (j < 2) {
				while (i < 2) {
					if (isInBounds(x + i, y + j)) {
						avg += map[x + i][y + j];
						adjacents++;
					}
					i++;
				}
				i = -1;
				j++;
			}

			map[x][y] = avg / float(adjacents);
		}
	}
}

void Generator::smooth(int iterations) {
	while (iterations-- > 0) {
		smooth();
	}
}

bool Generator::isInBounds(int x, int y) {
	if (x == resolution || x == -1 || y == resolution || y == -1)
		return false;
	return true;
}

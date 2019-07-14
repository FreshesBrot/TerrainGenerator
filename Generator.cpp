#include "Generator.h"

Generator::Generator() {
	resolution = 512;
	cropResolution = 0.5f;
	init();
}

Generator::Generator(GeneratorConfigFile* cfg) {
	resolution = cfg->getResolution();
	cropResolution = cfg->getCropResolution();
	cfg->Close();
	init();
}

Generator::~Generator() {

}

void Generator::init() {
	map.reserve(resolution);
	for (int i = 0; i < resolution; i++) {
		std::vector<int> vertical;
		vertical.reserve(resolution);
		map.push_back(vertical);
	}
}

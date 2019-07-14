#include "Generator.h"

Generator::Generator() {
	size = 16;
	cropResolution = 0.5f;
	init();
}

Generator::Generator(GeneratorConfigFile* cfg) {
	

	cfg->Close();
}

Generator::~Generator() {

}

void Generator::init() {
	map.reserve(size);
	for (int i = 0; i < size; i++) {
		std::vector<int> vertical;
		vertical.reserve(size);
		map.push_back(vertical);
	}
}

#include <iostream>
#include "PNGImage.h"
#include "Generator.h"

int main() {
	try {
		GeneratorConfigFile cfg(L"cfg.txt");
		Generator g(&cfg);
		return 0;
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << "\n";
		return 1;
	}
}


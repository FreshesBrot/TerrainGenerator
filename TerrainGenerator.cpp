#include <iostream>
#include "PNGImage.h"
#include "Generator.h"
#define IDX(x,y) ((x)+(y)*(res+1))

int main() {
	try {
		//initialize generator through file
		std::cout << "Reading Config File...\n";
		GeneratorConfigFile cfg(L"cfg.txt");
		Generator g(&cfg);
		int res = cfg.getResolution();
		int smoothIterations = cfg.getIterations();

		//generate the heightfield
		std::cout << "Generating HeightField of size " << res <<"...\n";
		g.generateHeightField();

		//smoothing the heighfield
		std::cout << "Smoothing HeightField "<< smoothIterations <<" times...\n";
		g.smoothHeightField(smoothIterations);
		std::cout << "Finished Generation.\n";
		float* map = g.getHeightField();

		//transform it into an image
		std::cout << "Saving Image...\n";
		PNGImage heightImage(res, res);
		for (int y = 0; y < res; y++) {
			for (int x = 0; x < res; x++)
				heightImage.setGrayPixel(x, y, map[IDX(x,y)]
				);
		}
		
		heightImage.saveImage(cfg.getMapFileName());

		std::cout << "Done!\n";
		system("pause");
		return 0;
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << "\n";
		return 1;
	}
}


#pragma once
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
#include "PNGImage.h"

//this class will create a meshfile in custom format
class MeshMaker {
public:

	//takes the file name to output the file to and a heightfield image
	MeshMaker(const char* fileName);
	MeshMaker(std::string fileName);
	MeshMaker();

	//creates geometry based on the specified height field image
	void MakeGeometry(PNGImage image);

	//writes / creates to the specified file
	void CreateMeshFile();

private:

	//the output file name
	const char* fileName;

	//vertex struct that holds relevant information
	struct Vertex {
		float x, y, z;
		float tx_u, tx_v;
	};

	//vectors that contain relevant geometry information
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	//the stream that writes data to the file
	std::ofstream data;


};


#include "MeshMaker.h"

MeshMaker::MeshMaker(const char* fileName) : vertices(),indices() {
	this->fileName = fileName;
}

MeshMaker::MeshMaker(std::string fileName) : vertices(),indices() {
	this->fileName = fileName.c_str();
}

MeshMaker::MeshMaker() {
	fileName = "dd.sof";
}

void MeshMaker::MakeGeometry(PNGImage image) {
	//position of one vertex
	//position is defined in a 10x1x10 cube
	float x, y, z;
	int resolution = image.getWidth();
	//increments
	float increment = 1 / float(resolution);

	//reserve space
	vertices.reserve((resolution-1) * (resolution-1));

	//go through all pixels; increment by resolution
	for (int i = 0; i < resolution;i++) {
		for (float j = 0; j < resolution; j++) {
			//get heightvalue and scale it according to resolution
			y = image.getGrayPixelfloat(j,i) * 10 / resolution - 1;
			x = j / float(resolution) * 10;
			z = i / float(resolution) * 10;
			float tx_u, tx_v;
			tx_u = i * increment;
			tx_v = j * increment;

			//push back calculated vertex
			//convert to D3D coordinate space by inverting x
			Vertex vrt = { x,y,-(z)+10,tx_u,tx_v };
			vertices.push_back(vrt);

		}
	}

	//reserve space
	indices.reserve(resolution * resolution);

	//generate indices
	for (int x = 0; x < resolution * (resolution - 1) - 1; x++) {
		int topLeft, topRight, botLeft, botRight;
		
		//indices specifiy a quadrilateral
		topLeft = x; topRight = x + 1;
		botLeft = x + resolution;
		botRight = x + 1 + resolution;
		
		//push back in order for tesselation
		indices.push_back(topLeft);
		indices.push_back(topRight);
		indices.push_back(botLeft);
		indices.push_back(botRight);
		

	}

}

void MeshMaker::CreateMeshFile() {
	data.open(fileName);

	//declare beginning of vertices
	data << "vrt\n";

	//convert every vertex to string representation
	std::vector<Vertex>::iterator vertexIterator;
	data << "#0\n";
	for (vertexIterator = vertices.begin(); vertexIterator < vertices.end(); vertexIterator++) {
		data << 
			//position
			vertexIterator->x << " " <<
			vertexIterator->y << " " <<
			vertexIterator->z << " " <<
			//color
			"0.0" << " " <<
			"0.0" << " " <<
			"0.0" << " " <<
			//normals
			"0.0 0.0 -1.0" << " " <<
			//"0.0 1.0 0.0" << " " <<
			//"0.0 1.0 0.0" << " " <<
			//tx coords
			vertexIterator->tx_u << " " <<
			vertexIterator->tx_v << " " << "\n";
	}
	//write end of block statement
	data << "# " << vertices.size()-1 << " ("<< vertices.size() << ")\n" << "/\n";
	
	//write indices
	data << "f\n";
	for (int i = 0; i < indices.size(); i += 4) {
		data << indices[i] << " " << indices[i + 1] << " " << indices[i + 3] << " " << indices[i + 2] << "\n";
	}

	//write end of block statement
	data << "#" << indices.size()-1 << " (" << indices.size()<<")\n/\n";
	//specifiy vertex and pixel shader for mesh
	data << "vs\nVertexShader.cso\n/\n";
	data << "ps\nPixelShader.cso\n/\n";

}

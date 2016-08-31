#include "ObjLoader.h"
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<memory>
#include"Math\AAMath.h"

using namespace AAEngine;

namespace {
	struct VertexIndex {
		unsigned int posIndx;
		unsigned int texIndex;
		unsigned int normalIndex;
		VertexIndex(unsigned int v, unsigned int vt, unsigned int vn) {
			posIndx = v;
			texIndex = vt;
			normalIndex = vn;
		}
	};
}

ObjLoader* ObjLoader::instance_ = nullptr;

ObjLoader::ObjLoader()
{
	delete instance_;
}
ObjLoader::~ObjLoader()
{
}
ObjLoader* ObjLoader::Instance() {
	if (instance_ == nullptr) {
		instance_ = new ObjLoader();
	}
	return instance_;
}
void ObjLoader::OpenGL_LoadOBJ(char *fileName)
{
	std::vector<Vector3> v_positions;
	std::vector<Vector2> v_uvTextures;
	std::vector<Vector3> v_normals;
	std::vector<VertexIndex> v_FaceVertexIndex;
	
	unsigned int vertexPerFace = 0;

	//1. Open file
	std::ifstream fileStream;
	fileStream.open(fileName);
	if (fileStream.fail()) {
		std::exit(404);
	}
	//2.Read/Write
	std::string line;
	while (!fileStream.eof())
	{
		
		std::getline(fileStream,line);
		//1. open my stringt
		std::istringstream stringStream;
		stringStream.str(line); //convert the the line of the fileStream as string
		
		//2.read/write
		std::string tag;
		stringStream >> tag;
		if (tag == "v") {
			//v  75.1132 6.2647 -31.6841
			float x, y, z;
			stringStream >> x >> y >> z;
			Vector3 pos = Vector3(x,y,z);
			v_positions.push_back(pos);
		}
		else if (tag == "vt") {
			//vt 1.0000 1.0000 
			float u, v;
			stringStream >> u >> v;
			Vector2 uv = Vector2(u,v);
			v_uvTextures.push_back(uv);
		}
		else if (tag == "vn") {
			//vn -0.5479 0.8360 -0.0308
			float x, y, z;
			stringStream >> x >>y>> z;
			Vector3 normal = Vector3(x,y,z);
			v_normals.push_back(normal);
		}
		else if (tag == "f") {
			// each face hase 4 vertices
			//index of   pos/vt/vn
			//f 25/25/7  26/26 /7  27/27/7  28/28/7
			std::string token;
			std::istringstream stringStream(line);
			stringStream >> token;//first token is the tag f;

			unsigned int indexV;
			unsigned int indexT;
			unsigned int indexN;
			unsigned char slash;

			while (stringStream >> token)
			{
				std::istringstream stringStream_2(token);
				stringStream_2 >> indexV >> slash >> indexT >> slash >> indexN;
				VertexIndex vertexIndex = VertexIndex(indexV, indexT, indexN);
				v_FaceVertexIndex.push_back(vertexIndex);
				
				if(vertexPerFace<4)
					vertexPerFace++;
			}
		}
	}
	fileStream.close();

	/*----------------------------------------------------
	v_FaceVertexIndex:
	------------------------------------------------------
	vertx1  vertx2  vertx3  vertx4
	|       |
	v/vt/vn |
	        v/vt/vn
	-----------------------------------------------------*/

	unsigned int numVertexIndex = v_FaceVertexIndex.size();

	float* positionsArray = new float[v_positions.size() * 3];
	unsigned int posIndex = 0;
	for (std::vector<Vector3>::iterator it = v_positions.begin(); it != v_positions.end(); it++) {
		positionsArray[posIndex++] = it->x;
		positionsArray[posIndex++] = it->y;
		positionsArray[posIndex++] = it->z;
	}
	float* texturesArray = new float[v_positions.size() * 2];
	float* normalsArray = new float[v_positions.size() * 3];

	int* indices = new int[numVertexIndex];

	for (int i = 0; i < numVertexIndex; i++) {
		unsigned int positionIndex = v_FaceVertexIndex[i].posIndx;
		positionIndex--;
		indices[i] = positionIndex;
		texturesArray[positionIndex * 2] = v_uvTextures[v_FaceVertexIndex[i].texIndex - 1].x;
		texturesArray[positionIndex * 2 + 1] = v_uvTextures[v_FaceVertexIndex[i].texIndex - 1].y;

		normalsArray[positionIndex * 3] = v_normals[v_FaceVertexIndex[i].normalIndex - 1].x;
		normalsArray[positionIndex * 3 + 1] = v_normals[v_FaceVertexIndex[i].normalIndex - 1].y;
		normalsArray[positionIndex * 3 + 2] = v_normals[v_FaceVertexIndex[i].normalIndex - 1].z;
	}
	/*
	arrayPos: x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4,..........
	arrayTex: u1,v2,u2,v2,u3,v3,u4,v4,.............
	arrayNor: x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4,..........
	*/
	//-------------------------------------------------------------
	// pass all data .....
	//-------------------------------------------------------------
	/*delete[] indices; 
	indices = nullptr;
	delete[] positionsArray;
	positionsArray = nullptr;
	delete[] texturesArray;
	texturesArray = nullptr;
	delete[] normalsArray;
	normalsArray = nullptr;*/
	v_FaceVertexIndex.clear();
	v_positions.clear();
	v_normals.clear();
}
void ObjLoader::DirectX_LoadOBJ(char *fileName)
{
	std::vector<Vector3> v_positions;
	std::vector<Vector2> v_uvTextures;
	std::vector<Vector3> v_normals;
	std::vector<VertexIndex> v_FaceVertexIndex;

	unsigned int vertexPerFace = 0;

	//1. Open file
	std::ifstream fileStream;
	fileStream.open(fileName);
	if (fileStream.fail()) {
		std::exit(404);
	}
	//2.Read/Write
	std::string line;
	while (!fileStream.eof())
	{

		std::getline(fileStream, line);
		//1. open my stringt
		std::istringstream stringStream;
		stringStream.str(line); //convert the the line of the fileStream as string

								//2.read/write
		std::string tag;
		stringStream >> tag;
		if (tag == "v") {
			//v  75.1132 6.2647 -31.6841
			float x, y, z;
			stringStream >> x >> y >> z;
			Vector3 pos = Vector3(x, y, z);
			v_positions.push_back(pos);
		}
		else if (tag == "vt") {
			//vt 1.0000 1.0000 
			float u, v;
			stringStream >> u >> v;
			Vector2 uv = Vector2(u, v);
			v_uvTextures.push_back(uv);
		}
		else if (tag == "vn") {
			//vn -0.5479 0.8360 -0.0308
			float x, y, z;
			stringStream >> x >> y >> z;
			Vector3 normal = Vector3(x, y, z);
			v_normals.push_back(normal);
		}
		else if (tag == "f") {
			// each face hase 4 vertices
			//index of   pos/vt/vn
			//f 25/25/7  26/26 /7  27/27/7  28/28/7
			std::string token;
			std::istringstream stringStream(line);
			stringStream >> token;//first token is the tag f;

			unsigned int indexV;
			unsigned int indexT;
			unsigned int indexN;
			unsigned char slash;

			while (stringStream >> token)
			{
				std::istringstream stringStream_2(token);
				stringStream_2 >> indexV >> slash >> indexT >> slash >> indexN;
				VertexIndex vertexIndex = VertexIndex(indexV, indexT, indexN);
				v_FaceVertexIndex.push_back(vertexIndex);

				if (vertexPerFace<4)
					vertexPerFace++;
			}
		}
	}
	fileStream.close();

	/*----------------------------------------------------
	v_FaceVertexIndex:
	------------------------------------------------------
	vertx1  vertx2  vertx3  vertx4
	|       |
	v/vt/vn |
	        v/vt/vn
	-----------------------------------------------------*/

	unsigned int numVertexIndex = v_FaceVertexIndex.size();
	MYDXVERTEX* vertices = new MYDXVERTEX[numVertexIndex];
	int* indices = new int[numVertexIndex];

	//copy the position data to Vertex-position;
	unsigned int posIndex = 0;
	for (std::vector<Vector3>::iterator it = v_positions.begin(); it != v_positions.end(); it++) {
		vertices[posIndex].px = it->x;
		vertices[posIndex].py = it->y;
		vertices[posIndex++].pz = it->z;
	}
	//set up the 
	unsigned int vertexindex = 0;
	for (int i = 0; i < numVertexIndex; i++) {
		unsigned int positionIndex = v_FaceVertexIndex[i].posIndx;
		vertices[positionIndex-1].tx = v_uvTextures[v_FaceVertexIndex[i].texIndex - 1].x;
		vertices[positionIndex-1].ty = v_uvTextures[v_FaceVertexIndex[i].texIndex - 1].y;

		vertices[positionIndex-1].nx = v_normals[v_FaceVertexIndex[i].normalIndex - 1].x;
		vertices[positionIndex-1].ny = v_normals[v_FaceVertexIndex[i].normalIndex - 1].y;
		vertices[positionIndex-1].nz = v_normals[v_FaceVertexIndex[i].normalIndex - 1].z;

		indices[i] = positionIndex-1;
	}
	//for (int i = 100; i < 500; i++) {
		//MYDXVERTEX temp = vertices[i];
	//}
	/*
		we Alignd all uv and normal data with position data. by sort the f index data.
	     DXVERTEX[0]: px1,py1,pz1,tx,ty,nx,ny,nz
		 DXVERTEX[1]: px2,py2,pz2,tx,ty,nx,ny,nz
		 DXVERTEX[2]: px3,py3,pz3,tx,ty,nx,ny,nz
	*/
	//-------------------------------------------------------------
	// pass all data .....
	//-------------------------------------------------------------
	/*delete[] indices;
	indices = nullptr;
	delete[] positionsArray;
	positionsArray = nullptr;
	delete[] texturesArray;
	texturesArray = nullptr;
	delete[] normalsArray;
	normalsArray = nullptr;*/

	v_FaceVertexIndex.clear();
	v_positions.clear();
	v_normals.clear();
}
void ObjLoader::BindToRenderPipleLine() {

}
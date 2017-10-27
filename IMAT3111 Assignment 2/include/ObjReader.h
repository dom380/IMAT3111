#pragma once
#ifndef OBJREADER_H
#define OBJREADER_H
//#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\hash.hpp>
#include <vector>
using std::vector;
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <Material.h>

class ObjReader {
public:
	ObjReader();
	~ObjReader();
	//void readObj(const char * filePath, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures, vector<unsigned short>&indices);
	void readObj(const char * filePath, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures, vector<unsigned short>&indices, Material& material);
private:
	vector<glm::vec3> faceIndices; //x = position index. y = texture index. z = normal index
	std::unordered_map<glm::vec3, unsigned short> indexVerticeMap;
	vector<glm::vec4> unpackedVertices;
	vector<glm::vec3> unpackedNormals;
	vector<glm::vec2> unpackedTextures;

	void readMtl(const char* filePath, Material& material);
	glm::vec3 readVec3(std::istringstream& stream);
};

#endif // !OBJREADER_H
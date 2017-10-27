#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include<glm\glm.hpp>

class Material 
{
public:
	Material() {};
	Material(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, glm::vec3 Ke, float Ni, float d, float Ns)
	{
		this->Ka = Ka;
		this->Kd = Kd;
		this->Ks = Ks;
		this->Ke = Ke;
		this->Ni = Ni;
		this->d = d;
		this->Ns = Ns;
		this->used = true;
	}
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 Ke;
	float Ni;
	float d;
	float Ns;
	bool used = false;
};
#endif // !MATERIAL_H

#include <stdafx.h>
#include <iostream>


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

using namespace std;

#define PII 3.14159265

glm::vec3 getNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	glm::vec3 edge1 = v2 - v1;
	glm::vec3 edge2 = v2 - v3;
	return glm::normalize(glm::cross(edge1, edge2));
}

bool isVisible(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 camPos) {
	glm::vec3 normal = getNormal(v1, v2, v3);
	double angle = acosf(glm::dot(normal, camPos) / (glm::length(normal)*glm::length(camPos))) *(180.0f / PII);
	return angle >= 0.0 && angle < 90.0;
}

void printMat4(glm::mat4 matrix) {
	printf("%f, %f, %f %f\n", matrix[0].x, matrix[1].x, matrix[2].x, matrix[3].x);
	printf("%f, %f, %f %f\n", matrix[0].y, matrix[1].y, matrix[2].y, matrix[3].y);
	printf("%f, %f, %f %f\n", matrix[0].z, matrix[1].z, matrix[2].z, matrix[3].z);
	printf("%f, %f, %f %f\n", matrix[0].w, matrix[1].w, matrix[2].w, matrix[3].w);
}

int main() {
	glm::vec3 v1 = glm::vec3(0.0, 0.0, 0.5);
	glm::vec3 v2 = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 v3 = glm::vec3(0.0, 0.0, 1.0);

	printf("A triangle has the following points:\n");
	printf("Point 1: %f %f %f\n", v1.x, v1.y, v1.z);
	printf("Point 2: %f %f %f\n", v2.x, v2.y, v2.z);
	printf("Point 3: %f %f %f\n", v3.x, v3.y, v3.z);

	glm::vec3 edge1 = v2 - v1;
	glm::vec3 edge2 = v2 - v3;

	glm::vec3 normal = glm::cross(edge1, edge2);
	normal = glm::normalize(normal);

	printf("2. The normal of the triangle is %fi, %fj, %fk\n", normal.x, normal.y, normal.z);

	printf("3. Given a lightsource at (0,2,2) the light direction for each vector will be:\n");
	glm::vec3 lightPos = glm::vec3(0.0, 2.0, 2.0);
	glm::vec3 lightDir1 = glm::normalize(lightPos - v1);
	glm::vec3 lightDir2 = glm::normalize(lightPos - v2);
	glm::vec3 lightDir3 = glm::normalize(lightPos - v3);
	printf("Direction from v1: %fi, %fj, %fk\n", lightDir1.x, lightDir1.y, lightDir1.z);
	printf("Direction from v2: %fi, %fj, %fk\n", lightDir2.x, lightDir2.y, lightDir2.z);
	printf("Direction from v3: %fi, %fj, %fk\n", lightDir3.x, lightDir3.y, lightDir3.z);

	double angle1 = acos(glm::dot(lightDir1, normal) / (glm::length(lightDir1)*glm::length(normal))) * (180.0f / PII);
	printf("4. Angle between lightsource and v1 normal in degrees: %f\n", angle1);
	double angle2 = acos(glm::dot(lightDir2, normal) / (glm::length(lightDir2)*glm::length(normal))) * (180.0f / PII);
	printf("Angle between lightsource and v2 normal in degrees: %f\n", angle2);
	double angle3 = acos(glm::dot(lightDir3, normal) / (glm::length(lightDir3)*glm::length(normal))) * (180.0f / PII);
	printf("Angle between lightsource and v3 normal in degrees: %f\n", angle3);

	printf("5. v3 will be the brightest vertex as it's angle is the smallest so it is nearest to the light\n");
	bool visible = isVisible(v1, v2, v3, glm::vec3(0.0, 2.0, 0.5));
	printf("6. With a camera at pos (0,2,0.5) is the triangle visible?");
	printf(visible ? " True\n" : " False\n");
	visible = isVisible(v1, v2, v3, glm::vec3(0.5, -2.0, 0.0));
	printf("With a camera at pos (0.5,-2,0.0) is the triangle visible?");
	printf(visible ? " True\n" : " False\n");

	glm::mat4 t1 = glm::mat4(1.0);
	t1 = glm::translate(t1, glm::vec3(2.0, 1.0, 3.0));

	printf("7. Tranlation matrix to move by (2,1,3)\n");
	printMat4(t1);

	glm::mat4 r1 = glm::mat4();
	r1 = glm::rotate(glm::radians(-45.f), glm::vec3(0.0, 1.0, 0.0));
	printf("8. Rotation matrix to rotate 45 degrees around Y axis\n");
	printMat4(r1);

	glm::mat4 m1 = r1*t1;
	printf("9. Compound matrix applying t1 then r1\n");
	printMat4(m1);

	glm::mat4 m2 = t1*r1;
	printf("10. Compound matrix applying r1 then t1\n");
	printMat4(m2);

	printf("11. There is a difference between the two compount martices due to the order of multiplication. \n This results in the order of opertaions to change.\n The first matrix is a translate then rotate while the second is a rotate then translate.\n");

	system("pause");
	return 0;
}

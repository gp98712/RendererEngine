#pragma once
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	// glm::vec3 color;
};

/*
* Material�� �ַ� 
*/
struct Material {
	glm::vec3 ambient = glm::vec3(0.1f);
	float shininess = 1.0f;
	glm::vec3 diffuse = glm::vec3(0.5f);
	// TMI: DirectX������ constant buffer�� ������ ������ 4byte ������ �����ϱ� ����
	// ���⿡ float dummy padding�� �־���Ѵ�.
	glm::vec3 specular = glm::vec3(0.5f);
};

struct Light {
	glm::vec3 strength = glm::vec3(1.0f);
	float fallOffStart = 0.0f;

	// opengl�� right hand ��ǥ�踦 ���� ������ -z ������ �����̴�.
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
	float fallOffEnd = 10.0f;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);
	float spotPower = 0.0f;
};
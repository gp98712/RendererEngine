#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Util/Shader.h"
#include "assimp/aabb.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	glm::vec3 tangentModel;

	// ���� ������ ���ϱ� ����� �̻��� ������ 1.0���� �ʱ�ȭ.
	glm::vec3 color = {
		1.0f, 1.0f, 1.0f 
	};
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures = {});

	void Draw(Shader &shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
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
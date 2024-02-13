#pragma once
#include "../VertexType.h"
#include <memory>
#include <vector>

using namespace std;

class Primitive
{
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::shared_ptr<Mesh> mesh;

	virtual void Draw(const char* shaderProgramName) {
		std::cout << "�̰� �������̵� ���ּ���" << std::endl;
	}

	virtual void SetTexture() {
		std::cout << "�̰� �������̵� ���ּ���" << std::endl;
	}

	void CalculateTangents();

public:
	~Primitive();

	void PutModelUniform(const char* shaderProgramName);

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
};


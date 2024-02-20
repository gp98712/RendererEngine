#pragma once
#include "../MeshMaterialsLight.h"
#include <memory>
#include <vector>
#include "BaseObject.h"

using namespace std;

class Primitive : public BaseObject
{
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	glm::vec3 vertexAveragePosition;

	void CalculateTangents();


	virtual void Draw(const char* shaderProgramName) {
		std::cout << "�̰� �������̵� ���ּ���" << std::endl;
	}

	virtual void SetTexture() {
		std::cout << "�̰� �������̵� ���ּ���" << std::endl;
	}

public:
	bool isAlphaMesh = false;
	std::shared_ptr<Mesh> mesh;

	void SetTexture(std::string path, std::string type);
	void SetupMesh();
	void CalculateVertexAveragePosition();

	~Primitive();
};


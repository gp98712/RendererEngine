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

	virtual void Draw() {
		std::cout << "�̰� �������̵� ���ּ���" << std::endl;
	}

	virtual void SetTexture() {
		std::cout << "�̰� �������̵� ���ּ���" << std::endl;
	}

	void CalculateTangents();

public:
	~Primitive();
};


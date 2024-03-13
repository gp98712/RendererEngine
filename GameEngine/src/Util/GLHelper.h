#pragma once
#include <vector>
#include <glm/glm.hpp>

extern std::vector<float> quadVertices;

// PostProcessing�� ���Ǵ� Quad ���� ���.
struct SimpleQuad {
	unsigned int VAO;
	unsigned int VBO;
	std::vector<float> quadVertices = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
};

SimpleQuad CreateSimpleQuad();

// ȭ�鿡 ������ �ؽ�ó ����
unsigned int CreateSimpleTexture();

glm::vec2 WorldToScreen(glm::vec3 worldPos, glm::mat4 view, glm::mat4 position);

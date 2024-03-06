#pragma once
#include <vector>


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

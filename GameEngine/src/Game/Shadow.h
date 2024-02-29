#pragma once
#include <iostream>
#include "MeshRenderer.h"
#include "Camera.h"
#include <memory>
#include "ShadowInterface.h"

class Shadow : public ShadowInterface
{
private:
	// �����뵵 vao
	unsigned int quadVAO;
	unsigned int quadVBO;
	std::vector<float> quadVertices;

public:
	Shadow();

	void WriteDepthMap(shared_ptr<MeshRenderer> meshRenderer,glm::mat4 lightSpaceMatrix);

	// �����뵵. �׸��ڸ� ����.
	void showDepthMap();
};


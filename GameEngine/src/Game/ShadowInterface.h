#pragma once
#include <memory>
#include "MeshRenderer.h"
#include "glm/glm.hpp"

class ShadowInterface {
public:
	const unsigned int SHADOW_WIDTH = 2048 * 2;
	const unsigned int SHADOW_HEIGHT = 2048 * 2;

	virtual void WriteDepthMap(shared_ptr<MeshRenderer> meshRenderer, glm::mat4 lightSpaceMatrix) {
		std::cerr << "�Ϲ� �׸��� �����ε� ���ּ���..." << std::endl;
	}

	virtual void WriteDepthMap(
		shared_ptr<MeshRenderer> meshRenderer, 
		std::vector<glm::mat4> lightSpaceMatrix,
		float far,
		glm::vec3 position
	) {
		std::cerr << "����Ʈ �׸��� �����ε� ���ּ���..." << std::endl;
	}

	// direcitonal shader�뵵
	unsigned int depthMapFrameBuffer; // �׸��� ������ ���� �����ӹ���.
	unsigned int depthMap; // �׸��� ������ ���� �ؽ�ó.

	// ť��� ���̴� �뵵.
	unsigned int depthCubeMap;
	unsigned int depthMapFBO;

	unsigned int getDepthMapTexture(int lightType) {
		if (lightType == 1) {
			return depthCubeMap;
		}

		return depthMap;
	}

	unsigned int getDepthMapFrameBuffer(int lightType) {
		if (lightType == 1) {
			return depthMapFBO;
		}

		return depthMapFrameBuffer;
	}


};

#pragma once
#include <memory>
#include "MeshRenderer.h"
#include "glm/glm.hpp"

class ShadowInterface {
private:
	const unsigned int SHADOW_WIDTH = 2048 * 2;
	const unsigned int SHADOW_HEIGHT = 2048 * 2;

public:
	virtual void WriteDepthMap(shared_ptr<MeshRenderer> meshRenderer, glm::mat4 lightSpaceMatrix) {
		std::cerr << "�Ϲ� �׸��� �����ε� ���ּ���..." << std::endl;
	}

	virtual void WriteDepthMap(shared_ptr<MeshRenderer> meshRenderer, std::vector<glm::mat4> lightSpaceMatrix) {
		std::cerr << "����Ʈ �׸��� �����ε� ���ּ���..." << std::endl;
	}


};

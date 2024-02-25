#pragma once
#include <iostream>
#include "../Game/MeshRenderer.h"
#include "Camera.h"
#include <memory>

class Shadow
{
private:
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

public:
	Shadow();

	void WriteDepthMap(shared_ptr<MeshRenderer> meshRenderer, Camera* camera);

	unsigned int depthMapFrameBuffer; // �׸��� ������ ���� �����ӹ���.
	unsigned int depthMap; // �׸��� ������ ���� �ؽ�ó.
};


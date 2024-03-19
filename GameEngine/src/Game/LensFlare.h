#pragma once
#include <glm/glm.hpp>
#include <glew.h>
#include "../Util/GLHelper.h"
#include "ImguiController.h"
#include "../Util/Shader.h"


/*
* �˰���
* 
* 1. Flare�� DownSample�Ѵ�.
* 2. Lens Flare ����� �ִ´� Ghosting, Halo, Starbust, Streaks
* 3. Blur ó���Ѵ�.
* 4. Upsample �� �� Original Image�� ��ģ��.
*/
class LensFlare
{
public:
	LensFlare();

	unsigned int downSampledTexture;
	unsigned int lensFlareTexture;

	unsigned int vao, vbo;
	unsigned int lensFlareFBO;

	void Draw(unsigned int sceneTexture);

};


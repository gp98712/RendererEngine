#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Bloom.h"
#include "GodRays.h"
#include "GBuffer.h"
#include "MeshRenderer.h"

class GraphicsPipeLine
{
private:
	void CreateVAO();
	void CreateMSAAFrameBuffer();
	void CreateIntermediateFrameBuffer();
	const char* programName = "gBuffer";

public:
	GraphicsPipeLine();
	void PutExposure(const char* programName);
	void DrawGBuffer(
		shared_ptr<MeshRenderer> mesh,
		std::shared_ptr<Camera> camera
	);
	void DrawGBuffer(shared_ptr<MeshRenderer> mesh,
		glm::vec3 lightPos,
		std::shared_ptr<Camera> camera);

	unsigned int msaaFrameBuffer;
	unsigned int msaaTexture; // �÷����� �ؽ�ó, MSAA

	unsigned int intermediateFrameBuffer;
	unsigned int screenTexture; // �÷����� �ؽ�ó, MSAA ����

	unsigned int rbo;
	unsigned int vao;
	unsigned int vbo;
	void use();

	void Draw(const char* programName);

	std::shared_ptr<Bloom> bloom;
	std::shared_ptr<GBuffer> gBuffer;
	std::shared_ptr<GodRays> godRays;
	std::shared_ptr<Bloom> godRayBloom;

	float exposure = 1.0f;

	float bloomThreshold = 0.04f;
};


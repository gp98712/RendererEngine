#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Bloom.h"

class PostProcessingFrameBuffer
{
private:
	void CreateVAO();
	void CreateMSAAFrameBuffer();
	void CreateIntermediateFrameBuffer();

public:
	PostProcessingFrameBuffer();
	void PutExposure(const char* programName);

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

	float exposure = 1.0f;

	float bloomThreshold = 0.04f;
};


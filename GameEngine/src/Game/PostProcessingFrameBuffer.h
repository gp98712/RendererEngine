#pragma once
#include <iostream>
#include <vector>

class PostProcessingFrameBuffer
{
private:
	void CreateVAO();
	void CreateMSAAFrameBuffer();
	void CreateIntermediateFrameBuffer();

public:
	PostProcessingFrameBuffer();

	unsigned int msaaFrameBuffer;
	unsigned int msaaTexture; // �÷����� �ؽ�ó, MSAA

	unsigned int intermediateFrameBuffer;
	unsigned int screenTexture; // �÷����� �ؽ�ó, MSAA ����

	unsigned int rbo;
	unsigned int vao;
	unsigned int vbo;
	void use();

	void Draw(const char* programName);

	float exposure = 1.0f;
};


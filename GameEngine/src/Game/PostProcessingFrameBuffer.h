#pragma once
#include <iostream>
#include <vector>

class PostProcessingFrameBuffer
{
private:
	void CreateVAO();
	void CreateMSAAFrameBuffer();
	void CreateIntermediateFrameBuffer();
	void CreateDepthMapFrameBuffer();

	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

public:
	PostProcessingFrameBuffer();

	unsigned int depthMapFrameBuffer; // �׸��� ������ ���� �����ӹ���.
	unsigned int depthMap; // �׸��� ������ ���� �ؽ�ó.

	unsigned int msaaFrameBuffer;
	unsigned int msaaTexture; // �÷����� �ؽ�ó, MSAA

	unsigned int intermediateFrameBuffer;
	unsigned int screenTexture; // �÷����� �ؽ�ó, MSAA ����

	unsigned int rbo;
	unsigned int vao;
	unsigned int vbo;
	void use();

	std::vector<float> quadVertices;

	void Draw(const char* programName);
};


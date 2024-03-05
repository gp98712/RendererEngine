#include "PostProcessingFrameBuffer.h"
#include <glew.h>
#include "../Constants.h"
#include "../Util/Shader.h"
#include "../Util/GLHelper.h"
#include <memory>

PostProcessingFrameBuffer::PostProcessingFrameBuffer()
{
	bloom = std::make_shared<Bloom>();
	CreateVAO();
	CreateMSAAFrameBuffer();
	CreateIntermediateFrameBuffer();
}

void PostProcessingFrameBuffer::Draw(const char* programName)
{

	auto shader = Shader::getInstance();
	
	bloom->CopySceneTexture(msaaFrameBuffer);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFrameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFrameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// 기본 컬러버퍼 사용.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_FRAMEBUFFER_SRGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(Shader::getInstance()->getShaderProgram(programName));
	PutExposure(programName);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_FRAMEBUFFER_SRGB);
}

void PostProcessingFrameBuffer::use()
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessingFrameBuffer::PutExposure(const char* programName) {
	auto shader = Shader::getInstance();
	auto progrma = shader->getShaderProgram(programName);
	glUseProgram(progrma);
	shader->setFloat(programName, "exposure", exposure);
}

void PostProcessingFrameBuffer::CreateIntermediateFrameBuffer() {
	glGenFramebuffers(1, &intermediateFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFrameBuffer);

	screenTexture = CreateSimpleTexture();

	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		screenTexture,
		0
	);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Intermediate Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessingFrameBuffer::CreateMSAAFrameBuffer() {
	glGenFramebuffers(1, &msaaFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);

	// MSAA용 컬러버퍼 텍스처 생성
	glGenTextures(1, &msaaTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTexture);
	glTexImage2DMultisample(
		GL_TEXTURE_2D_MULTISAMPLE,
		4,
		GL_RGBA32F,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		GL_TRUE
	);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// 프레임버퍼 - 텍스처 연결할때 사용하는 함수.
	// target: 연결 대상 프레임버퍼
	// attachment: 텍스처 종료, COLOR_ATTACHMENT0 <- 컬러버퍼
	// texTarget: 연결할 텍스처 종류
	// texTarget : 텍스처 ID
	// level: mip level
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D_MULTISAMPLE,
		msaaTexture,
		0
	);

	// Bloom 텍스처 바인딩
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D_MULTISAMPLE,
		bloom->sceneTexture,
		0
	);

	// rbo 생성
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(
		GL_RENDERBUFFER,
		4,
		GL_DEPTH24_STENCIL8,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		rbo
	);
	/*
	* 2 모두 MSAA 텍스처
	* 0: FrameBuffer
	* 1: Bloom FrameBuffer
	*/
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);


	// opengl 에러 체크
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessingFrameBuffer::CreateVAO()
{
	SimpleQuad result = CreateSimpleQuad();

	vao = result.VAO;
	vbo = result.VBO;
}


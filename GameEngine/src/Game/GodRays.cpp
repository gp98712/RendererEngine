#include "GodRays.h"
#include "../Constants.h"
#include "ImguiController.h"

GodRays::GodRays() {
	// 텍스처 생성
	glGenTextures(1, &godRayTexture);
	glBindTexture(GL_TEXTURE_2D, godRayTexture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB8,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0,
		GL_RGB,
		GL_FLOAT,
		NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &godRaySceneTexture);
	glBindTexture(GL_TEXTURE_2D, godRaySceneTexture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0,
		GL_RGBA,
		GL_FLOAT,
		NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// 프레임버퍼 생성
	glGenFramebuffers(1, &godRayFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, godRayFrameBuffer);

	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		godRaySceneTexture,
		0
	);

	// vao 생성
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(float), &quad[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	// rbo 생성
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// godRay에 있을 원형 구체.

	rayCircle = std::make_shared<Circle>();
	rayCircle->scale = glm::vec3(2.0f);
	rayCircle->SetupMesh();

	// 임구이 바인딩
	auto imguiController = ImguiController::getInstance();
	imguiController->godLightTexture = godRayTexture;
	imguiController->decay = decay;
	imguiController->density = density;
	imguiController->weight = weight;
	imguiController->godRayExposure = exposure;
}

void GodRays::ImGuiUpdate() {
	auto imguiController = ImguiController::getInstance();
	decay = imguiController->decay;
	density = imguiController->density;
	weight = imguiController->weight;
	exposure = imguiController->godRayExposure;
}

void GodRays::Draw(
	shared_ptr<MeshRenderer> mesh,
	glm::vec3 rayPosition,
	std::shared_ptr<Camera> camera) {
	const char* godRayProgramName = "god-ray";
	glBindFramebuffer(GL_FRAMEBUFFER, godRayFrameBuffer);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		godRaySceneTexture,
		0
	);
	// rbo도 넣어야할까? 고민해보자.
	auto shader = Shader::getInstance();
	auto program = shader->getShaderProgram(godRayProgramName);
	glUseProgram(program);
	
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	camera->putCameraUniform(godRayProgramName);
	shader->setVec3(godRayProgramName, "rayPosition", rayPosition);
	shader->setBool(godRayProgramName, "isGodRay", false);


	mesh->Draw(godRayProgramName);

	shader->setBool(godRayProgramName, "isGodRay", true);
	rayCircle->position = rayPosition;
	rayCircle->Draw(godRayProgramName);

	glm::vec2 screenPos = WorldToScreen(rayPosition, camera->view, camera->projection);

	// worldToScreen은 왼쪽상단이 0,0 오른쪽 하단이 WINDOW_WIDTH, WINDOW_HEIGHT 좌표계
	// 이를 opengl 텍스처 좌표계로 변환해야함.
	screenPos.x = screenPos.x / WINDOW_WIDTH;
	screenPos.y = 1.0 - (screenPos.y / WINDOW_HEIGHT);

	// 포스트 프로세싱하기 위한 코드.
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		godRayTexture,
		0
	);

	const char* rayEffect = "god-ray-effect";

	program = shader->getShaderProgram(rayEffect);
	glUseProgram(program);

	ImGuiUpdate();
	shader->setFloat(rayEffect, "decay", decay);
	shader->setFloat(rayEffect, "density", density);
	shader->setFloat(rayEffect, "weight", weight);
	shader->setFloat(rayEffect, "exposure", exposure);

	shader->setVec2(rayEffect, "lightPosition", screenPos);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, godRaySceneTexture);
	shader->setInt(rayEffect, "godRayTexture", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GodRays::~GodRays() {
	glDeleteFramebuffers(1, &godRayFrameBuffer);
	glDeleteTextures(1, &godRayTexture);
	glDeleteRenderbuffers(1, &rbo);
}

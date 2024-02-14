#include "Camera.h"
#include "../Util/Shader.h"
#include <SDL.h>
#include "Input.h"

Camera::Camera(float fov, int width, int height) {
	projection = glm::perspective(fov, float(width) / height, near, far);
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	/*
	* ����.
	* ī�޶� -> Ÿ�� ���Ͱ� �ƴ�
	* ī�޶� <- Ÿ�� �����̴�.
	* 
	* 3D ���� ī�޶� �����̴°� �ƴ϶� ������ �����̴� ���̱� �����̴�.
	* �����ϱ� �������� �ּ��� ���ܳ��ڴ�.
	*/
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	view = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp
	);
}

void Camera::CameraLookAround(float x, float y) {
	yaw += x * sensitive;
	pitch += y * sensitive;

	cameraDirection = glm::vec3(
		cos(yaw * cos(pitch)),
		sin(pitch),
		sin(yaw * cos(pitch))
	);
	cameraFront = glm::normalize(cameraDirection);
}

void Camera::Update(float deltaTime) {
	const auto& Input = Input::GetInstance();
	const float cameraSpeed = 2.5f * deltaTime;

	if (Input->IsKeyPressed(SDL_SCANCODE_W)) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (Input->IsKeyPressed(SDL_SCANCODE_S)) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (Input->IsKeyPressed(SDL_SCANCODE_A)) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (Input->IsKeyPressed(SDL_SCANCODE_D)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);

	view = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp
	);
}

void Camera::putCameraUniform(const char* shaderProgramName) {
	auto shader = Shader::getInstance();

	shader->setMat4(shaderProgramName,"projection", projection);
	shader->setMat4(shaderProgramName,"view", view);
}
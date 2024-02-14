#include "Camera.h"
#include "../Util/Shader.h"
#include <SDL.h>

Camera::Camera(float fov, int width, int height) {
	projection = glm::perspective(fov, float(width) / height, near, far);
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
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
		cameraTarget,
		cameraUp
	);
}

void Camera::Update() {
	auto time = SDL_GetTicks() / 5000.0f;
	float camX = sin(time * 3.0f) * radius;
	float camZ = cos(time * 3.0f) * radius;

	cameraPos = glm::vec3(camX, 0.0f, camZ);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);

	view = glm::lookAt(
		cameraPos,
		cameraTarget,
		cameraUp
	);
}

void Camera::putCameraUniform(const char* shaderProgramName) {
	auto shader = Shader::getInstance();

	shader->setMat4(shaderProgramName,"projection", projection);
	shader->setMat4(shaderProgramName,"view", view);
}
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	float radius = 3.0f;
	float yaw = glm::radians(-90.0f);
	float pitch = 0.0f;
	float sensitive = 1.0;

public:
	// 45���� ������ ����.
	Camera(float fov, int width, int height);
	float near = 0.1f;
	float far = 100.0f;
	glm::mat4 projection;

	// ����� ���� ī�޶� �̱����̶�. ���ó��.
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;

	glm::mat4 view;

	void Update(float deltaTime);
	void CameraLookAround(float x, float y);

	void putCameraUniform(const char* shaderProgramName);
};


#pragma once
#include "../GameObject/PrimitiveObject/Box.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Shadow.h"
#include "PointShadow.h"
#include "ShadowInterface.h"

class Shadow;

struct LightPower {
	float constant;
	float linear;
	float quadratic;
};


class Light
{
private:
	static std::vector<LightPower> lightPowers;

	// �׸��� ����� ���� near,far��.
	// �ָ��ִ� ��ü�� �׸��ڸ� �������� �ʴ´�.
	float near = 0.1f;
	float far = 30.0f;

public:
	shared_ptr<Box> box;

	shared_ptr<ShadowInterface> shadow;

	Light(int lightType);
	glm::mat4 lightSpaceMatrix;
	void Draw(const char* shaderProgramName);
	// 0 : directional , 1 : point, 2 : spot
	int lightType = 1;

	// ������.
	int lightPower = 3;

	bool isDisable = false;

	// directional light, spot light �� ���.
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

	glm::vec3 strength = glm::vec3(1.0f, 1.0f, 1.0f);

	float cutOff = glm::cos(glm::radians(12.5f));
	float cutOuter = glm::cos(glm::radians(17.5f));

	// �׸��� ���ν� �þ� ��ȯ�� ���� ������ ������ �Լ�.
	void MakeShadow(shared_ptr<MeshRenderer> meshRenderer);
	void PutShadowMap(const char* shaderProgramName,int index, int current);
	void MakeDirectionalShadow(const char* shaderProgramName, shared_ptr<MeshRenderer> meshRenderer);
	void MakePointShadow(const char* shaderProgramName, shared_ptr<MeshRenderer> meshRenderer);

	void PutLightUniform(const char* shaderProgramName, int lightPosition);

	glm::vec3 getPosition();

	void setPosition(glm::vec3 position);
	void lookHere(glm::vec3 point);

	glm::quat yRotationQuat = glm::angleAxis(
		glm::radians(0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	void Update(float deltaTime);
	void CalculateLightSpaceMatrix();
};


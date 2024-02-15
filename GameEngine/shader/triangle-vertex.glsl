#version 330 core
#include common.glsl
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 tangentModel;
layout (location = 4) in vec3 aColor;

out vec2 TexCoord;
out vec3 posWorld;
out vec3 normalWorld;

void main() {
	// ��ġ�����̱� ������ 1.0���� ����
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	posWorld = (model * vec4(aPos, 1.0f)).xyz;

	// normal�� ���������̱� ������ 0.0���� ����.
	// 4������ 0�̸� �̵� ��ȯ�� ������� �ʱ� ������ �������ʹ� 0.0�̴�.
	normalWorld = (invTranspose * vec4(aNormal,0.0f)).xyz;
	TexCoord = aTexCoord;
}
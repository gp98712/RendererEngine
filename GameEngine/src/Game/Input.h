#pragma once
#include <SDL.h>
#include <glm/glm.hpp>
#include "../Game/Camera.h"


class Input
{
private:
	static Input* instance;
	Input();
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
public:
	static Input* GetInstance() {
		if (instance == nullptr) {
			instance = new Input();
		}

		return instance;
	}

	const Uint8* state;

	bool IsKeyPressed(int key) {
		return state[key];
	}

	bool rightMouseButtonDown = false;
	/*
	* ���� ������ ��ǥ��� ���� ����� (0, 0) ������
	* 
	* ���ǻ� Input Ŭ�������� ���콺 ��ǥ��� NDC�� �����ϴ�.
	*/
	glm::vec2 mousePos;
	glm::vec2 ScreenNormalize(int x, int y);
	void SetMouse();
};


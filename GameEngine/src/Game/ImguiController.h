#pragma once
#include <SDL.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>
class ImguiController
{

public:
	static ImguiController* instance;
	ImguiController(SDL_Window* window, SDL_GLContext context);
	~ImguiController();
	void Update();
	void Render();

	float f = 0.0f;
	glm::vec3 clear_color = glm::vec3(0.45f, 0.55f, 0.60f);
	int counter = 0;

	ImguiController& operator=(const ImguiController&) = delete; // �Ҵ� ������ ��Ȱ��ȭ
	ImguiController(const ImguiController&) = delete; // ���� ������ ��Ȱ��ȭ

	static void CreateInstance(SDL_Window* window, SDL_GLContext context) {
		instance = new ImguiController(window, context);
	}

	static ImguiController* getInstance() {
		return instance;
	}

};


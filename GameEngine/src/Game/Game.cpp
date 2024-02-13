#include "Game.h"
#include <SDL.h>
#include <GL/glew.h>
#include "../Util/Shader.h"
#include "../Util/stb_image.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>
#include "./ImguiController.h"
#include "../GameObject/PrimitiveObject/Plane.h"
#include "../Constants.h"

Game::Game() {
	mWindow = nullptr;
	mIsRunning = true;

}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("SDL �ʱ�ȭ ����: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"My Game Practices",
		100,
		100,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL
	);

	if (!mWindow) {
		SDL_Log("������ ���� ���� %s", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	context = SDL_GL_CreateContext(mWindow);

	if (!context) {
		SDL_Log("GL Context ���� ����! %s", SDL_GetError());
	}

	if (glewInit() != GLEW_OK) {
		SDL_Log("GLEW �ʱ�ȭ ����!");
		return false;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	ImguiController::CreateInstance(mWindow, context);
	imguiController = ImguiController::getInstance();

	stbi_set_flip_vertically_on_load(true);

	// ���̴� ����

	auto shader = Shader::getInstance();
	shader->loadShaderProgram(
		"./shader/triangle-vertex.glsl",
		"./shader/triangle-fragment.glsl",
		"triangle"
	);

	plane = new Plane();
	box = new Box();
	circle = new Circle();
	camera = new Camera(
		75.0f,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

	// �Ÿ��� �������� ���̰� �ϱ� ���ؼ� x�� 0.3�� �̵�
	{
		plane->position = glm::vec3(-0.5f, 0.0f, 0.0f);
		box->position = glm::vec3(0.0f, 0.0f, 0.0f);
		circle->position = glm::vec3(0.5f, 0.0f, 0.0f);
	}

	// �������� ����

	{
		plane->scale = glm::vec3(0.2f, 0.2f, 0.2f);
		box->scale = glm::vec3(0.2f, 0.2f, 0.2f); 
	}



	return true;
}

void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	SDL_GL_DeleteContext(context);
	delete imguiController;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type) {
			//  �������� x ��ư�� ������ ���� ����
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Ű���� Ű�� ���ȴ��� ����
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}
}

void Game::UpdateGame() {

	/*
	* fps�� 60���� �����Ѵ�. (60FPS�� ���� 16.6ms�̴�.)
	* 
	* PS: fps�� 60���� �����ϴ� ����
	* 
	* ���� ����� �Ź� Update�Ҷ� �������� �޶����� ���� �Ի� ����� �޶�����.
	* ���⼱ ���� ���� ������� �ִ� fps�� 60���� ���� �Ŵ� ������� �ذ���
	*/
	while (!SDL_TICKS_PASSED(SDL_GetTicks(),mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();

	// ��Ÿ �ð��� �ʹ� ũ�� ������ ���ߴ� ���� ����
	/*
	* �� ������ �ʿ��� ����.
	* ���� sleep�� �ɾ��ų�, ����ŷ� �ý����� �ᱸ�� �ٽ� �����Ű��
	* deltaTime�� ũ�� �Ҿ�� ��찡 �߻��Ѵ�.
	* �̸� �����ϱ� ���ؼ� deltaTime�� 0.05f�� ���� �ʵ��� �����Ѵ�.
	* 60fps�� ������ �ɾ�ξ��� ������ �Ϲ����� ��Ȳ���� deltaTime��
	* 0.0166f �������ȴ�.
	*/
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// 3�� ������Ʈ.
	{
		plane->rotation += glm::vec3(30.0f,0.0f,0.0f) * deltaTime;
		box->rotation += glm::vec3(3.0f, 3.0f, 3.0f) * deltaTime;
		circle->rotation += glm::vec3(3.0f, 3.0f, 3.0f) * deltaTime;
	}
}

void Game::GenerateOutput() {
	imguiController->Update();

	float timeValue = SDL_GetTicks() / 1000.0f;

	auto program = Shader::getInstance()->getShaderProgram("triangle");
	// ���� ã�� ���ϸ� -1�̴�.
	glUseProgram(program);

	// �Ͼ������ �ʱ�ȭ.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_LINE);
	
	camera->putCameraUniform("triangle");
	plane->Draw("triangle");
	box->Draw("triangle");
	circle->Draw("triangle");

	imguiController->Render();
	SDL_GL_SwapWindow(mWindow);
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}

}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

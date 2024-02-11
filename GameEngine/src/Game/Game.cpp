#include "Game.h"
#include <SDL.h>
#include <GL/glew.h>
#include "../Util/Shader.h"
#include "../Util/stb_image.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>
#include "./ImguiController.h"

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
		1024,
		768,
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

	glViewport(0, 0, 1024, 768);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	SetOpenGL();

	ImguiController::CreateInstance(mWindow, context);
	imguiController = ImguiController::getInstance();

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
}

void Game::GenerateOutput() {
	imguiController->Update();

	float timeValue = SDL_GetTicks() / 1000.0f;
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

	auto program = Shader::getInstance()->getShaderProgram("triangle");
	// ���� ã�� ���ϸ� -1�̴�.
	int vertexColorLocation =
		glGetUniformLocation(program,"ourColor");
	glUseProgram(program);
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	glBindVertexArray(VAO);
	// �ؽ�ó ���ε�

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(program, "texture2"), 1);


	// �Ͼ������ �ʱ�ȭ.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	* 6: �ε��� ����
	* GL_UNSIGNED_INT: �ε��� Ÿ��
	* 0: �ε��� �迭 ���� ��ġ
	*/
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	imguiController->Render();
	SDL_GL_SwapWindow(mWindow);
}

void Game::SetOpenGL() {
	// �ؽ�ó �ε�
	int width, height, nrChannels;
	unsigned char* data = stbi_load("./assets/images/wall.jpg", &width, &height, &nrChannels, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	/*
	* S: ������ (x��)
	* T: ������ (y��)
	* Ư���� �� �ִ°��� �ƴϴ�. �׳� �������� �����̶��Ѵ�.
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/*
	* 
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D, // �ؽ�ó ���ε� ��� ����, ť����� GL_TEXTURE_3D�̴�.
		0, // mipmap ����
		GL_RGB, // internal formap
		width, 
		height,
		0, // border ũ��
		GL_RGB, // �Է� ������ ����
		GL_UNSIGNED_BYTE, // �ȼ� ������ Ÿ��
		data // �ȼ� ������ ������.
		);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	unsigned char* data2 = stbi_load("./assets/images/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	/*
	* mipmap? : �ؽ�ó ���� ���.
	* GPU���� �ؽ�ó�� �̸� �������� ���� ���� �������� �ؽ�ó�� �����д�.
	* ex) 256* 256 -> 128 * 128 -> 64 * 64 -> 32 * 32 -> 16 * 16 -> 8 * 8 -> 4 * 4 -> 2 * 2 -> 1 * 1
	* mipmap level ���� �̿� ��Ī�ȴ�.
	* mipmap 0 -> 256 * 256
	* mipmap 1 -> 128 * 128
	* 
	* ���� �Ÿ��� �� ��ü�� �������Ҷ� ���� �ػ󵵰� ���� �ؽ�ó�� �� �ʿ䰡 ������
	mipmap�� ������ �����ϸ� ����ȭ�� �ð��� ǰ���� ���� �� �ִ�.
	*/
	stbi_image_free(data);
	stbi_image_free(data2);


	// ���̴� �ε�
	auto shader = Shader::getInstance();
	auto program = shader->loadShaderProgram(
		"./shader/triangle-vertex.glsl",
		"./shader/triangle-fragment.glsl",
		"triangle"
	);

	// VAO, VBO, EBO ����
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/*
	* �� �ڵ��� �ǹ�.
	* 
	* GL_ARRAY_BUFFER�� ���ε��� Buffer OBJECT��
	* vertices.size() * sizeof(float) ��ŭ�� �޸𸮸� �Ҵ��ϰ�
	* vertices.data()�� �����͸� �����Ѵ�.
	* ���� GL_STATIC_DRAW�� �����Ͽ� �����Ͱ� ������ ������ ����Ѵ�.
	*/
	glBufferData(
		GL_ARRAY_BUFFER, 
		vertices.size() * sizeof(float),
		vertices.data(), 
		GL_STATIC_DRAW
	);

	// position
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	/*
	* color
	* 1: attribute location
	* 3: size
	* GL_FLOAT: type
	* GL_FALSE: normalized
	* 3 * sizeof(float): stride
	* (void*)(3 * sizeof(float)): offset
	*
	*/

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		indices.data(),
		GL_STATIC_DRAW
	);
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

#include "Game.h"
#include <SDL.h>
#include <glew.h>
#include "../Util/Shader.h"
#include "../Util/stb_image.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "./ImguiController.h"
#include "../GameObject/PrimitiveObject/Plane.h"
#include "../Constants.h"
#include <glm/gtc/quaternion.hpp>
#include <random>


Game::Game() {
	mWindow = nullptr;
	mIsRunning = true;

}

void Game::GenerateOutput() {
	imguiController->Update();

	float timeValue = SDL_GetTicks() / 1000.0f;
	auto shader = Shader::getInstance();

	// �����������!
	meshRenderer->ResetMesh();

	if (modelOn) {
		meshRenderer->AddMesh(backPack);
	}

	/*
	for (auto& circle : pbrTestCircle) {
		meshRenderer->AddMesh(circle);
	}
	*/
	

	meshRenderer->MeshAlignment(camera.get());

	lightManager->MakeShadow(meshRenderer);

	graphicsPipe->DrawGBuffer(
		meshRenderer,
		lightManager->directionLights[0]->box->position,
		camera
	);
	graphicsPipe->DrawSSAO(camera);

	// ���߿� ������ ó���ϵ��� ����
	graphicsPipe->godRays->Draw(
		meshRenderer,
		lightManager->directionLights[0]->box->position,
		camera,
		graphicsPipe->gBuffer->godRayTexture
	);

	graphicsPipe->use();
	
	//const char* shaderName = "simple-pbr-shading";
	const char* shaderName = "default";
	auto imguiController = ImguiController::getInstance();
	imguiController->PutPBRUniform(shaderName);

	graphicsPipe->PutExposure(shaderName);
	cubeMap->PutCubeMapTexture(shaderName);
	camera->putCameraUniform(shaderName);
	lightManager->PutLightUniform(shaderName);

	meshRenderer->Draw(shaderName);
	//lightManager->DrawLight(camera);
	cubeMap->Draw("cubemap", camera.get());

	auto normalMode = ImguiController::getInstance()->showNormal;

	if (normalMode) {
		lightManager->PutLightUniform("normal");
		camera->putCameraUniform("normal");
		meshRenderer->Draw("normal");
	}

	graphicsPipe->Draw("hdr");

	imguiController->Render();
	SDL_GL_SwapWindow(mWindow);
}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("SDL �ʱ�ȭ ����: %s", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


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
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	ImguiController::CreateInstance(mWindow, context);
	imguiController = ImguiController::getInstance();

	stbi_set_flip_vertically_on_load(true);

	// ���̴� ����
	CreateShaderProgram();
	meshRenderer = make_shared<MeshRenderer>();

	graphicsPipe = make_shared<GraphicsPipeLine>();
	cubeMap = make_shared<CubeMap>("./assets/hdr-cubemap/");

	// ȭ�鿡 �׸� ������Ʈ�� ����
	plane = make_shared<Plane>();
	plane->scale = glm::vec3(5.0f, 5.0f, 5.0f);
	plane->position = glm::vec3(0.0f, 0.0f, -2.0f);
	plane->SetTexture("./assets/images/bricks2.jpg", "albedo");
	plane->SetTexture("./assets/images/bricks2_normal.jpg", "normal");
	plane->SetTexture("./assets/images/bricks2_disp.jpg", "height");
	plane->SetupMesh();

	plane->rotation = glm::vec3(-90.0f, 0.0f, 0.0f);

	vector<glm::vec3> translations = {
		glm::vec3(-1.5f,0.0f,-0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

	for (int i = 0; i < translations.size(); i++) {
		auto box = make_shared<Box>();

		box->SetTexture("./assets/container2.png", "albedo");
		box->SetTexture("./assets/container2_specular.png", "specular");
		box->SetupMesh();
		box->scale = glm::vec3(0.5f, 0.5f, 0.5f);
		box->position = translations[i] + glm::vec3(0.0f, 0.5f, 0.0f);

		this->box.push_back(box);
		box->isAlphaMesh = false;
	}

	for (int i = 0; i < translations.size(); i++) {
		auto grass = make_shared<Plane>();
		grass->SetTexture("./assets/images/grass.png", "albedo");
		grass->SetupMesh();
		grass->scale = glm::vec3(0.5f, 0.5f, 0.5f);
		grass->position = translations[i] + glm::vec3(0.0f,0.5f,0.6f);

		this->grass.push_back(grass);
		grass->isAlphaMesh = true;
	}

	auto _circle = make_shared<Circle>();
	_circle->SetTexture("./assets/pbr-test/knotty-plywood_albedo.png", "albedo");
	_circle->SetTexture("./assets/pbr-test/knotty-plywood_metallic.png", "metallic");
	_circle->SetTexture("./assets/pbr-test/knotty-plywood_roughness.png", "roughness");
	_circle->SetTexture("./assets/pbr-test/knotty-plywood_normal-ogl.png", "normal");
	_circle->SetTexture("./assets/pbr-test/knotty-plywood_ao.png", "ao");
	_circle->SetTexture("./assets/pbr-test/knotty-plywood_height.png", "height");
	_circle->SetupMesh();
	_circle->position = glm::vec3(0.0f, 1.0f, 0.0f);


	// ���� : metallic
	// ������ : roughness
	int count = 0;
	for (float i = 0; i < 5; i++) {
		for (float j = 0; j < 5; j++) {
			auto forPbrTestCircle = make_shared<Circle>();
			forPbrTestCircle->SetupMesh();
			forPbrTestCircle->position = glm::vec3(j, i, 0.0f);

			this->pbrTestCircle.push_back(forPbrTestCircle);

			float metallic = (float)i / 5.0;
			float roughness = (float)j / 5.0;

			this->pbrTestMetallic.push_back(metallic);
			this->pbrTestRoughness.push_back(roughness);
			count++;
		}
		count = 0;
	}

	if (modelOn) {
		backPack = make_shared<Model>("./assets/pbrSponza/sponza/Sponza.gltf");
		//backPack = make_shared<Model>("./assets/interogation_room/scene.gltf");
	}

	camera = make_shared<Camera>(
		90.0f,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

	camera->cameraPos = glm::vec3(0.0f, 1.0f, 4.0f);

	lightManager = LightManager::getInstance();

	auto imguiController = ImguiController::getInstance();
	
	// ����Ʈ 
	lightManager->CreateLight
	(
		0,
		glm::vec3(0, 20, -0.001),
		glm::vec3(0, -1, 0.005),
		12
	);
	imguiController->directionalLightPosition = lightManager->directionLights[0]->box->position;
	imguiController->directionalLightDirection = lightManager->directionLights[0]->direction;
	imguiController->directionalLightDepthMap = lightManager->directionLights[0]->shadow->depthMap;


	// ����Ʈ.
	lightManager->CreateLight
	(
		1,
		glm::vec3(-0.116, 2.117, -1.116),
		glm::vec3(-0.042, -0.390, 0.952),
		1
	);

	input = Input::GetInstance();

	return true;
}

void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	SDL_GL_DeleteContext(context);
	delete imguiController;
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

	camera->Update(deltaTime);

	accTime += deltaTime;

	input->SetMouse();

	auto imguiController = ImguiController::getInstance();

	const auto exposure = imguiController->exposure;

	graphicsPipe->exposure = exposure;


	if (imguiController->useSun) {
		lightManager->directionLights[0]->setPosition(imguiController->directionalLightPosition);
		lightManager->directionLights[0]->direction = imguiController->directionalLightDirection;

		lightManager->EnableDirectionalLight(0);
	}
	else {
		lightManager->DisableDirectionalLight(0);
	}

	if (imguiController->usePointLight) {
		lightManager->EnablePointLight(0);
		//lightManager->EnablePointLight(1);
	}
	else {
		lightManager->DisablePointLight(0);
		//lightManager->DisablePointLight(1);
	}

	graphicsPipe->bloomThreshold = imguiController->bloomThreshold;

	//lightManager->UpdateLight(deltaTime);

	cubeMap->select = imguiController->select;
	cubeMap->lodLevel = imguiController->lodLevel;

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
		case SDL_QUIT:
			mIsRunning = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_RIGHT) {
				input->rightMouseButtonDown = true;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			input->rightMouseButtonDown = false;
			break;
		}
	}

	// Ű���� Ű�� ���ȴ��� ����
	const Uint8* state = input->state;
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	if (state[SDL_SCANCODE_F]) {
		camera->ResetPitch();
	}
}


void Game::CreateShaderProgram() {
	auto shader = Shader::getInstance();
	shader->loadShaderProgram(
		"./shader/default-vertex.glsl",
		"./shader/default-fragment.glsl",
		"default"
	);

	shader->loadShaderProgram(
		"./shader/normal-vertex.glsl",
		"./shader/normal-fragment.glsl",
		"normal",
		"./shader/normal-geometry.glsl"
	);

	shader->loadShaderProgram(
		"./shader/light-vertex.glsl",
		"./shader/light-fragment.glsl",
		"light"
	);

	shader->loadShaderProgram(
		"./shader/hdr-vertex.glsl",
		"./shader/hdr-fragment.glsl",
		"hdr"
	);

	shader->loadShaderProgram(
		"./shader/hdr-vertex.glsl",
		"./shader/up-sample-fragment.glsl",
		"up-sample"
	);

	shader->loadShaderProgram(
		"./shader/hdr-vertex.glsl",
		"./shader/down-sample-fragment.glsl",
		"down-sample"
	);

	shader->loadShaderProgram(
		"./shader/cubemap-vertex.glsl",
		"./shader/cubemap-fragment.glsl",
		"cubemap"
	);

	shader->loadShaderProgram(
		"./shader/shadow-vertex.glsl",
		"./shader/shadow-fragment.glsl",
		"shadow"
	);

	shader->loadShaderProgram(
		"./shader/shadow-debug-vertex.glsl",
		"./shader/shadow-debug-fragment.glsl",
		"shadow-debug"
	);

	shader->loadShaderProgram(
		"./shader/point-shadow-vertex.glsl",
		"./shader/point-shadow-fragment.glsl",
		"point-shadow",
		"./shader/point-shadow-geometry.glsl"
	);

	shader->loadShaderProgram(
		"./shader/simple-shading-vertex.glsl",
		"./shader/simple-shading-fragment.glsl",
		"simple-shading"
	);

	shader->loadShaderProgram(
		"./shader/simple-shading-vertex.glsl",
		"./shader/simple-pbr-shading-fragment.glsl",
		"simple-pbr-shading"
	);

	shader->loadShaderProgram(
		"./shader/hdr-vertex.glsl",
		"./shader/god-ray-post-processing-fragment.glsl",
		"god-ray-effect"
	);

	shader->loadShaderProgram(
		"./shader/hdr-vertex.glsl",
		"./shader/lens-flare-fragment.glsl",
		"lensFlare"
	);

	shader->loadShaderProgram(
		"./shader/hdr-vertex.glsl",
		"./shader/lens-flare-dirt-fragment.glsl",
		"lens-dirt"
	);

	shader->loadShaderProgram(
		"./shader/gbuffer-vertex.glsl",
		"./shader/gbuffer-fragment.glsl",
		"gBuffer"
	);

	shader->loadShaderProgram(
		"./shader/hdr-vertex.glsl",
		"./shader/ssao-fragment.glsl",
		"SSAO"
	);

	shader->loadShaderProgram(
		"./shader/hdr-vertex.glsl",
		"./shader/ssao-blur-fragment.glsl",
		"SSAOBlur"
	);
}

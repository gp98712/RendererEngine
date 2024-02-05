#include "ImguiController.h"

ImguiController* ImguiController::instance = nullptr;
/*
* Imgui SDL2 with OpenGL ���� ����
* 
* https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_opengl3/main.cpp
*/
ImguiController::ImguiController(SDL_Window* window, SDL_GLContext context) {
	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	// ��Ʈ��Ʋ��
	ImFont* font = io.Fonts->AddFontDefault();
	io.Fonts->Build();

	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 330"); // ���⼭ ������ GLSL ������ �����ϼ���.

}

void ImguiController::Update() {
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// �ӱ��� ����
	ImGui::Begin("Game Engine Controller");
	ImGui::Text("This is example");

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}


void ImguiController::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



ImguiController::~ImguiController() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
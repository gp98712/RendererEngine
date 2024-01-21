#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			break;
		}
	}
}

void Game::Setup() {
	playerPosition = glm::vec2(10.0, 20.0);
	playerVelocity = glm::vec2(60.0f, 60.0f);
}

void Game::Update()
{
	// �ʴ�����. 0.1 <- 0.1��
	float deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	millisecsPreviousFrame = SDL_GetTicks();

	playerPosition += playerVelocity * deltaTime;
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21,21,21,255);
	SDL_RenderClear(renderer);

	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstRect = {
		playerPosition.x,
		playerPosition.y,
		32,
		32
	};

	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_DestroyTexture(texture);

	// ���������� ������� ��������.
	SDL_RenderPresent(renderer);
}

Game::Game()
{
	isRunning = false;
	Logger::Log("Game constructor called!");
}

Game::~Game()
{
}

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING)!=0) {
		Logger::Err("Error initializing SDL.");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0,&displayMode);
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		Logger::Err("Error creating SDL window.");
		return;
	}

	renderer = SDL_CreateRenderer(
		window, -1, 0
	);

	if (!renderer) {
		Logger::Err("Error creating SDL renderer.");
		return;
	}


	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}

void Game::Run()
{
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

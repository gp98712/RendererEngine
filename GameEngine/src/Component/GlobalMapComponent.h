#pragma once
#include <vector>
#include <SDL.h>

class GlobalMapComponent {
private:
	static GlobalMapComponent* globalMapComponent;
	int width;
	int height;

	void GenerateMazeByBinaryAlgorithm();
	void GenerateTreePathBasedMaze();

public:
	static const int CELL_SIZE = 30;

	static GlobalMapComponent* GetInstance(int width, int height) {

		if (globalMapComponent == nullptr) {
			globalMapComponent = new GlobalMapComponent(width, height);
		}
		return globalMapComponent;
	}

	void RenderMaze(SDL_Renderer* renderer);

	GlobalMapComponent(int width, int height);

	// 3���� �ڷᱸ��
	// map[x][y][0] : ���ʺ�
	// map[x][y][1] : ����
	// map[x][y][2] : �����ʺ�
	// map[x][y][3] : �Ʒ���
	std::vector<std::vector<std::vector<int>>> map;
};
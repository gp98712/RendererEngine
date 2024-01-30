#pragma once
#include <vector>
#include <SDL.h>
#include <memory>
#include <queue>

/*
* ���� ������Ʈ�� ������������ Global ������Ʈ�� ���������� �ʴ�.
* ���� register�� ���ؼ� entity�� ����ϴ� ������ �ʿ����� �ʴ�.
* �׳� �̱������� �̿��Ѵ�.
* 
*/

struct Edge;

struct GraphNode {
	int x;
	int y;
	int GraphNodeIndex;
	std::vector<Edge> adjacentEdges;
};

struct Edge {
	std::shared_ptr<GraphNode> from;
	std::shared_ptr<GraphNode> to;
	float weight;

	Edge(std::shared_ptr<GraphNode> from, std::shared_ptr<GraphNode> to, float weight) {
		this->from = from;
		this->to = to;
		this->weight = weight;
	}
};

struct TraceGraph {
	std::shared_ptr<TraceGraph> parent;
	std::shared_ptr<GraphNode> current;
	std::shared_ptr<TraceGraph> childNode;
};

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
	~GlobalMapComponent();

	// 3���� �ڷᱸ��
	// map[x][y][0] : ���ʺ�
	// map[x][y][1] : ����
	// map[x][y][2] : �����ʺ�
	// map[x][y][3] : �Ʒ���
	std::vector<std::vector<std::vector<int>>> map;
	std::vector<std::vector<std::shared_ptr<GraphNode>>> mapGraph;
	void GetShortestPath(int startX, int startY, int endX, int endY);
	void RenderShortestPathStepByStep(SDL_Renderer* renderer);
	void GreedyBestFirstSearch(int startX, int startY, int endX, int endY);

	std::shared_ptr<TraceGraph> traceGraph;
};
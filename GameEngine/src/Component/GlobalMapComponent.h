#pragma once
#include <vector>
#include <SDL.h>
#include <memory>

/*
* ���� ������Ʈ�� ������������ Global ������Ʈ�� ���������� �ʴ�.
* ���� register�� ���ؼ� entity�� ����ϴ� ������ �ʿ����� �ʴ�.
* �׳� �̱������� �̿��Ѵ�.
* 
*/

struct GraphNode {
	int x;
	int y;
	int GraphNodeIndex;
	int weight = 1;
	std::vector<std::shared_ptr<GraphNode>> adjacentNodes;
};

struct TraceGraph {
	std::shared_ptr<TraceGraph> parent;
	std::shared_ptr<GraphNode> current;
	std::shared_ptr<TraceGraph> childNode;
};


struct WeightedEdge {
	struct WeightedGraphNode* from;
	struct WeightedGraphNode* to;
	float weight;
};

struct WeightedGraphNode {
	std::vector<WeightedEdge*> mEdges;
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
	// map[x][y][2] : �����ʺ�	std::vector<std::vector<GraphNode>> mapGraph;

	// map[x][y][3] : �Ʒ���
	std::vector<std::vector<std::vector<int>>> map;
	std::vector<std::vector<std::shared_ptr<GraphNode>>> mapGraph;
	std::shared_ptr<TraceGraph> traceGraph;
	void GetShortestPath(int startX, int startY, int endX, int endY);
	void RenderShortestPathStepByStep(SDL_Renderer* renderer);
};
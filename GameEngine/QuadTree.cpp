#include "QuadTree.h"
QuadTree::QuadTree(QuadTreeRectangle pBounds, int pLevel)
{
	level = pLevel;
	bounds = pBounds;
	objects = std::vector<QuadTreeRectangle>();
	nodes = std::vector<std::unique_ptr<QuadTree>>();
	nodes.resize(4);
}

QuadTree::~QuadTree()
{
	Clear();
}

void QuadTree::Clear() {
	objects.clear();
	for (auto& node : nodes) {
		node->Clear();
		node.reset();
	}

	nodes.clear();
}

void QuadTree::Split() {
	if (level >= MAX_LEVELS) {
		return;
	}

	int subWidth = (int)(bounds.width / 2);
	int subHeight = (int)(bounds.height / 2);
	int x = (int)bounds.x;
	int y = (int)bounds.y;

	nodes[0] = std::make_unique<QuadTree>(QuadTreeRectangle(x + subWidth, y, subWidth, subHeight),level+1);
	nodes[1] = std::make_unique<QuadTree>(QuadTreeRectangle(x, y, subWidth, subHeight), level + 1);
	nodes[2] = std::make_unique<QuadTree>(QuadTreeRectangle(x, y + subHeight, subWidth, subHeight), level + 1);
	nodes[3] = std::make_unique<QuadTree>(QuadTreeRectangle(x + subWidth, y + subHeight, subWidth, subHeight), level + 1);
}

int QuadTree::GetIndex(QuadTreeRectangle pRect) {
	/*
	* -1 : ���� QuadTree�� ���Ե��� ����. (�ڱ� �ڽ���, nodes���� ���ԵǾ����� ����)
	* 0 : top right
	* 1 : top left
	* 2 : bottom left
	* 3 : bottom right
	*/

	int index = -1;
	double verticalMidPoint = (double)bounds.x + (bounds.width / 2.0f);
	double horizontalMidPoint = (double)bounds.y + (bounds.height / 2.0f);

	// pRect.y - ���� ��ŭ �׾����� �߾� ���򼱺��� ���� �ִ���
	bool topQuadrant = 
		(pRect.y < horizontalMidPoint && 
		(pRect.y + pRect.height) < horizontalMidPoint);

	// pRect.y �� �߾� ���򼱺��� �Ʒ��� �ִ��� üũ.
	bool bottomQuadrant = (pRect.y > horizontalMidPoint);


	//	2 | 1
	//  -----
	//	3 | 4

	// pRect�� �߾� ���������� ���ʿ� �ִ°� üũ
	// 1,2,3,4 �и������� nodes�� 0���� �����̶� 0,1,2,3 ���� ǥ����
	if (pRect.x < verticalMidPoint && (pRect.x + pRect.width) < verticalMidPoint) {
		if (topQuadrant) {
			index = 1;
		}
		else if (bottomQuadrant) {
			index = 2;
		}
	}
	// �����ʿ� �ִ��� üũ
	else if (pRect.x > verticalMidPoint) {
		if (topQuadrant) {
			index = 0;
		}
		else if (bottomQuadrant) {
			index = 3;
		}
	}

	return index;
}

void QuadTree::Insert(QuadTreeRectangle pRect) {
	if (nodes[0] != nullptr) {
		int index = GetIndex(pRect);

		if (index != -1) {
			nodes[index]->Insert(pRect);
			return;
		}
	}

	objects.push_back(pRect);

	if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
		if (nodes[0] == nullptr) {
			Split();
		}

		int i = 0;
		while (i < objects.size()) {
			int index = GetIndex(objects[i]);
			if (index != -1) {
				nodes[index]->Insert(objects[i]);
				objects.erase(objects.begin() + i);
			}
			else {
				i++;
			}
		}
	}	
}

std::vector<QuadTreeRectangle>& QuadTree::Retrieve(std::vector<QuadTreeRectangle>& returnObjects, QuadTreeRectangle pRect) {
	int index = GetIndex(pRect);
	if (index != -1 && nodes[0] != nullptr) {
		nodes[index]->Retrieve(returnObjects, pRect);
	}

	returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());

	return returnObjects;
}
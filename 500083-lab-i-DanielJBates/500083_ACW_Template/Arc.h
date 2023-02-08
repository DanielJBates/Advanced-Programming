#pragma once

#include<string>

class Node;

class Arc
{

public:
	Arc(Node* pNodeA, Node* pNodeB, const std::string pTransportMode);
	~Arc();
	Node* GetNode(std::string pRequestesNode);
	std::string GetTransportMode();
	double GetDistance();

private:
	Node* mNodeA;
	Node* mNodeB;
	std::string mTransportMode;
	double mDistance;
};


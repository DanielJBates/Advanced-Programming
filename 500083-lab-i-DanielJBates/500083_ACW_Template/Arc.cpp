#include "Arc.h"
#include "Node.h"

Arc::Arc(Node* pNodeA, Node* pNodeB, const std::string pTransportMode)
{
	mNodeA = pNodeA;
	mNodeB = pNodeB;
	mTransportMode = pTransportMode;

	double dX = mNodeA->GetX() - mNodeB->GetX();
	dX = dX * dX;

	double dY = mNodeA->GetY() - mNodeB->GetY();
	dY = dY * dY;

	mDistance = sqrt(dX + dY);
}
Arc::~Arc()
{

};

Node* Arc::GetNode(std::string pRequestesNode)
{
	if (pRequestesNode == "A")
	{
		return mNodeA;
	}
	else if (pRequestesNode == "B")
	{
		return mNodeB;
	}
}
std::string Arc::GetTransportMode()
{
	return mTransportMode;
}
double Arc::GetDistance()
{
	return mDistance;
}
#include "Node.h"
#include "Arc.h"
#include "ACW_Wrapper.h"

Node::Node(const std::string pPlaceName, const int pReferenceCode, const double pLatitude, const double pLongitude)
{
	Node::mPlaceName = pPlaceName;
	Node::mReferenceCode = pReferenceCode;
	Node::mLatitude = pLatitude;
	Node::mLongitude = pLongitude;
	LLtoUTM(mLatitude, mLongitude, mX, mY);
}

Node::~Node()
{
}

int Node::GetReferenceCode()
{
	return mReferenceCode;
}
double Node::GetX()
{
	return mX;
}
double Node::GetY()
{
	return mY;
}
std::string Node::GetName()
{
	return mPlaceName;
}
std::vector<Arc*> Node::GetLinks()
{
	return mLinks;
}
void Node::AddLink(Arc* pLink)
{
	mLinks.push_back(pLink);
}

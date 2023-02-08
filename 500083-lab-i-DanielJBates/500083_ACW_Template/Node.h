#pragma once

#include<string>
#include<vector>

class Arc;

class Node
{

public:
	Node(const std::string pPlaceName, const int pReferenceCode, const double pLatitude, const double pLongitude);
	~Node();
	int GetReferenceCode();
	double GetX();
	double GetY();
	std::vector<Arc*> GetLinks();
	std::string GetName();
	void AddLink(Arc* pLink);

private:
	std::string mPlaceName;
	int mReferenceCode;
	double mLatitude, mLongitude, mX, mY;
	std::vector<Arc*> mLinks;
};


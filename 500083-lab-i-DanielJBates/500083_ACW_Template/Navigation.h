#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "Node.h"

class Navigation
{
	std::ofstream _outFile;
	std::vector<Node*> mPlaces;

public:
	Navigation();
	~Navigation();

	bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks);
	bool ProcessCommand(const std::string& commandString);
	bool IsTraversableBy(const std::string& pTransportMethod, const std::string& pLinkTransportMode);
	Node* SearchByReferenceCode(const int& pReferenceCode);

private:
	double CalculateDistance(const double pNodeA_X, const double pNodeA_Y, const double pNodeB_X, const double pNodeB_Y);

	void MaxDist();
	void MaxLink();
	void FindDist(const int& pReferenceCodeStart, const int& pReferenceCodeEnd);
	void FindNeighbour(const int& pReferenceCode);
	bool Check(const std::string& pTransportMode, const int& pReferenceCodeStart, const int& pReferenceCodeEnd);
	void FindRoute(const std::string& pTransportMode, const int& pReferenceCodeStart, const int& pReferenceCodeEnd);
	void FindShortestRoute(const std::string& pTransportMode, const int& pReferenceCodeStart, const int& pReferenceCodeEnd);
};
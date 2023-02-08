#include "Navigation.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Node.h"
#include "Arc.h"
#include "ACW_Wrapper.h"

Navigation::Navigation() : _outFile("Output.txt")
{
}

Navigation::~Navigation()
{
}

bool Navigation::ProcessCommand(const std::string& commandString)
{
	std::istringstream inString(commandString);
	std::string command;

	inString >> command;

	if (command == "MaxDist")
	{
		MaxDist();

		_outFile << std::endl;

		return true;
	}
	else if (command == "MaxLink")
	{
		MaxLink();

		_outFile << std::endl;

		return true;
	}
	else if (command == "FindDist")
	{
		int referenceCodeStart, referenceCodeEnd;

		inString >> referenceCodeStart >> referenceCodeEnd;

		FindDist(referenceCodeStart, referenceCodeEnd);

		_outFile << std::endl;

		return true;
	}
	else if (command == "FindNeighbour")
	{
		int referenceCode;

		inString >> referenceCode;

		FindNeighbour(referenceCode);

		_outFile << std::endl;

		return true;
	}
	else if (command == "Check")
	{
		int currentNode, nextNode;
		std::string transportMethod;

		inString >> transportMethod;
		inString >> currentNode;

		do
		{
			inString >> nextNode;

			if (currentNode == nextNode || !Check(transportMethod, currentNode, nextNode))
			{
				break;
			}
			else
			{
				currentNode = nextNode;
			}
		} while (!inString.fail());

		_outFile << std::endl;

		return true;
	}
	else if (command == "FindRoute")
	{
		std::string transportMode;
		int referenceCodeStart, referenceCodeEnd;

		inString >> transportMode >> referenceCodeStart >> referenceCodeEnd;

		FindRoute(transportMode, referenceCodeStart, referenceCodeEnd);

		_outFile << std::endl;

		return true;
	}
	else if (command == "FindShortestRoute")
	{
		std::string transportMode;
		int referenceCodeStart, referenceCodeEnd;

		inString >> transportMode >> referenceCodeStart >> referenceCodeEnd;

		FindShortestRoute(transportMode, referenceCodeStart, referenceCodeEnd);

		_outFile << std::endl;

		return true;
	}

	return false;
}

bool Navigation::BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks)
{
	std::fstream finPlaces(fileNamePlaces);
	std::fstream finLinks(fileNameLinks);

	if (finPlaces.fail() || finLinks.fail()) return false;

	if (finPlaces.is_open())
	{
		char line[255];

		while (finPlaces.getline(line, 255, ','))
		{
			std::string placeName;
			int referenceCode;
			float latitude, longitude;

			placeName = std::string(line);

			finPlaces.getline(line, 255, ',');
			std::istringstream inReferenceCode(line);
			inReferenceCode >> referenceCode;

			finPlaces.getline(line, 255, ',');
			std::istringstream inLatitude(line);
			inLatitude >> latitude;

			finPlaces.getline(line, 255);
			std::istringstream inLongitude(line);
			inLongitude >> longitude;

			Node* const ptr = new Node(placeName, referenceCode, latitude, longitude);
			Navigation::mPlaces.push_back(ptr);
		}
		finPlaces.close();
	}

	if (finLinks.is_open())
	{
		char line[255];

		while (finLinks.getline(line, 255, ','))
		{
			int referenceCodeA, referenceCodeB;
			std::string transportMode;

			std::istringstream inReferenceCodeStart(line);
			inReferenceCodeStart >> referenceCodeA;

			finLinks.getline(line, 255, ',');
			std::istringstream inReferenceCodeDestination(line);
			inReferenceCodeDestination >> referenceCodeB;

			finLinks.getline(line, 255);
			std::istringstream inTransport(line);
			inTransport >> transportMode;

			Node* tempNode = SearchByReferenceCode(referenceCodeA);
			Arc* ptr = new Arc(tempNode, SearchByReferenceCode(referenceCodeB), transportMode);
			tempNode->AddLink(ptr);

			tempNode = SearchByReferenceCode(referenceCodeB);
			ptr = new Arc(tempNode, SearchByReferenceCode(referenceCodeA), transportMode);
			tempNode->AddLink(ptr);
		}
		finLinks.close();
	}

	return true;
}

bool Navigation::IsTraversableBy(const std::string& pTransportMethod, const std::string& pLinkTransportMode)
{
	if (pTransportMethod == "Foot")
	{
		return true;
	}
	else if (pTransportMethod == "Bike")
	{
		if (pLinkTransportMode != "Foot")
		{
			return true;
		}
	}
	else if (pTransportMethod == "Car")
	{
		if (pLinkTransportMode != "Foot" || pLinkTransportMode != "Bike" || pLinkTransportMode != "Rail")
		{
			return true;
		}
	}
	else if (pTransportMethod == "Bus")
	{
		if (pLinkTransportMode == "Bus" || pLinkTransportMode == "Ship")
		{
			return true;
		}
	}
	else if (pTransportMethod == "Ship" || pTransportMethod == "Rail")
	{
		if (pLinkTransportMode == pTransportMethod)
		{
			return true;
		}
	}
	return false;

}

Node* Navigation::SearchByReferenceCode(const int& pReferenceCode)
{
	for (int i = 0; i < mPlaces.size(); i++)
	{
		if (mPlaces[i]->GetReferenceCode() == pReferenceCode)
		{
			return mPlaces[i];
		}
	}
	return nullptr;
}

double Navigation::CalculateDistance(const double pNodeA_X, const double pNodeA_Y, const double pNodeB_X, const double pNodeB_Y)
{
	const double distance = sqrt((((pNodeA_X - pNodeB_X) * (pNodeA_X - pNodeB_X)) + ((pNodeA_Y - pNodeB_Y) * (pNodeA_Y - pNodeB_Y))));
	return distance;
}

void Navigation::MaxDist()
{
	Node* a = nullptr;
	Node* b = nullptr;
	double distance = 0;

	for (int i = 0; i < mPlaces.size(); i++)
	{
		for (int j = 0; j < mPlaces.size(); j++)
		{
			if (distance < CalculateDistance(mPlaces[i]->GetX(), mPlaces[i]->GetY(), mPlaces[j]->GetX(), mPlaces[j]->GetY()))
			{
				distance = CalculateDistance(mPlaces[i]->GetX(), mPlaces[i]->GetY(), mPlaces[j]->GetX(), mPlaces[j]->GetY());
				a = mPlaces[i];
				b = mPlaces[j];
			}
		}
	}

	_outFile << a->GetName() << "," << b->GetName() << "," << distance << std::endl;
}
void Navigation::MaxLink()
{
	Arc* maxLink = nullptr;

	for (int i = 0; i < mPlaces.size(); i++)
	{
		for (int j = 0; j < mPlaces[i]->GetLinks().size(); j++)
		{
			if (maxLink == nullptr)
			{
				maxLink = mPlaces[i]->GetLinks()[j];
			}
			else if (mPlaces[i]->GetLinks()[j]->GetDistance() > maxLink->GetDistance())
			{
				maxLink = mPlaces[i]->GetLinks()[j];
			}
		}
	}

	_outFile << maxLink->GetNode("A")->GetReferenceCode() << "," << maxLink->GetNode("B")->GetReferenceCode() << "," << maxLink->GetDistance() << std::endl;
}
void Navigation::FindDist(const int& pReferenceCodeStart, const int& pReferenceCodeEnd)
{
	Node* const startNode = SearchByReferenceCode(pReferenceCodeStart);
	Node* const endNode = SearchByReferenceCode(pReferenceCodeEnd);

	_outFile << startNode->GetName() << "," << endNode->GetName() << "," << CalculateDistance(startNode->GetX(), startNode->GetY(), endNode->GetX(), endNode->GetY()) << std::endl;
}
void Navigation::FindNeighbour(const int& pReferenceCode)
{
	std::vector<Arc*> tempLinks = SearchByReferenceCode(pReferenceCode)->GetLinks();
	for (int i = 0; i < tempLinks.size(); i++)
	{
		_outFile << tempLinks[i]->GetNode("B")->GetReferenceCode() << std::endl;
	}
}
bool Navigation::Check(const std::string& pTransportMode, const int& pReferenceCodeA, const int& pReferenceCodeB)
{
	Node* const currentNode = SearchByReferenceCode(pReferenceCodeA);
	for (int i = 0; i < currentNode->GetLinks().size(); i++)
	{
		Arc* const currentLink = currentNode->GetLinks()[i];
		if (currentLink->GetNode("B")->GetReferenceCode() == pReferenceCodeB)
		{
			if (IsTraversableBy(pTransportMode, currentLink->GetTransportMode()))
			{
				_outFile << pReferenceCodeA << "," << pReferenceCodeB << ",PASS" << std::endl;
				return true;
			}
		}
	}
	_outFile << pReferenceCodeA << "," << pReferenceCodeB << ",FAIL" << std::endl;
	return false;
}
void Navigation::FindRoute(const std::string& pTransportMode, const int& pReferenceCodeStart, const int& pReferenceCodeEnd)
{
	Node* current = SearchByReferenceCode(pReferenceCodeStart);
	Node* next = nullptr;
	Node* const destination = SearchByReferenceCode(pReferenceCodeEnd);
	double distanceLeft = CalculateDistance(current->GetX(), current->GetY(), destination->GetX(), destination->GetY());
	std::vector<int> route;

	do
	{
		for (int i = 0; i < current->GetLinks().size(); i++)
		{
			Arc* const currentLink = current->GetLinks()[i];
			double const distanceFromNext = CalculateDistance(currentLink->GetNode("B")->GetX(), currentLink->GetNode("B")->GetY(), destination->GetX(), destination->GetY());
			if (IsTraversableBy(pTransportMode, currentLink->GetTransportMode()) && distanceLeft > distanceFromNext)
			{
				next = currentLink->GetNode("B");
				distanceLeft = distanceFromNext;
				break;
			}
		}

		if (next == nullptr)
		{
			_outFile << "FAIL" << std::endl;
			break;
		}

		route.push_back(current->GetReferenceCode());

		current = next;
		next = nullptr;

		if (current == destination)
		{
			route.push_back(current->GetReferenceCode());

			for (int i = 0; i < route.size(); i++)
			{
				_outFile << route[i] << std::endl;
			}
		}
	} while (current != destination);
}
void Navigation::FindShortestRoute(const std::string& pTransportMode, const int& pReferenceCodeStart, const int& pReferenceCodeEnd)
{
	Node* current = SearchByReferenceCode(pReferenceCodeStart);
	Node* next = nullptr;
	Node* const destination = SearchByReferenceCode(pReferenceCodeEnd);
	double distanceLeft = CalculateDistance(current->GetX(), current->GetY(), destination->GetX(), destination->GetY());
	std::vector<int> route;

	do
	{
		for (int i = 0; i < current->GetLinks().size(); i++)
		{
			Arc* const currentLink = current->GetLinks()[i];
			if (IsTraversableBy(pTransportMode, currentLink->GetTransportMode()))
			{			
				const double distanceFromNext = CalculateDistance(currentLink->GetNode("B")->GetX(), currentLink->GetNode("B")->GetY(), destination->GetX(), destination->GetY());
				if (distanceLeft > distanceFromNext)
				{
					next = currentLink->GetNode("B");
					distanceLeft = distanceFromNext;
				}
			}
		}

		if (next == nullptr)
		{
			_outFile << "FAIL" << std::endl;
			break;
		}

		route.push_back(current->GetReferenceCode());

		current = next;
		next = nullptr;

		if (current == destination)
		{
			route.push_back(current->GetReferenceCode());

			for (int i = 0; i < route.size(); i++)
			{
				_outFile << route[i] << std::endl;
			}
		}
	} while (current != destination);
}

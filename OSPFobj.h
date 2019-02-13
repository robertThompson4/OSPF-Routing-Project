#pragma once
#include<string>
#include<vector>
class OSPFobj
{
public:

	OSPFobj();
	~OSPFobj();
	void build();
private:
	struct node
	{
		long long valueBetween;
		float adjustedValue;
		float bestPathTotal;
		std::string bestPathLetter;
		bool visited;
	};
	long findSmallestPath(std::vector<std::vector<node>>& gridTwo, long holderOne, long startingNum, int numOfRout, std::vector<long>& numbersUsed, std::vector<float>& outputVec);
	std::string findPath(std::vector<std::vector<node>> gridTwo, long startingPoint, int numOfRout);

};


#include "OSPFobj.h"
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<stack>
#include<sstream>
#include<iomanip>

using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::endl;
using std::stringstream;
using std::to_string;
using std::fixed;
using std::setprecision;

OSPFobj::OSPFobj()
{

}

//This function builds the OSPF route. It goes through and builds the route with a 2D matrix of vectors.
//Things to note, each branch has a  large value between each node. When I put the input weights into the vector matrix
// I immediately adjusted them to make my life easier.
void OSPFobj::build()
{
	ifstream fin("ospf.in");
	ofstream fout("ospf.out");
	int numOfRout = 0;
	node sample;
	sample.valueBetween = 0;
	sample.adjustedValue = 0;
	sample.visited = false;
	sample.bestPathTotal = 0;
	sample.bestPathLetter = " ";
	fin >> numOfRout;
	while (numOfRout != 0)
	{
		vector<vector<node>> grid(numOfRout + 2, vector<node>(numOfRout + 2, sample));
		for (int r = 0; r < numOfRout + 2; r++)
		{
			for (int c = 0; c < numOfRout + 2; c++)
			{
				if (r == 0 || c == 0 || r == numOfRout + 1 || c == numOfRout + 1)
					grid[r][c].valueBetween = 0;
				else
				{
					fin >> grid[r][c].valueBetween;
					if (grid[r][c].valueBetween != 0) {
						grid[r][c].adjustedValue = (pow(10, 8) / grid[r][c].valueBetween);
						grid[r][c].bestPathTotal = grid[r][c].adjustedValue;
					}

				}
			}
		}
		int secondInput = 0;
		string outputString;
		fin >> secondInput;
		while (secondInput != 0)
		{
			outputString = findPath(grid, secondInput, numOfRout);
			fout << outputString << endl;
			fin >> secondInput;
		}

		fin >> numOfRout;
		if (numOfRout != 0)
			fout << "*****" << endl;
	}
}

long OSPFobj::findSmallestPath(vector<vector<node>>& gridTwo, long holderOne, long startingNum, int numOfRout, vector<long>& numbersUsed, vector<float>& outputVec)
{
	float holderTwo = 50000;
	long holderThree=0;
	long holderFour=0;

	//This for loop finds the smallest path between the available paths that don't connect and make a loop
	for (int r = 1; r < numOfRout + 1; r++)
	{
		if (r == numbersUsed[r])
		{
			for (int c = 1; c < numOfRout + 1; c++)
			{
				if (gridTwo[r][c].bestPathTotal != 0 && gridTwo[r][c].bestPathTotal <= holderTwo && gridTwo[r][c].visited == false)
				{
					if (numbersUsed[c] != c)
					{
						holderTwo = gridTwo[r][c].bestPathTotal;
						holderThree = r;
						holderFour = c;
					}
				}
			}
		}
	}


	//When checking whether a path has been used I already turned both [a][b] and [b][a] paths to true if they have been used. 
	//I did this because they are the same path coming from either a to b or from b to a.
	//Also this part of the function 
	if (gridTwo[holderThree][holderFour].visited == false)
	{
		if (holderFour == startingNum)
		{
			for (int c = 1; c < numOfRout + 1; c++)
				if (c != holderFour && gridTwo[holderThree][c].valueBetween != 0)
				{
					gridTwo[holderThree][c].bestPathTotal = gridTwo[holderThree][c].bestPathTotal + gridTwo[holderThree][holderFour].bestPathTotal;
					gridTwo[c][holderThree].bestPathTotal = gridTwo[c][holderThree].bestPathTotal + gridTwo[holderThree][holderFour].bestPathTotal;
				}
			outputVec[holderThree] = gridTwo[holderThree][holderFour].bestPathTotal;
			numbersUsed[holderThree] = holderThree;
		}
		else if (holderThree == startingNum)
		{
			for (int c = 1; c < numOfRout + 1; c++)
				if (c != holderThree && gridTwo[c][holderFour].valueBetween != 0)
				{
					gridTwo[c][holderFour].bestPathTotal = gridTwo[c][holderFour].bestPathTotal + gridTwo[holderThree][holderFour].bestPathTotal;
					gridTwo[holderFour][c].bestPathTotal = gridTwo[holderFour][c].bestPathTotal + gridTwo[holderThree][holderFour].bestPathTotal;
				}
			outputVec[holderFour] = gridTwo[holderThree][holderFour].bestPathTotal;
			numbersUsed[holderFour] = holderFour;

		}
		else
		{
			if (holderFour == numbersUsed[holderFour])
			{
				for (int c = 1; c < numOfRout + 1; c++)
					if (c != holderFour && gridTwo[holderThree][c].valueBetween != 0)
					{
						gridTwo[holderThree][c].bestPathTotal = gridTwo[holderThree][c].bestPathTotal + gridTwo[holderThree][holderFour].bestPathTotal;
						gridTwo[c][holderThree].bestPathTotal = gridTwo[c][holderThree].bestPathTotal + gridTwo[holderThree][holderFour].bestPathTotal;
					}
				outputVec[holderThree] = gridTwo[holderThree][holderFour].bestPathTotal;
				numbersUsed[holderThree] = holderThree;
			}
			else if (holderThree == numbersUsed[holderThree])
			{

				for (int c = 1; c < numOfRout + 1; c++)
					if (c != holderThree && gridTwo[c][holderFour].valueBetween != 0)
					{
						gridTwo[c][holderFour].bestPathTotal = gridTwo[c][holderFour].bestPathTotal + gridTwo[holderThree][holderFour].bestPathTotal;
						gridTwo[holderFour][c].bestPathTotal = gridTwo[holderFour][c].bestPathTotal + gridTwo[holderThree][holderFour].bestPathTotal;
					}
				outputVec[holderFour] = gridTwo[holderThree][holderFour].bestPathTotal;
				numbersUsed[holderFour] = holderFour;
			}
		}

		gridTwo[holderFour][holderThree].visited = true;
		gridTwo[holderThree][holderFour].visited = true;
	}
	return 1;
}


//This function takes in the starting point of routes, the number of routes
string OSPFobj::findPath(vector<vector<node>> gridTwo, long startingPoint, int numOfRout)
{
	string outputString = to_string(startingPoint) + ": ";
	long holderOne = startingPoint;
	long holderTwo = 0;
	int count = numOfRout - 1;
	vector<float> outputVec(numOfRout + 1, 500);
	vector<long> numbersUsed(numOfRout + 1, 500);
	numbersUsed[startingPoint] = startingPoint;
	while (count != 0)
	{
		holderOne = findSmallestPath(gridTwo, holderOne, startingPoint, numOfRout, numbersUsed, outputVec);
		count--;
	}
	count = 1;
	while (count != numOfRout + 1)
	{
		if (numbersUsed[count] != startingPoint)
		{
			stringstream stream;
			string holderSix;
			float holderFive;
			holderFive = outputVec[count];
			stream << fixed << setprecision(2) << holderFive;
			holderSix = stream.str();
			outputString = outputString + to_string(count) + "(" + holderSix + ")";
			if (count != numOfRout)
				outputString = outputString + " ";
		}
		count++;
	}
	return outputString;
}

OSPFobj::~OSPFobj()
{
}

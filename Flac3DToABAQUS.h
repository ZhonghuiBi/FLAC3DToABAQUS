#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class Flac3DToABAQUS
{
public:
	Flac3DToABAQUS(string inFileName, string outFileName);

	bool isNum(string str);

	//Import the file.f3grid
	void inputFlacData();

	//Export the file.inp
	void outputABAQUSData();

	~Flac3DToABAQUS();

private:
	/* The link of ABAQUS and FLAC3D */
	//Brick Element
	int m_Brick[8] = {0,1,4,2,3,6,7,5};
	//Wedge Element
	int m_Wedge[6] = {0,3,1,2,5,4};
	//Pyramid Element
	int m_Pyramid[8] = {0,1,4,2,3,3,3,3};
	//Tetrahedron Element
	int m_Tetrahedron[4] = {0,1,2,3};

	//Define input file name
	string m_flacFileName;
	//Define output file name
	string m_ABAQUSFileName;
	/************GRIDPOINTS************/
	//Node number
	vector<string> m_Node;
	//Node x coordinate
	vector<string> m_NodeXCoordinate;
	//Node y coordinate
	vector<string> m_NodeYCoordinate;
	//Node z coordinate
	vector<string> m_NodeZCoordinate;
	/**************ZONES***************/
	//Elements
	//8 nodes element
	vector<string> m_ElementNumB8;
	vector<string> m_ElementNodeNumB8;
	//7 nodes element
	vector<string> m_ElementNumM7;
	vector<string> m_ElementNodeNumM7;
	//6 nodes element
	vector<string> m_ElementNumW6;
	vector<string> m_ElementNodeNumW6;
	//5 nodes element
	vector<string> m_ElementNumP5;
	vector<string> m_ElementNodeNumP5;
	//4 nodes element
	vector<string> m_ElementNumT4;
	vector<string> m_ElementNodeNumT4;
	/**************GROUPS***************/
	vector<string> m_GroupNames;
	vector<string> m_GroupElements;  //Include string,such as ZGROUP
};















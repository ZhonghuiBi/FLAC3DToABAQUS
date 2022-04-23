#include <iostream>
#include <string>
#include "Flac3DToABAQUS.h"
using namespace std;


int main()
{
	string inFlacFileName = "DeDa_Tunnel.f3grid";
	string outABAQUSFileName = "DeDa_Tunnel.inp";
	Flac3DToABAQUS FA(inFlacFileName, outABAQUSFileName);

	system("pause");
	//exit(0);
	return 0;
}




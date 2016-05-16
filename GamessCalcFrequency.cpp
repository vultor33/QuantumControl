#include "GamessCalcFrequency.h"

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "Coordstructs.h"

using namespace std;

GamessCalcFrequency::~GamessCalcFrequency() {}

GamessCalcFrequency::GamessCalcFrequency()
{
	for (int naI = 0; naI < 9; naI++)
	{
		for (int liI = 1; liI < 10; liI++)
		{
			vector<CoordXYZ> mol = readCoordinates(naI, liI);

			//setando o vector<string> options
			// se for par - seta um
			// se for inpar - o outro
			// writeQuantum input e pronto.


		}
	}


}

vector<CoordXYZ> GamessCalcFrequency::readCoordinates(int naI, int liI)
{
	string xyzName;
	string naNumber, liNumber;
	stringstream convert;
	convert << naI << liI;
	convert >> naNumber >> liNumber;
	xyzName = "na" + naNumber + "li" + liNumber + ".xyz";

	ifstream readXyz_(xyzName.c_str());
	vector<CoordXYZ> mol;
	string dum1, dum2;
	readXyz_ >> dum1;
	readXyz_ >> dum2;
	for (int i = 0; i < naI + liI; i++)
	{
		readXyz_ >> mol[i].atomlabel
			>> mol[i].x
			>> mol[i].y
			>> mol[i].z;

		if (mol[i].atomlabel == "na")
			mol[i].atomlabel = "Na";
		if (mol[i].atomlabel == "li")
			mol[i].atomlabel = "Li";
	}
}





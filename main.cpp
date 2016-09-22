#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "ControlMopac.h"
#include "ReadQuantumOutput.h"
#include "Coordstructs.h"
#include "ReadQuantumOptions.h"
#include "WriteQuantumInput.h"
#include "GamessCalcFrequency.h"
#include "VultorParametrizeH.h"

using namespace std;

int main(int argc, char *argv[])
{
	string inputName;
	int naI, liI;
	string firstWord;
	if(argc > 1)
		firstWord = argv[1];
	if (firstWord == "readXYZ")
	{
		ReadQuantumOutput readOut_("gamess");
		string fileName = argv[2];
		readOut_.readOutput(fileName);
		ofstream print_((fileName + "--.xyz").c_str());
		vector<CoordXYZ> allAtoms = readOut_.getCoordinates();
		print_ << allAtoms.size() << endl << readOut_.getEnergy() << endl;
		for(size_t i = 0; i < allAtoms.size(); i++)
		{
			print_ << allAtoms[i].atomlabel << "  "
			<< allAtoms[i].x << "  "
			<< allAtoms[i].y << "  "
			<< allAtoms[i].z << "  " << endl;
		}
		print_.close();	

	}
	else
	{
		inputName = argv[1];
		stringstream convert1, convert2;
		convert1 << argv[2];
		convert2 << argv[3];
		convert1 >> naI;
		convert2 >> liI;
		GamessCalcFrequency gcalc_;
		gcalc_.runXyzToGamess("/home/vultor/gamess/rungms "," /scr/ "," 8 ", naI, liI, inputName);
	}

	return 0;
}





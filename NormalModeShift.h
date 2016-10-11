#ifndef NORMALMODESHIFT_H
#define NORMALMODESHIFT_H

#include <vector>
#include <string>
#include <fstream>
#include "Coordstructs.h"

class NormalModeShit
{
public:
	NormalModeShit();

	~NormalModeShit();

	std::vector<CoordXYZ> makeShift(std::string molFileName, std::string modeFileName, double multiplier);

	void printXyz(std::ofstream & print_, std::vector<CoordXYZ> allAtoms);

private:

	std::vector<CoordXYZ> readCoordinates(std::string xyzName);

	std::vector<CoordXYZ> readNormalMode(std::string modeFileName, int nAtoms);

	std::vector<CoordXYZ> addMode(std::vector<CoordXYZ> & mol, std::vector<CoordXYZ> & mode, double multiplier);

	/*PRINT
	string fileName = "teste";
	ofstream print_((fileName + "--.xyz").c_str());
	printXyz(print_, mol);
	for (int i = 0; i < 20; i++)
	{
		double mult = i * 0.3;
		vector<CoordXYZ> x1 = addMode(mol, mode3, mult);
		printXyz(print_, x1);
	}
	print_.close();*/
};



#endif
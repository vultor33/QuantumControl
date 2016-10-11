#include "NormalModeShift.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Coordstructs.h"

using namespace std;

NormalModeShit::NormalModeShit(){}

NormalModeShit::~NormalModeShit(){}

vector<CoordXYZ> NormalModeShit::makeShift(string molFileName, string modeFileName, double multiplier)
{
	vector<CoordXYZ> mol = readCoordinates((molFileName+".xyz").c_str());
	vector<CoordXYZ> mode1 = readNormalMode((modeFileName + ".txt").c_str(), mol.size());
	return addMode(mol, mode1, multiplier);
}

vector<CoordXYZ> NormalModeShit::readCoordinates(string xyzName)
{
	ifstream readXyz_(xyzName.c_str());
	string auxline;
	getline(readXyz_, auxline);
	int natoms;
	stringstream line0;
	line0 << auxline;
	line0 >> natoms;
	vector<CoordXYZ> mol(natoms);
	getline(readXyz_, auxline);
	for (int i = 0; i < natoms; i++)
	{
		getline(readXyz_, auxline);
		stringstream line;
		line << auxline;
		line >> mol[i].atomlabel
			>> mol[i].x
			>> mol[i].y
			>> mol[i].z;
	}
	readXyz_.close();
	return mol;
}

vector<CoordXYZ> NormalModeShit::readNormalMode(string modeFileName, int nAtoms)
{
	vector<CoordXYZ> mode(nAtoms);
	ifstream readMode_(modeFileName.c_str());
	string auxline;
	for (int i = 0; i < nAtoms; i++)
	{
		getline(readMode_, auxline);
		stringstream line1;
		line1 << auxline;
		line1 >> mode[i].x;

		getline(readMode_, auxline);
		stringstream line2;
		line2 << auxline;
		line2 >> mode[i].y;

		getline(readMode_, auxline);
		stringstream line3;
		line3 << auxline;
		line3 >> mode[i].z;
	}
	readMode_.close();
	return mode;
}

void NormalModeShit::printXyz(ofstream & print_, vector<CoordXYZ> allAtoms)
{
	print_ << allAtoms.size() << endl << "title" << endl;
	for (size_t i = 0; i < allAtoms.size(); i++)
	{
		print_ << allAtoms[i].atomlabel << "  "
			<< allAtoms[i].x << "  "
			<< allAtoms[i].y << "  "
			<< allAtoms[i].z << "  " << endl;
	}
}

vector<CoordXYZ> NormalModeShit::addMode(vector<CoordXYZ> & mol, vector<CoordXYZ> & mode, double multiplier)
{
	vector<CoordXYZ> molMode = mol;
	for (size_t i = 0; i < mol.size(); i++)
	{
		molMode[i].x += mode[i].x * multiplier;
		molMode[i].y += mode[i].y * multiplier;
		molMode[i].z += mode[i].z * multiplier;
	}
	return molMode;
}

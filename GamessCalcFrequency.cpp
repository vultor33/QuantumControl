#include "GamessCalcFrequency.h"

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

#include "Coordstructs.h"
#include "WriteQuantumInput.h"
#include "ReadQuantumOutput.h"

using namespace std;

GamessCalcFrequency::~GamessCalcFrequency() {}

GamessCalcFrequency::GamessCalcFrequency()
{
	for (int naI = 0; naI < 10; naI++)
	{
		for (int liI = 1; liI < 10; liI++)
		{
			if ((naI + liI < 5) || (naI + liI > 10))
				continue;

			vector<CoordXYZ> mol = readCoordinates(naI, liI);
			vector<string> optionsOtim;
			vector<string> optionsFreq;

			if (((naI + liI) % 2) == 0)
			{
				optionsOtim = setGamessOptions(0);
				optionsFreq = setGamessOptions(2);
			}
			else
			{
				optionsOtim = setGamessOptions(1);
				optionsFreq = setGamessOptions(3);
			}

			for (int i = 0; i < naI; i++)
			{
				optionsOtim.push_back("auxFiles/na-base.txt");
				optionsFreq.push_back("auxFiles/na-base.txt");
			}
			for (int i = 0; i < liI; i++)
			{
				optionsOtim.push_back("auxFiles/li-base.txt");
				optionsFreq.push_back("auxFiles/li-base.txt");
			}
			optionsOtim.push_back("EndOfBasis");
			optionsFreq.push_back("EndOfBasis");
			optionsOtim.push_back("ActivateEcp");
			optionsFreq.push_back("ActivateEcp");
			for (int i = 0; i < naI; i++)
			{
				optionsOtim.push_back("auxFiles/na-ecp.txt");
				optionsFreq.push_back("auxFiles/na-ecp.txt");
			}
			for (int i = 0; i < liI; i++)
			{
				optionsOtim.push_back("auxFiles/li-ecp.txt");
				optionsFreq.push_back("auxFiles/li-ecp.txt");
			}
			optionsOtim.push_back("EndOfEcp");
			optionsFreq.push_back("EndOfEcp");

			WriteQuantumInput writeInput_(optionsOtim);
			string inputName = writeInput_.createInput(mol);

			//system(roda name)
			// e assim vai
			// read, roda de novo.
			// frequency
		}
	}
}

vector<CoordXYZ> GamessCalcFrequency::readCoordinates(int naI, int liI)
{
	string xyzName;
	string naNumber, liNumber;
	stringstream convert;
	convert << naI << "  " << liI;
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

	return mol;
}

void GamessCalcFrequency::getLowestEnergyFromXyzFile(int naI, int liI)
{
	double en1, en2, en3;
	en1 = readXyzEnergy("coord-3968/", naI, liI);
	en2 = readXyzEnergy("coord-13/", naI, liI);
	en3 = readXyzEnergy("coord-4509/", naI, liI);
	cout << "n:  " << naI << "  " << liI << endl;
	cout << "en1:  " << en1 << "  en2:  " << en2 << "  en3:  " << en3 << endl;

	string xyzName, copyName;
	string naNumber, liNumber;
	stringstream convert;
	convert << naI << "  " << liI;
	convert >> naNumber >> liNumber;
	string path;

	if (en1 < en2)
	{
		if (en1 < en3)
		{
			path = "coord-3968/";
		}
		else
		{
			path = "coord-4509/";
		}
	}
	else
	{
		if (en2 < en3)
		{
			path = "coord-13/";
		}
		else
		{
			path = "coord-4509/";
		}
	}

	xyzName = path + "na" + naNumber + "li" + liNumber + ".xyz";
	copyName = "na" + naNumber + "li" + liNumber + ".xyz";
	copyFile(xyzName, copyName);
}


double GamessCalcFrequency::readXyzEnergy(string path, int naI, int liI)
{

	string xyzName;
	string naNumber, liNumber;
	stringstream convert;
	convert << naI << "  " << liI;
	convert >> naNumber >> liNumber;
	xyzName = path + "na" + naNumber + "li" + liNumber + ".xyz";

	ifstream readXyz_(xyzName.c_str());
	string auxline;
	getline(readXyz_, auxline);
	getline(readXyz_, auxline);
	auxline = auxline.substr(3, auxline.length() - 5);

	stringstream convert2;
	convert2 << auxline;
	double energy2;
	convert2 >> energy2;
	return energy2;
}


void GamessCalcFrequency::copyFile(string from, string to)
{
	ifstream source(from, ios::binary);
	ofstream dest(to, ios::binary);

	istreambuf_iterator<char> begin_source(source);
	istreambuf_iterator<char> end_source;
	ostreambuf_iterator<char> begin_dest(dest);
	copy(begin_source, end_source, begin_dest);

	source.close();
	dest.close();
}

vector<string> GamessCalcFrequency::setGamessOptions(int nOpt)
{
	vector<string> options(11);
	options[0] = "gamess";
	options[1] = "";
	options[3] = " ISPHER=1 COORD=UNIQUE NOSYM=1 UNITS=ANGS PP=READ $END";
	options[4] = " $GUESS GUESS=HUCKEL $END";
	options[5] = " $SYSTEM MWORDS=40 MEMDDI=20  $END";
	options[6] = " $SCF DIRSCF=.FALSE. $END";
	options[7] = " $DATA";
	options[8] = " titulo";
	options[9] = "C1";
	options[10] = "EndOfHeader";

	if (nOpt == 0)
		options[2] = " $CONTRL SCFTYP=RHF RUNTYP=OPTIMIZE EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=1";
	else if(nOpt == 1)
		options[2] = " $CONTRL SCFTYP=ROHF RUNTYP=OPTIMIZE EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=2";
	else if(nOpt == 2)
		options[2] = " $CONTRL SCFTYP=RHF RUNTYP=HESSIAN EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=1";
	else if(nOpt == 3)
		options[2] = " $CONTRL SCFTYP=ROHF RUNTYP=HESSIAN EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=2";

	return options;
}

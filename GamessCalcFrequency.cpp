#include "GamessCalcFrequency.h"

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>

#include "Coordstructs.h"
#include "WriteQuantumInput.h"
#include "ReadQuantumOutput.h"

using namespace std;

GamessCalcFrequency::~GamessCalcFrequency() {}

GamessCalcFrequency::GamessCalcFrequency(string gamessPath, string scrPath, string nProc)
{
	ofstream freqFile_("frequenciesFiles.txt");
	string removeScr = "rm  " + scrPath;
	bool ecpFlag;

	for (int naI = 2; naI < 10; naI++)
	{
		for (int liI = 1; liI < 10; liI++)
		{
			if ((naI + liI < 5) || (naI + liI > 10))
				continue;

			ecpFlag = (naI != 0);
			vector<CoordXYZ> mol = readCoordinates(naI, liI);
			vector<string> optionsOtim;
			vector<string> optionsFreq;

			if (((naI + liI) % 2) == 0)
			{
				optionsOtim = setGamessOptions(0,ecpFlag);
				optionsFreq = setGamessOptions(2,ecpFlag);
			}
			else
			{
				optionsOtim = setGamessOptions(1,ecpFlag);
				optionsFreq = setGamessOptions(3,ecpFlag);
			}

			for (int i = 0; i < naI; i++)
			{
				optionsOtim.push_back("na-base.txt");
				optionsFreq.push_back("na-base.txt");
			}
			for (int i = 0; i < liI; i++)
			{
				optionsOtim.push_back("li-base.txt");
				optionsFreq.push_back("li-base.txt");
			}
			optionsOtim.push_back("EndOfBasis");
			optionsFreq.push_back("EndOfBasis");
			optionsOtim.push_back("ActivateEcp");
			optionsFreq.push_back("ActivateEcp");
			for (int i = 0; i < naI; i++)
			{
				optionsOtim.push_back("na-ecp.txt");
				optionsFreq.push_back("na-ecp.txt");
			}
			for (int i = 0; i < liI; i++)
			{
				optionsOtim.push_back("li-ecp.txt");
				optionsFreq.push_back("li-ecp.txt");
			}
			optionsOtim.push_back("EndOfEcp");
			optionsFreq.push_back("EndOfEcp");

			string projectName;
			string naNumber, liNumber;
			stringstream convert;
			convert << naI << "  " << liI;
			convert >> naNumber >> liNumber;
			projectName = "na-" + naNumber + "-li-" + liNumber + "-ver-";
			optionsOtim[1] = projectName;
			optionsFreq[1] = projectName;

			WriteQuantumInput writeInput_(optionsOtim);
			string inputName = writeInput_.createInput(mol);
			if(inputName == "") continue;
			system((removeScr + inputName + ".dat").c_str());
			system((gamessPath + inputName + ".inp" + " 00 " + nProc + "  >  " + inputName + ".out").c_str());
			ReadQuantumOutput readO1_("gamess");
			readO1_.readOutput(inputName);
			vector<CoordXYZ> mol1 = readO1_.getCoordinates();

			WriteQuantumInput writeInput2_(optionsOtim);
			string inputName2 = writeInput2_.createInput(mol1, 1);
			if(inputName2 == "") continue;
			system((removeScr + inputName2 + ".dat").c_str());
			system((gamessPath + inputName2 + ".inp" + " 00 " + nProc + "  >  " + inputName2 + ".out").c_str());
			ReadQuantumOutput readO2_("gamess");
			readO2_.readOutput(inputName2);
			vector<CoordXYZ> mol2 = readO2_.getCoordinates();

			WriteQuantumInput writeInput3_(optionsFreq);
			string inputName3 = writeInput3_.createInput(mol2, 2);
			if(inputName3 == "") continue;
			system((removeScr + inputName3 + ".dat").c_str());
			system((gamessPath + inputName3 + ".inp" + " 00 " + nProc + "  >  " + inputName3 + ".out").c_str());
			ReadQuantumOutput readO3_("gamess");
			readO3_.readOutput(inputName3);
			double freq = readO3_.getFirstFrequency();

			freqFile_ << "na:  " << naI << "  li:  " << liI << "  freq:  " << freq << endl;
		}
	}
	freqFile_.close();
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
	vector<CoordXYZ> mol(naI + liI);
	string auxline;
	getline(readXyz_,auxline);
	getline(readXyz_,auxline);
	for (int i = 0; i < naI + liI; i++)
	{
		getline(readXyz_,auxline);
		stringstream line;
		line << auxline;
		line >> mol[i].atomlabel
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


void GamessCalcFrequency::getLowestEnergyFromXyzFile(std::vector<std::string> & filenames, std::string lowEnergyFileName, std::ofstream & allEnergies)
{
	vector<double> all;
	size_t size = filenames.size();
	for(size_t i = 0; i < size; i++)
	{
		double energyTemp = readXyzEnergy(filenames[i]);
		all.push_back(energyTemp);
	}
	int low;
	if(size == 1)
		low = 0;
	else
	{
		low = 0;
		double lowestEnergy = all[low];
		for(size_t i = 1; i < size; i++) 
		{
			if(lowestEnergy > all[i])
			{
				low = i;
				lowestEnergy = all[i];
			}
		}
	}	
	copyFile(filenames[low], lowEnergyFileName);

	allEnergies << lowEnergyFileName << "    ";
	if(allEnergies.is_open())
	{
		for(size_t i = 0; i < size; i++)
		{
			allEnergies << setprecision(8) << all[i] << "             ";
		}
	}
	allEnergies << endl;

}

/*
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
*/


double GamessCalcFrequency::readXyzEnergy(string name)
{
	ifstream readXyz_(name.c_str());
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

vector<string> GamessCalcFrequency::setGamessOptions(int nOpt, bool ecpFlag)
{
	vector<string> options(11);
	options[0] = "gamess";
	options[1] = "";
	options[4] = " $GUESS GUESS=HUCKEL $END";
	options[5] = " $SYSTEM MWORDS=40 MEMDDI=20  $END";
	options[6] = " $SCF DIRSCF=.FALSE. $END";
	options[7] = " $DATA";
	options[8] = " titulo";
	options[9] = "C1";
	options[10] = "EndOfHeader";

	if(ecpFlag)
		options[3] = " ISPHER=1 COORD=UNIQUE NOSYM=1 UNITS=ANGS PP=READ $END";
	else
		options[3] = " ISPHER=1 COORD=UNIQUE NOSYM=1 UNITS=ANGS $END";

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

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

GamessCalcFrequency::GamessCalcFrequency() {}

void GamessCalcFrequency::runXyzToGamess(string gamessPath, string scrPath, string nProc, int naI, int liI, string projectName)
{
	vector<int> structuresDone;
	vector<string> calcOptions = getFreqOptionsFromFile(structuresDone);

	ofstream freqFile_("frequenciesFiles.txt");
	string removeScr = "rm  " + scrPath;
	bool ecpFlag;

	ecpFlag = !((naI == 0) || (calcOptions[0] == "noecp"));
	vector<CoordXYZ> mol = readCoordinates(naI, liI, projectName);
	vector<string> optionsOtim;
	vector<string> optionsFreq;
	if (((naI + liI) % 2) == 0)
	{
		optionsOtim = setGamessOptions(0,ecpFlag,calcOptions);
		optionsFreq = setGamessOptions(2,ecpFlag,calcOptions);
	}
	else
	{
		optionsOtim = setGamessOptions(1,ecpFlag,calcOptions);
		optionsFreq = setGamessOptions(3,ecpFlag,calcOptions);
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
	optionsOtim[1] = projectName;
	optionsFreq[1] = projectName;
	WriteQuantumInput writeInput_(optionsOtim);
	string inputName = writeInput_.createInput(mol);
	if(inputName == "") return;
	system((removeScr + inputName + ".dat").c_str());
	system((gamessPath + inputName + ".inp" + " 00 " + nProc + "  >  " + inputName + ".out").c_str());
	ReadQuantumOutput readO1_("gamess");
	readO1_.readOutput(inputName);
	vector<CoordXYZ> mol1 = readO1_.getCoordinates();
	WriteQuantumInput writeInput2_(optionsOtim);
	string inputName2 = writeInput2_.createInput(mol1, 1);
	if(inputName2 == "") return;
	system((removeScr + inputName2 + ".dat").c_str());
	system((gamessPath + inputName2 + ".inp" + " 00 " + nProc + "  >  " + inputName2 + ".out").c_str());
	ReadQuantumOutput readO2_("gamess");
	readO2_.readOutput(inputName2);
	vector<CoordXYZ> mol2 = readO2_.getCoordinates();
	WriteQuantumInput writeInput3_(optionsFreq);
	string inputName3 = writeInput3_.createInput(mol2, 2);
	if(inputName3 == "") return;
	system((removeScr + inputName3 + ".dat").c_str());
	system((gamessPath + inputName3 + ".inp" + " 00 " + nProc + "  >  " + inputName3 + ".out").c_str());
	ReadQuantumOutput readO3_("gamess");
	readO3_.readOutput(inputName3);
	double freq = readO3_.getFirstFrequency();
	freqFile_ << "na:  " << naI << "  li:  " << liI << "  freq:  " << freq << endl;
	freqFile_.close();
}

vector<CoordXYZ> GamessCalcFrequency::readCoordinates(int naI, int liI, string xyzName)
{
	//string xyzName;
	//string naNumber, liNumber;
	//stringstream convert;
	//convert << naI << "  " << liI;
	//convert >> naNumber >> liNumber;

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

vector<string> GamessCalcFrequency::getFreqOptionsFromFile(vector<int> & structuresDone)
{
        ifstream freqOptions_("calcFreqOptions.txt");
        vector<string> calcOptions;
	int n1, n2;
        string auxline;
	while(getline(freqOptions_,auxline))
	{
		stringstream convert;
		convert << auxline;
		string first;
		convert >> first;
		if(first == "end")
			break;
		else if((first == "mp2")||(first=="dft"))
			calcOptions.push_back(first);
		else if((first == "noecp")||(first == "ecp"))
			calcOptions.push_back(first);
		else if(first == "$CONTRL")
			calcOptions.push_back(auxline);
		else
		{
			stringstream convertNumber;
			convertNumber << auxline;
			convertNumber >> n1 >> n2;
			structuresDone.push_back(n1);
			structuresDone.push_back(n2);
		}
	}
        freqOptions_.close();
	return calcOptions;
}



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

vector<string> GamessCalcFrequency::setGamessOptions(int nOpt, bool ecpFlag, vector<string> & calcOptions)
{
	vector<string> options(7);
	options[0] = "gamess";
	options[1] = "";
	options[4] = " $GUESS GUESS=HUCKEL $END";
	options[5] = " $SYSTEM MWORDS=40 MEMDDI=20  $END";
	options[6] = " $SCF DIRSCF=.FALSE. $END";
	if(calcOptions[1] != "mp2")
		options.push_back(" $FORCE METHOD=SEMINUM $END");
	options.push_back(" $DATA");
	options.push_back(" titulo");
	options.push_back("C1");
	options.push_back("EndOfHeader");

	if(ecpFlag)
		options[3] = " ISPHER=1 COORD=UNIQUE NOSYM=1 UNITS=ANGS PP=READ $END";
	else
		options[3] = " ISPHER=1 COORD=UNIQUE NOSYM=1 UNITS=ANGS $END";

	if (nOpt == 0)
		options[2] = calcOptions[2];// " $CONTRL SCFTYP=RHF RUNTYP=OPTIMIZE EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=1";
	else if(nOpt == 1)
		options[2] = calcOptions[3];// " $CONTRL SCFTYP=ROHF RUNTYP=OPTIMIZE EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=2";
	else if(nOpt == 2)
		options[2] = calcOptions[4];//" $CONTRL SCFTYP=RHF RUNTYP=HESSIAN EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=1";
	else if(nOpt == 3)
		options[2] = calcOptions[5];//" $CONTRL SCFTYP=ROHF RUNTYP=HESSIAN EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=2";

	return options;
}





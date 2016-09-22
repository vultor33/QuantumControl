#ifndef GAMESSCALCFREQUENCY_H
#define GAMESSCALCFREQUENCY_H

#include <vector>
#include <string>
#include <fstream>
#include "Coordstructs.h"

class GamessCalcFrequency
{
public:
	~GamessCalcFrequency();
	
	GamessCalcFrequency(std::string gamessPath, std::string scrPath, std::string nProc);
	
	GamessCalcFrequency();
	
	void getLowestEnergyFromXyzFile(std::vector<std::string> & filenames, std::string lowEnergyFileName, std::ofstream & allEnergies);
	// ofstream need header and - allEnergies.open("xxx.xxx", std::ios_base::app);

	void runXyzToGamess(std::string gamessPath, std::string scrPath, std::string nProc, int naI, int liI, std::string projectName);

private:
	std::vector<CoordXYZ> readCoordinates(int naI, int liI);

	double readXyzEnergy(std::string name);

	void copyFile(std::string from, std::string to);

	std::vector<std::string> setGamessOptions(int nOpt, bool ecpFlag, std::vector<std::string> & calcOptions);

	std::vector<std::string> getFreqOptionsFromFile(std::vector<int> & structuresDone);

};


#endif

/*
EXAMPLE ON MAIN
	ofstream of_;
	of_.open("allEnergies.txt", std::ios_base::app);
	of_ << "name           coord-13               coord-3968            coord-4509 " << endl;
	GamessCalcFrequency gqf_;
	vector<string> files(3);
	string clusterName, path1,path2,path3;
	path1 = "coord-13/";
	path2 = "coord-4509/";
	path3 = "coord-4509/";
	
	clusterName = "na0li3.xyz";
	files[0] = path1 + clusterName;
	files[1] = path2 + clusterName;
	files[2] = path3 + clusterName;
	gqf_.getLowestEnergyFromXyzFile(files,clusterName,of_);
*/




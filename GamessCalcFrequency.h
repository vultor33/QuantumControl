#ifndef GAMESSCALCFREQUENCY_H
#define GAMESSCALCFREQUENCY_H

#include <vector>
#include <string>
#include "Coordstructs.h"

class GamessCalcFrequency
{
public:
	GamessCalcFrequency(std::string gamessPath, std::string scrPath, std::string nProc);
	
	~GamessCalcFrequency();

private:
	std::vector<CoordXYZ> readCoordinates(int naI, int liI);

	void getLowestEnergyFromXyzFile(int naI, int liI);

	double readXyzEnergy(std::string path, int naI, int liI);

	void copyFile(std::string from, std::string to);

	std::vector<std::string> setGamessOptions(int nOpt, bool ecpFlag);

};


#endif

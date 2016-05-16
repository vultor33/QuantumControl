#ifndef GAMESSCALCFREQUENCY_H
#define GAMESSCALCFREQUENCY_H

#include <vector>
#include "Coordstructs.h"

class GamessCalcFrequency
{
public:
	GamessCalcFrequency();
	
	~GamessCalcFrequency();

private:
	std::vector<CoordXYZ> readCoordinates(int naI, int liI);

};


#endif

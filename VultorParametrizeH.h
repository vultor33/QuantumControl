#ifndef VULTORPARAMETRIZEH_H
#define VULTORPARAMETRIZEH_H

#include <vector>
#include "Coordstructs.h"

class VultorParametrizeH
{
public:
	VultorParametrizeH();
	~VultorParametrizeH();

	double getError(std::vector<MopacParams> &params);


};

#endif
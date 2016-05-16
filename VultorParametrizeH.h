#ifndef VULTORPARAMETRIZEH_H
#define VULTORPARAMETRIZEH_H

#include <vector>

class VultorParametrizeH
{
public:
	VultorParametrizeH();
	~VultorParametrizeH();

	double getError(std::vector<double> &params);


};

#endif
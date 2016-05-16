#ifndef CONTROLMOPAC_H
#define CONTROLMOPAC_H

#include <vector>
#include <string>

#include "Coordstructs.h"
#include "ReadQuantumOutput.h"

#include <vector>
#include <string>

class ControlMopac
{
public:
	ControlMopac();
	~ControlMopac();

	bool optimize(
		std::vector<CoordXYZ> & allAtoms,
		std::vector<std::string> & options,
		std::vector<double> &params);

	double getHydrogenError(
		std::vector<CoordXYZ> & allAtoms,
		std::vector<std::string> & options,
		std::vector<double> &params,
		int model);


private:

	void createParamsFile(std::string paramsName, std::vector<double> &params);

	std::string mopacExecPath = "C:/\"Program Files\"/MOPAC/MOPAC2016.exe  ";

};

#endif


#include "ControlMopac.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

#include "Coordstructs.h"
#include "ReadQuantumOutput.h"
#include "WriteQuantumInput.h"
#include "CalculateError.h"

using namespace std;

ControlMopac::ControlMopac() {}

ControlMopac::~ControlMopac(){}

bool ControlMopac::optimize(
	vector<CoordXYZ> & allAtoms,
	vector<string> & options,
	vector<MopacParams> & params)
{

	ReadQuantumOutput readmop_("mopac");

	WriteQuantumInput writeMop_(options);

	string inputName = writeMop_.createInput(allAtoms);

	createParamsFile(options[3], params);

	system((mopacExecPath + inputName + ".mop").c_str());

	ReadQuantumOutput readGamess_(options[0]);

	readmop_.readOutput(inputName);

	vector<CoordXYZ> optimizedAtoms = readmop_.getCoordinates();

	if (optimizedAtoms.size() == 0) return false;

	return true;
}




double ControlMopac::getHydrogenError(
	vector<CoordXYZ> & allAtoms,
	vector<string> & options,
	vector<MopacParams> & params,
	int model)
{
	ReadQuantumOutput readmop_("mopac");

	WriteQuantumInput writeMop_(options);

	string inputName = writeMop_.createInput(allAtoms);

	createParamsFile("hparams", params);

	system((mopacExecPath + inputName + ".mop").c_str());

	ReadQuantumOutput readGamess_(options[0]);

	readmop_.readOutput(inputName);

	vector<CoordXYZ> optimizedAtoms = readmop_.getCoordinates();

	if (optimizedAtoms.size() == 0) return 1.0e99;

	CalculateError calcError_;

	size_t natoms = optimizedAtoms.size();
	vector<double> x(3 * natoms);
	for (size_t i = 0; i < natoms; i++)
	{
		x[i] = optimizedAtoms[i].x;
		x[i + natoms] = optimizedAtoms[i].y;
		x[i + 2 * natoms] = optimizedAtoms[i].z;
	}

	double evHartree = 0.0367502e0;

	vector<double> dipole = readmop_.getDipole();

	double error = calcError_.calculate(model, x, dipole, readmop_.getEnergy() * evHartree, readmop_.getIonizationPotential() * evHartree);

	return error;
}

void ControlMopac::createParamsFile(string paramsName, vector<MopacParams>& params)
{
	string name = paramsName + ".inp";
	remove(name.c_str());
	ofstream paramsFile_(name.c_str());

	for (size_t i = 0; i < params.size(); i++)
		paramsFile_ << params[i].paramName << "  " << params[i].paramValue << endl;

	paramsFile_ << "  END     " << endl;
	paramsFile_.close();
}






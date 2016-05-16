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
	vector<double> & params)
{
	ReadQuantumOutput readmop_("mopac");

	WriteQuantumInput writeMop_(options);

	string inputName = writeMop_.createInput(allAtoms);

	createParamsFile("hparams", params);

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
	vector<double> & params,
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

	double error = calcError_.calculate(model, x, readmop_.getDipole(), readmop_.getEnergy() * evHartree, readmop_.getIonizationPotential() * evHartree);

	return error;
}




void ControlMopac::createParamsFile(string paramsName, vector<double>& params)
{
	string name = paramsName + ".inp";
	remove(name.c_str());
	ofstream paramsFile_(name.c_str());

	paramsFile_ << "USS    H  " << params[0] << endl;
	paramsFile_ << "ZS     H  " << params[1] << endl;
	paramsFile_ << "BETAS  H  " << params[2] << endl;
	paramsFile_ << "ALP    H  " << params[3] << endl;
	paramsFile_ << "GSS    H  " << params[4] << endl;
	paramsFile_ << "FN11   H  " << params[5] << endl;
	paramsFile_ << "FN21   H  " << params[6] << endl;
	paramsFile_ << "FN31   H  " << params[7] << endl;
	paramsFile_ << "FN12   H  " << 0.0e0 << endl;
	paramsFile_ << "FN22   H  " << 0.0e0 << endl;
	paramsFile_ << "FN32   H  " << 0.0e0 << endl;
	paramsFile_ << "FN13   H  " << 0.0e0 << endl;
	paramsFile_ << "FN23   H  " << 0.0e0 << endl;
	paramsFile_ << "FN33   H  " << 0.0e0 << endl;
	paramsFile_ << "  END     " << endl;
	paramsFile_.close();
}



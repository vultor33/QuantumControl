#include "VultorParametrizeH.h"

#include "ControlMopac.h"

using namespace std;

VultorParametrizeH::VultorParametrizeH(){}

VultorParametrizeH::~VultorParametrizeH(){}

double VultorParametrizeH::getError(vector<double> &params)
{
	/*
	h2Pcoord = "-0.1788677855   0.0000000000   0.0000000000\n";
	h2Pcoord += " 0.8788677855   0.0000000000   0.0000000000\n";
	h2coord = "-0.0215907187   0.0000000000   0.0000000000\n";
	h2coord += " 0.7215907187   0.0000000000   0.0000000000\n";
	h3Pcoord = "-0.86064160   -2.35389491    0.00000000\n";
	h3Pcoord += "-1.73498149   -2.43398081    0.00000000\n";
	h3Pcoord += "-1.36716797   -1.63673730    0.00000000\n";
	h4Pcoord = "-1.07634620   -2.36569490    0.00000000\n";
	h4Pcoord += "-1.92044244   -2.31521126    0.00000000\n";
	h4Pcoord += "-1.45159804   -1.55801030    0.00000000\n";
	h4Pcoord += "-1.45159804    0.16340270    0.00000000\n";
	*/

	//MODELO 1
	ControlMopac controlMop1;
	vector<string> options1(5);
	options1[0] = "mopac";
	options1[1] = "h2p-mol";
	options1[2] = "RM1 UHF CHARGE=1";
	options1[3] = "hparams";
	options1[4] = "";
	vector<CoordXYZ> atoms1(2);
	atoms1[0].atomlabel = "H";
	atoms1[0].x = -0.1788677855;
	atoms1[0].y = 0.0e0;
	atoms1[0].z = 0.0e0;
	atoms1[1].atomlabel = "H";
	atoms1[1].x = 0.8788677855;
	atoms1[1].y = 0.0e0;
	atoms1[1].z = 0.0e0;
	double error1 = controlMop1.getHydrogenError(atoms1, options1, params, 1);


	//MODELO 2
	ControlMopac controlMop2;
	vector<string> options2(5);
	options2[0] = "mopac";
	options2[1] = "h2-mol";
	options2[2] = "RM1";
	options2[3] = "hparams";
	options2[4] = "";
	vector<CoordXYZ> atoms2(2);
	atoms2[0].atomlabel = "H";
	atoms2[0].x = -0.0215907187;
	atoms2[0].y = 0.0e0;
	atoms2[0].z = 0.0e0;
	atoms2[1].atomlabel = "H";
	atoms2[1].x = 0.7215907187;
	atoms2[1].y = 0.0e0;
	atoms2[1].z = 0.0e0;
	double error2 = controlMop2.getHydrogenError(atoms2, options2, params, 2);


	//MODELO 3
	ControlMopac controlMop3;
	vector<string> options3(5);
	options3[0] = "mopac";
	options3[1] = "h3p-mol";
	options3[2] = "RM1  CHARGE=1";
	options3[3] = "hparams";
	options3[4] = "";
	vector<CoordXYZ> atoms3(3);
	atoms3[0].atomlabel = "H";
	atoms3[0].x = -0.86064160;
	atoms3[0].y = -2.35389491;
	atoms3[0].z =  0.00000000;
	atoms3[1].atomlabel = "H";
	atoms3[1].x = -1.73498149;
	atoms3[1].y = -2.43398081;
	atoms3[1].z =  0.00000000;
	atoms3[2].atomlabel = "H";
	atoms3[2].x = -1.36716797;
	atoms3[2].y = -1.63673730;
	atoms3[2].z =  0.00000000;
	double error3 = controlMop3.getHydrogenError(atoms3, options3, params, 3);

	//MODELO 4
	ControlMopac controlMop4;
	vector<string> options4(5);
	options4[0] = "mopac";
	options4[1] = "h4p-mol";
	options4[2] = "RM1 CHARGE=1 UHF";
	options4[3] = "hparams";
	options4[4] = "";
	vector<CoordXYZ> atoms4(4);
	atoms4[0].atomlabel = "H";
	atoms4[0].x = -1.07634620;
	atoms4[0].y = -2.36569490;
	atoms4[0].z = 0.00000000;
	atoms4[1].atomlabel = "H";
	atoms4[1].x = -1.92044244;
	atoms4[1].y = -2.31521126;
	atoms4[1].z = 0.00000000;
	atoms4[2].atomlabel = "H";
	atoms4[2].x = -1.45159804;
	atoms4[2].y = -1.55801030;
	atoms4[2].z = 0.00000000;
	atoms4[3].atomlabel = "H";
	atoms4[3].x = -1.45159804;
	atoms4[3].y = 0.16340270;
	atoms4[3].z = 0.00000000;
	double error4 = controlMop4.getHydrogenError(atoms4, options4, params, 4);


	//MODELO 5
	ControlMopac controlMop5;
	vector<string> options5(5);
	options5[0] = "mopac";
	options5[1] = "h5p-mol";
	options5[2] = "RM1 CHARGE=1";
	options5[3] = "hparams";
	options5[4] = "";
	vector<CoordXYZ> atoms5(5);
	atoms5[0].atomlabel = "H";
	atoms5[0].x = -1.04653711;
	atoms5[0].y = -2.46693805;
	atoms5[0].z = 0.00000000;
	atoms5[1].atomlabel = "H";
	atoms5[1].x = -1.85665897;
	atoms5[1].y = -2.46693805;
	atoms5[1].z = 0.00000000;
	atoms5[2].atomlabel = "H";
	atoms5[2].x = -1.45159804;
	atoms5[2].y = -1.55801030;
	atoms5[2].z = 0.00000000;
	atoms5[3].atomlabel = "H";
	atoms5[3].x = -1.45159804;
	atoms5[3].y = -0.30341030;
	atoms5[3].z = 0.38425000;
	atoms5[4].atomlabel = "H";
	atoms5[4].x = -1.45159804;
	atoms5[4].y = -0.30341030;
	atoms5[4].z = -0.38425000;
	double error5 = controlMop5.getHydrogenError(atoms5, options5, params, 5);


	return (error1 + error2 + error3 + error4 + error5) / 5.0e0;
}






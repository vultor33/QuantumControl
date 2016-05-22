#include <iostream>
#include <vector>

#include "ControlMopac.h"
#include "ReadQuantumOutput.h"
#include "Coordstructs.h"
#include "ReadQuantumOptions.h"
#include "WriteQuantumInput.h"
#include "GamessCalcFrequency.h"
#include "VultorParametrizeH.h"

using namespace std;

int main()
{
	ControlMopac crmp_;
	vector<CoordXYZ> x(4);
	x[0].atomlabel = "B";
	x[0].x = 0;
	x[0].y = 0;
	x[0].y = 0;
	x[1].atomlabel = "H";
	x[1].x = 1;
	x[1].y = 0;
	x[1].y = 0;
	x[2].atomlabel = "H";
	x[2].x = 0;
	x[2].y = 1;
	x[2].y = 0;
	x[3].atomlabel = "H";
	x[3].x = 0;
	x[3].y = 0;
	x[3].y = 1;

	vector<MopacParams> params(14);
	params[0].paramValue = 1;
	params[1].paramValue = 1;
	params[2].paramValue = 1;
	params[3].paramValue = 1;
	params[4].paramValue = 1;
	params[5].paramValue = 1;
	params[6].paramValue = 1;
	params[7].paramValue = 1;
	params[8].paramValue = 1;
	params[9].paramValue = 1;
	params[10].paramValue = 1;
	params[11].paramValue = 1;
	params[12].paramValue = 1;
	params[13].paramValue = 1;
	params[0].paramName = "USS    H  ";
	params[1].paramName = "ZS     H  ";
	params[2].paramName = "BETAS  H  ";
	params[3].paramName = "ALP    H  ";
	params[4].paramName = "GSS    H  ";
	params[5].paramName = "FN11   H  ";
	params[6].paramName = "FN21   H  ";
	params[7].paramName = "FN31   H  ";
	params[8].paramName = "FN12   H  ";
	params[9].paramName = "FN22   H  ";
	params[10].paramName = "FN32   H  ";
	params[11].paramName = "FN13   H  ";
	params[12].paramName = "FN23   H  ";
	params[13].paramName = "FN33   H  ";


	vector<string> options(5);
	options[0] = "mopac";
	options[1] = "h5p-mol";
	options[2] = "RM1 CHARGE=1";
	options[3] = "hparams";
	options[4] = "";
	crmp_.optimize(x, options, params);



	VultorParametrizeH vp_;
	
	return 0;
}


/* 
EXEMPLO GAMESS
vector<double> params(8);
params[0] = -11.96067700;
params[1] = 1.08267370;
params[2] = -5.76544470;
params[3] = 3.06835950;
params[4] = 13.98321300;
params[5] = 0.10288880;
params[6] = 5.90172270;
params[7] = 1.17501180;

vector<string> options(20);
options[0] = "gamess";
options[1] = "teste-gamess";
options[2] = " $CONTRL SCFTYP=ROHF RUNTYP=OPTIMIZE EXETYP=RUN MPLEVL=2 MAXIT=200 MULT=2";
options[3] = " ISPHER=1 COORD=UNIQUE NOSYM=1 UNITS=ANGS PP=READ $END";
options[4] = " $GUESS GUESS=HUCKEL $END";
options[5] = " $SYSTEM MWORDS=40 MEMDDI=20  $END";
options[6] = " $SCF DIRSCF=.FALSE. $END";
options[7] = " $DATA";
options[8] = " titulo";
options[9] = "C1";
options[10] = "EndOfHeader";
options[11] = "na-base.txt";
options[12] = "li-base.txt";
options[13] = "li-base.txt";
options[14] = "EndOfBasis";
options[15] = "ActivateEcp";
options[16] = "na-ecp.txt";
options[17] = "li-ecp.txt";
options[18] = "li-ecp.txt";
options[19] = "EndOfEcp";

WriteQuantumInput wri_(options);
vector<CoordXYZ> mol(3);
mol[0].atomlabel = "Na";
mol[1].atomlabel = "Li";
mol[2].atomlabel = "Li";
mol[0].x = 0.0;
mol[0].y = 0.0;
mol[0].z = 0.0;
mol[1].x = 0.0;
mol[1].y = 0.0;
mol[1].z = 0.0;
mol[2].x = 0.0;
mol[2].y = 0.0;
mol[2].z = 0.0;

string name = wri_.createInput(mol);
ReadQuantumOutput redQ_("gamess");
redQ_.activateDeactivateReadings("ionization", false);
redQ_.readOutput("build-na-1-li-20");


EXEMPLO DE VARIAS OTIMIZACOES DO GAMESS
string gamessPathVerno = "/programas/gamess/rungms ";
string scrPath = "/scr/";
string nProc = "8";
GamessCalcFrequency gcf_(gamessPathVerno, scrPath, nProc);


// ler as paradinhas no fomato xyz
//   converter a primeira minuscula
// contruir um input de otimizacao - rodar.
// pegar o resultado - rodar de novo.
// calcular frequencia - na0li2 - POSITIVA!

*/

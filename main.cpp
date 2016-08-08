#include <iostream>
#include <vector>
#include <fstream>

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
        GamessCalcFrequency gcalc("/programas/gamess/rungms "," /scr/ "," 8 ");

	return 0;
}

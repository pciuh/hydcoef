#include "func.h"

int main (int argc, char *argv[])
{
    const int NrHullSections = 256;
    const int NrWaveFrequencies = 101;

    if(argc<3){
        printf("Missing hull.csv and conf.csv!\n");
        exit(8);
    }
    printf("\nOpening hull file: %s\n",argv[1]);
    struct Sections sec = ReadHullFile(argv[1],NrHullSections);
    struct Config   cnf = ReadConfFile(argv[2]);
    struct Hydro    hyd = Hydrostatic(sec.n,cnf.L,sec.x,sec.R);

    float w[NrWaveFrequencies];
    CalcWaveFreq(cnf.wMin,cnf.wMax,NrWaveFrequencies, w);

    // calculate added mass and damping
    struct Hcoef rad = Radiation(NrWaveFrequencies,sec.n,w,sec.x,sec.R,hyd);

    // print absolute values to screen every 10th frequency
    char chna[][10] = {"A33","A55"};
    char chua[][10] = {"kg","kgm2"};
    prihead("Added Mass",chna,chua);
    prires(w,rad.A33x,rad.A55x,NrWaveFrequencies);

    char chnb[][10] = {"B33","B55"};
    char chub[][10] = {"kg/s","kgm2/s"};
    prihead("Damping",chnb,chub);
    prires(w,rad.B33x,rad.B55x,NrWaveFrequencies);

    SaveResults(argv[1],w,rad,hyd,NrWaveFrequencies);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

#include "func.h"

#define NFRA 99   // maximum number of hull sections
#define NFREQ 99  // number of frequencies

const float G = 9.80665;  // m/s2, standard gravity
const float RHO = 1025.9; // kg/m3, standard sea water density

int main (int argc, char *argv[])
{
    int i,j,c;
    float x[NFRA], R[NFRA];
    float L,wMin,wMax;
    char str[66];
    FILE *infile, *outfile;

    if(argc<3){
        printf("Missing hull.csv and conf.csv!\n");
        exit(8);
    }
    printf("%s\n",argv[1]);
    
    // Reading hull file
    infile = fopen(argv[1],"r");
    c=0;
    i=0;
    char* STR = "%f,%f";
    while(c != EOF)
    {
        fseek(infile,-1,SEEK_CUR);
        fgets(str,sizeof(str),infile);
        if(i>0){
            sscanf(str,STR,&x[i-1],&R[i-1]);
        }
        i++;
        c = fgetc(infile);
    }
    fclose(infile);
    int n = i-1;

    if (n>NFRA) {
        printf("Too many sections! Limit is 99\n");
        exit(8);
    }

    // reading config file
    infile = fopen(argv[2],"r");
    STR = "%f,%f,%f";
    i=0;
    c=0;

    while(c != EOF)
    {
        fseek(infile,-1,SEEK_CUR);
        fgets(str,sizeof(str),infile);
        if(i>0){
            sscanf(str,STR,&L,&wMin,&wMax);
        }
        i++;
        c = fgetc(infile);
    }
    fclose(infile);
    
    printf("\nSection Area:\n");
    for(i=0;i<n;i++){
        x[i] = x[i]*L;
        R[i] = R[i]*L;
        printf("%8.1f%8.2f\n",x[i],2.0*asin(1)*pow(R[i],2));
    }

    // basic hydrostatic calculations
    float A=0;
    float xA=0;
    for(i=0;i<n-1;i++) {
        float dx = (x[i+1]-x[i]);
        xA += 2.0*x[i]*asin(1)*pow(R[i],2)*dx;
         A += 2.0*asin(1)*pow(R[i],2)*dx;
    }
    
    float LCB = xA/A; // longitudinal center of buoyancy for pitch computations
    float W = A*RHO;  // mass of the hull
    float I55 = W*pow(0.25*L,2); // pitch moment of inertia (assumed 0.25L radii of gyratio)

    printf("\n LCB:%12.3f\n",LCB);
    printf("MASS:%12.1f\n",W);

    float w[NFREQ];       
    float dw = (wMax-wMin)/NFREQ;
    
    float ww = wMin;
    i = 0;
    while(ww<wMax){
        w[i] = ww;
        ww += dw;
        i++;
    }

    float A33x[NFREQ],B33x[NFREQ];
    float A55x[NFREQ],B55x[NFREQ];

    // Calculation of hydrodynamic coefficients acc. to slender body theory by Salvesen (1970)
    for(j=0;j<NFREQ;j++){
        for(i=0;i<n-1;i++){
            float dx = (x[i+1]-x[i]);
            A33x[j] += 0.5*dx*(acal(w[j],R[i])+acal(w[j],R[i+1]));
            B33x[j] += 0.5*dx*(bcal(w[j],R[i])+bcal(w[j],R[i+1]));

            A55x[j] += 0.5*dx*(pow(x[i]-LCB,2)*acal(w[j],R[i])+pow(x[i+1]-LCB,2)*acal(w[j],R[i+1]));
            B55x[j] += 0.5*dx*(pow(x[i]-LCB,2)*bcal(w[j],R[i])+pow(x[i+1]-LCB,2)*bcal(w[j],R[i+1]));
        }
    }

    printf("\nAdded Mass:\n");
    for(i=0;i<NFREQ;i+=10){
        printf("%8.2f%12.1e%12.1e\n",w[i],A33x[i],A55x[i]);
    }

    printf("\nDamping:\n");
    for(i=0;i<NFREQ;i+=10){
        printf("%8.2f%12.1e%12.1e\n",w[i],B33x[i],B55x[i]);
    }

    // save results to file
    outfile = fopen("output/hyd_coef.csv","w");
    fprintf(outfile,"%s,%s,%s,%s,%s\n","OMEGA","A33","A55","B33","B55");
    for(i=0;i<NFREQ;i++){
        fprintf(outfile,"%f,%f,%f,%f,%f\n",w[i],A33x[i]/W,A55x[i]/I55,B33x[i]/W,B55x[i]/I55);
    }
    fclose(outfile);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"

#define NFRA 99   // maximum number of hull sections
#define NFREQ 99  // number of frequencies

int prires(float w[NFREQ],float Ax[NFREQ], float Ay[NFREQ], int n){
    for(int i=0;i<n;i+=10){
        printf("%8.2f%12.1e%12.1e\n",w[i],Ax[i],Ay[i]);
    }
    return 0;
}

int main (int argc, char *argv[])
{
    int i,j,c;
    float x[NFRA], R[NFRA];
    float L,wMin,wMax;
    char str[66];
    
    float RHO = 1025.9; // kg/m3, standard sea water density


    FILE *infile, *outfile;

    if(argc<3){
        printf("Missing hull.csv and conf.csv!\n");
        exit(8);
    }
    printf("\nOpening hull file: %s\n",argv[1]);
    
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
    
    printf("\nSection Area:\n\n");
    printf("%8s%8s%8s\n","X","R","A");
    printf("%8s%8s%8s\n\n","m","m","m2");
    for(i=0;i<n;i++){
        x[i] = x[i]*L;
        R[i] = R[i]*L;
        printf("%8.1f%8.2f%8.2f\n",x[i],R[i],asin(1)*pow(R[i],2));
    }

    // basic hydrostatic calculations
    float A=0;
    float xA=0;
    float Rmax=-1e4;
    for(i=0;i<n-1;i++) {
        float dx = (x[i+1]-x[i]);
        xA += x[i]*asin(1)*pow(R[i],2)*dx;
         A += asin(1)*pow(R[i],2)*dx;
         if(R[i]>Rmax){
            Rmax = R[i];
         }
    }
    
    float LCB = xA/A; // longitudinal center of buoyancy for pitch computations
    float W = A*RHO;  // mass of the hull
    float I55 = W*pow(0.25*L,2); // pitch moment of inertia (assumed 0.25L radii of gyratio)

    printf("\n LCB [m]:%12.3f\n",LCB);
    printf("VOL [m3]:%12.1f\n",A);
    printf("MASS [t]:%12.1f\n",W*1e-3);
    printf("  CB [-]:%12.3f\n",A/L/2/pow(Rmax,2));

    float w[NFREQ];       
    float dw = (wMax-wMin)/NFREQ;
    
    float ww = wMin;
    i = 0;
    while(ww<wMax){
        w[i] = ww;
        ww += dw;
        i++;
    }

    int nw = i-1;

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

    // print absolute values to screen every 10th frequency
    printf("\nAdded Mass:\n\n");
    printf("%8s%12s%12s\n","OMEGA","A33","A55");
    printf("%8s%12s%12s\n\n","rad/s","kg","kgm2");
    prires(w,A33x,A55x,nw);

    printf("\nDamping:\n\n");
    printf("%8s%12s%12s\n","OMEGA","B33","B55");
    printf("%8s%12s%12s\n\n","rad/s","kg/s","kgm2/s");
    prires(w,B33x,B55x,nw);

    // save results to file
    char *ofnm = strtok(argv[1], ".csv");
    strcat(ofnm,"-hc.csv");

    printf("\nSaving to: ");
    printf("%s\n",ofnm);

    outfile = fopen(ofnm,"w");
    fprintf(outfile,"%s,%s,%s,%s,%s\n","OMEGA","A33","A55","B33","B55");
    for(i=0;i<NFREQ;i++){
        fprintf(outfile,"%f,%f,%f,%f,%f\n",w[i],A33x[i]/W,A55x[i]/I55,B33x[i]/W,B55x[i]/I55);
    }
    fclose(outfile);
    return 0;
}
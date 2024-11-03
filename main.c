#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"

#define NFRA 99   // maximum number of hull sections
#define NFREQ 101  // number of frequencies

int main (int argc, char *argv[])
{
    int i,c;
    float x[NFRA], R[NFRA];
    float L,wMin,wMax;
    char str[66];
    
    FILE *infile, *outfile;

    if(argc<3){
        printf("Missing hull.csv and conf.csv!\n");
        exit(8);
    }
    printf("\nOpening hull file: %s\n",argv[1]);
    
    // Reading hull file
    infile = fopen(argv[1],"r");
    char* STR = "%f,%f";
    c=0;
    for(i=0;c!=EOF;i++){
        fseek(infile,-1,SEEK_CUR);
        fgets(str,sizeof(str),infile);
        if(i>0){
            sscanf(str,STR,&x[i-1],&R[i-1]);
        }
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
    c=0;
    for(i=0;c!=EOF;i++)
    {
        fseek(infile,-1,SEEK_CUR);
        fgets(str,sizeof(str),infile);
        if(i>0){
            sscanf(str,STR,&L,&wMin,&wMax);
        }
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
    struct Hydro hyd = hydrostatic(n,L,x,R);

    printf("\n LCB [m]:%12.3f\n",hyd.LCB);
    printf("VOL [m3]:%12.1f\n",hyd.A);
    printf("MASS [t]:%12.1f\n",hyd.W*1e-3);
    printf("  CB [-]:%12.3f\n",hyd.CB);


    //calculate wave frequencies    
    float w[NFREQ];       
    float dw = (wMax-wMin)/(NFREQ-1);
    
    float ww = wMin;
    for(i=0;ww<wMax;i++){
        w[i] = ww;
        ww += dw;
    }

    int nw = i-1;

    struct Hcoef rad = radiation(nw,n,w,x,R,hyd);

    // print absolute values to screen every 10th frequency
    char chna[][10] = {"A33","A55"};
    char chua[][10] = {"kg","kgm2"};
    prihead("Added Mass",chna,chua);
    prires(w,rad.A33x,rad.A55x,nw);

    char chnb[][10] = {"B33","B55"};
    char chub[][10] = {"kg/s","kgm2/s"};
    prihead("Damping",chnb,chub);
    prires(w,rad.B33x,rad.B55x,nw);

    // save results to file
    char *ofnm = strtok(argv[1], ".csv");
    strcat(ofnm,"-hc.csv");

    printf("\nSaving to: ");
    printf("%s\n",ofnm);

    outfile = fopen(ofnm,"w");
    fprintf(outfile,"%s,%s,%s,%s,%s\n","OMEGA","A33","A55","B33","B55");
    for(i=0;i<NFREQ;i++){
        fprintf(outfile,"%f,%f,%f,%f,%f\n",w[i],rad.A33x[i]/hyd.W,rad.A55x[i]/hyd.I55,rad.B33x[i]/hyd.W,rad.B55x[i]/hyd.I55);
    }
    fclose(outfile);
    return 0;
}
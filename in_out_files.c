/***************************************************************************
                          read_files.c  -  read files: hull and config
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/

#include "func.h"

struct Sections ReadHullFile(char* HullFileName, int NoHullSections) {
    FILE *infile;
    char str[66];
    int i;
 // Reading hull file
    struct Sections r = {{0},{0},0};
    
    infile = fopen(HullFileName,"r");
    char* STR = "%f,%f";
    int c=0;
    for(i=0;c!=EOF;i++){
        fseek(infile,-1,SEEK_CUR);
        fgets(str,sizeof(str),infile);
        if(i>0){
            sscanf(str,STR,&r.x[i-1],&r.R[i-1]);
        }
        c = fgetc(infile);
    }
    fclose(infile);
    r.n = i-1;

    if (r.n>NoHullSections) {
        printf("Too many sections! Limit is %d\n",NoHullSections);
        exit(8);
    }
    return r;
}

struct Config ReadConfFile(char* ConfFileName) {
    FILE *infile;
    char str[66];
    int i;
 // Reading hull file
    struct Config r = {0,0,0};
    
    infile = fopen(ConfFileName,"r");
    char* STR = "%f,%f,%f";
    int c=0;
    for(i=0;c!=EOF;i++){
        fseek(infile,-1,SEEK_CUR);
        fgets(str,sizeof(str),infile);
        if(i>0){
            sscanf(str,STR,&r.L,&r.wMin,&r.wMax);
        }
        c = fgetc(infile);
    }
    fclose(infile);
    
    return r;
}
void SaveResults(char *FileName, float *w, struct Hcoef rad, struct Hydro hyd, int NoWaveFrequencies){
    FILE *outfile;

    // save results to file
    char *ofnm = strtok(FileName, ".csv");
    strcat(ofnm,"-hc.csv");

    printf("\nSaving to: ");
    printf("%s\n",ofnm);

    outfile = fopen(ofnm,"w");
    fprintf(outfile,"%s,%s,%s,%s,%s\n","OMEGA","A33","A55","B33","B55");
    for(int i=0;i<NoWaveFrequencies;i++){
        fprintf(outfile,"%f,%f,%f,%f,%f\n",w[i],rad.A33x[i]/hyd.W,rad.A55x[i]/hyd.I55,rad.B33x[i]/hyd.W,rad.B55x[i]/hyd.I55);
    }
    fclose(outfile);

}

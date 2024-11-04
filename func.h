/***************************************************************************
                          func.h  -  header file
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Hydro {
    float A,LCB, W, I55, CB;
};

struct Hcoef {
   float A33x[101];
   float B33x[101];
   float A55x[101];
   float B55x[101];
};

struct Sections {
   float x[99];
   float R[99];
   int n;
};

struct Config {
   float L,wMin,wMax;
};

extern float afun(float x);
extern float bfun(float x);
extern float acal(float w, float R);
extern float bcal(float w, float R);
extern void prires(float w[],float Ax[], float Ay[], int n);
extern void prihead(char title[], char coef_name[][10], char unit_name[][10]);
extern void CalcWaveFreq(float wMin, float wMax, int NoWaveFrequencies, float *w);
extern void SaveResults(char *ofnm, float *w, struct Hcoef rad, struct Hydro hyd, int NoWaveFrequencies);
extern struct Hydro Hydrostatic(int n, float L, float x[], float R[]);
extern struct Hcoef Radiation(int nw, int n,float w[], float x[], float R[], struct Hydro hyd);
extern struct Sections ReadHullFile(char* HullFileName, int NoNullSections);
extern struct Config ReadConfFile(char* ConfFileName);
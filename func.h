/***************************************************************************
                          func.h  -  header file
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
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

extern float afun(float x);
extern float bfun(float x);
extern float acal(float w, float R);
extern float bcal(float w, float R);
extern void prires(float w[],float Ax[], float Ay[], int n);
extern void prihead(char title[], char coef_name[][10], char unit_name[][10]);
extern struct Hydro hydrostatic(int n, float L, float x[], float R[]);
extern struct Hcoef radiation(int nw, int n,float w[], float x[], float R[], struct Hydro hyd);
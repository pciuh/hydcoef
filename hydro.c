/***************************************************************************
                          hydro.c  -  calculate hydrostatic and hydrodynamic
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/

#include "func.h"

struct Hydro Hydrostatic(int n, float L, float x[], float R[]){

    const float RHO = 1025.9; // water density [kg/m3]
    
    float A=0;
    float xA=0;
    float Rmax=-1e4;

    printf("\nSection Area:\n\n");
    printf("%8s%8s%8s\n","X","R","A");
    printf("%8s%8s%8s\n\n","m","m","m2");
    for(int i=0;i<n;i++){
        x[i] = x[i]*L;
        R[i] = R[i]*L;
        printf("%8.1f%8.2f%8.2f\n",x[i],R[i],asin(1)*pow(R[i],2));
    }

    for(int i=0;i<n-1;i++) {
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
    float CB = A/L/2/pow(Rmax,2); // block coefficient

    // print hydrostatics to screen
    printf("\n LCB [m]:%12.3f\n",LCB);
    printf("VOL [m3]:%12.1f\n",A);
    printf("MASS [t]:%12.1f\n",W*1e-3);
    printf("  CB [-]:%12.3f\n",CB);


    struct Hydro r = {A, LCB, W, I55, CB};
    
    return r;
}

struct Hcoef Radiation(int nw, int n,float w[], float x[], float R[], struct Hydro hyd){

    // initialize output structure
    struct Hcoef r = {{0},{0},{0},{0}};

    // Calculation of hydrodynamic coefficients acc. to slender body theory by Salvesen (1970)
    for(int j=0;j<nw;j++){
        r.A33x[j]=0,r.B33x[j]=0;
        r.A55x[j]=0,r.B55x[j]=0;
        for(int i=0;i<n-1;i++){
            float dx = (x[i+1]-x[i]);
            r.A33x[j] += 0.5*dx*(acal(w[j],R[i])+acal(w[j],R[i+1]));
            r.B33x[j] += 0.5*dx*(bcal(w[j],R[i])+bcal(w[j],R[i+1]));

            r.A55x[j] += 0.5*dx*(pow(x[i]-hyd.LCB,2)*acal(w[j],R[i])+pow(x[i+1]-hyd.LCB,2)*acal(w[j],R[i+1]));
            r.B55x[j] += 0.5*dx*(pow(x[i]-hyd.LCB,2)*bcal(w[j],R[i])+pow(x[i+1]-hyd.LCB,2)*bcal(w[j],R[i+1]));
        }
    }
       
    return r;
}

void CalcWaveFreq(float wMin, float wMax, int NoWaveFrequencies, float *w){
    //calculate wave frequencies        
    //static float w[NoWaveFrequencies];
    float dw = (wMax-wMin)/(NoWaveFrequencies-1);
    
    float ww = wMin;
    for(int i=0;ww<wMax;i++){
        w[i] = ww;
        ww += dw;
    }
}
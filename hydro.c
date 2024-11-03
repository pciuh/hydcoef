/***************************************************************************
                          hydro.c  -  calculate hydrostatic and hydrodynamic
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/

#include "func.h"

struct Hydro hydrostatic(int n, float L, float x[], float R[]){

    const float RHO = 1025.9; // water density [kg/m3]
    
    float A=0;
    float xA=0;
    float Rmax=-1e4;

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

    struct Hydro r = {A, LCB, W, I55, CB};
    
    return r;
}

struct Hcoef radiation(int nw, int n,float w[], float x[], float R[], struct Hydro hyd){

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
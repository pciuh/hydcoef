/***************************************************************************
                          adm_approx.c  -  approximation of added mass for half circle
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/

#include "func.h"


float afun(float x) {
    // approximation for 2D added mass A33(2D) of half circle acc. to [1]
    // function returns value A33(2D)/rho A, where A = 1/2 pi R^2
    const float p = -0.60779994;
    const float q =  0.31969453;
    const float r =  0.59697270;
    return q*exp(-p*x)/pow(x,r);
}

float acal(float w, float R) {  
    // 2D added mass coefficient
    const float G = 9.80665;  // m/s2, standard gravity
    const float RHO = 1025.9; // kg/m3, standard sea water density

    float x = pow(w,2)*R/G;
    float A = asin(1)*pow(R,2);
    return R !=0 ? afun(x)*RHO*A : 0;
}
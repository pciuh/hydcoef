/***************************************************************************
                          dam_approx.c  -  approximation of damping for half circle
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/

#include "func.h"


float bfun(float x) {
    // approximation for 2D damping coefficient B33(2D) of half circle acc. to [1]
    // function returns value B33(2D)/rho w A, where A = 1/2 pi R^2, w is wave frequency [rad/s]
    // 1.853009982688, 2.319122286921, 0.762173086053
    const float p = 1.853;
    const float q = 2.319;
    const float r = 0.762;
    return q*exp(-p*pow(x,r));
}

float bcal(float w, float R) {   
    // 2D damping coefficient 
    const float G = 9.80665;  // m/s2, standard gravity
    const float RHO = 1025.9; // kg/m3, standard sea water density

    float x = pow(w,2)*R/G;
    float A = asin(1)*pow(R,2);
    return R !=0 ? bfun(x)*w*RHO*A : 0;
}
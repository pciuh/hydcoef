/***************************************************************************
                          dam_approx.c  -  approximation of damping for half circle
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "func.h"


float bfun(float x) {
    // approximation for 2D damping coefficient B33(2D) of half circle acc. to [1]
    // function returns value B33(2D)/rho w A, where A = 1/2 pi R^2, w is wave frequency [rad/s]
    float p = 1.85300997;
    float q = 2.31912227;
    float r = 0.76217309;
    return q*exp(-p*pow(x,r));
}

float bcal(float w, float R) {   
    // 2D damping coefficient 
    float G = 9.80665;  // m/s2, standard gravity
    float RHO = 1025.9; // kg/m3, standard sea water density

    float x = pow(w,2)*R/G;
    float A = asin(1)*pow(R,2);
    return R !=0 ? bfun(x)*w*RHO*A : 0;
}
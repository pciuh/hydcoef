/***************************************************************************
                          print_coef.c  -  print table of coefficients to screen
                             -------------------
    begin                : thu nov 02 18:48 CEST 2024
    copyright            : (C) 2024 by sEbastian biElicki
    email                : s.b.bielicki@icloud.com
 ***************************************************************************/

#include "func.h"

void prires(float w[],float Ax[], float Ay[], int n){
    for(int i=0;i<n;i+=10){
        printf("%8.2f%12.1e%12.1e\n",w[i],Ax[i],Ay[i]);
    }
    return;
}
void prihead(char title[], char coef_name[][10], char unit_name[][10]) {
    printf("\n%s:\n\n",title);
    printf("%8s%12s%12s\n","OMEGA",coef_name[0],coef_name[1]);
    printf("%8s%12s%12s\n\n","rad/s",unit_name[0],unit_name[1]);
    return;
}
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define N 5000
#define PI 3.141592653

//Sellmeier Coefs, from Gehrsitz, et al., J Appl Phys (87) p7825

#define A 3.3646
#define C1 50.37298
#define E1sq 7.835
#define C0 0.0912
#define E0sq 2.383

#define L 8.e-3 //device length in m

double n(double lmbda)
{
  double index, l;

  l = lmbda / 1.e-6; //m -> um
  index = sqrt(A + (C0/(E0sq - pow(l, -2.0))) + (C1/(E1sq - pow (1, -2.0))));

  return index;
}

int main(int argc, char* argv[])
{
  double lP, lS, lI;
  double nP, nS, nI;
  double dnS, dnI, detuning;
  double k, x, eff[N], lambda[N];

  int i;

  for (i=0; i<N; i++){
    lP = 780e-9;
    detuning = (double)i * 15.e-9 / N;
    lS = lP + detuning; lI = lP - detuning;

    //birefringence (fit to OWMS data)
    dnS = -1.e3*lS + 0.0017;
    dnI = -1.e3*lI + 0.0017;
    nP = n(lP); 
    nS = n(lS)-dnS; 
    nI = n(lI)-dnI;

    k = 2.0*PI*(nS/lS + nI/lI - 2*nP/lP);
    x = k*L/2;

    eff[i] = pow(sin(x)/x,2.0);
    lambda[i] = detuning * 1.e9;
  
    printf("%f\t%f\n", lambda[i], eff[i]);
  }

  return 0;
}

  


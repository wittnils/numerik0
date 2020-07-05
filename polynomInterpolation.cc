#include <iostream>
#include <vector>
#include <fstream>
#include "hdnum/hdnum.hh"

//g++ -I../hdnum/ -o PolynomInterpol polynomInterpolation.cc

int main(){
    // Interpoliere die Polynome f(x) = 1./(1+x^2) und g(x)=\sqrt(\abs{x}) auf dem Intervall I=[-1,1]
    // Aequidistante Stützstellen: x_i = -1 + h*i, mit h=2/n und i=1,...,n durch ein Polynom vom Grad n. 
    // Wir verwenden Lagrange-Polynome als Basis. 
    std::ofstream outfile;
    outfile.open("polynomdata.dat", std::ios_base::app);
    int n = 20;
    double h = 2./n;
    int density = 1000;
    double g_sum = 0; 
    double f_sum = 0;
    double x_eval = -1;
    double produkt = 1.; 
    double increment = 2./density;  
    hdnum::Vector<double> f_values(n+1,0.0);
    hdnum::Vector<double> g_values(n+1,0.0); 
    // Werte die Funktionen f,g aus und speichere sie in Vektoren, das sind unsere Koeffzienten für die 
    // Lagrange Polynome. 
    for(int i = 0; i<=n; ++i){
        g_values[i] = sqrt(abs(-1.+i*h));
        f_values[i] = 1./(1+(-1.+i*h)*(-1.+i*h)); 
    }
    // Werte f, g aus 
    for(int k = 0; k< density; ++k){
      f_sum = 0; 
      g_sum = 0; 
      for(int i = 0; i<=n ; ++i){
          produkt = 1; 
          for(int j = 0; j<=n; ++j){
            if(j!=i){
                //std::cout << produkt << std::endl; 
                produkt *= (x_eval - (-1.+j*h))*1./((i-j)*h);
            }
          } 
          f_sum += f_values[i]*produkt;
          g_sum += g_values[i]*produkt; 
      }
      x_eval += increment; 
      outfile << x_eval << " " << f_sum << " " << g_sum << std::endl; 
       
    }
    return 0;
}
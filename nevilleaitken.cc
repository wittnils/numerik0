#include <iostream>
#include <vector>
#include "hdnum/hdnum.hh"

//g++ -I../hdnum/ -o nevilleAitken nevilleaitken.cc

// Interpolation nimmt zwei Vektoren x,y entgegen, 
template<typename V> 
V interpolation(hdnum::Vector<V>& x, hdnum::Vector<V>& y, V x_eval){
    // Führe das Neville-Aitken-Schema aus
    hdnum::Vector<V> aitkenVektor(y.size());
    aitkenVektor = y; std::cout << aitkenVektor << std::endl; 
    int k = 1;
    double vorfaktor, vorvorfaktor; 
    // Wir brauchen n-1 Schritte
    while(k<y.size()){
        // Wir laufen den AitkenVektor von vorne nach hinten durch 
        for(int j = 0; j < y.size()-k; ++j){ 
          vorvorfaktor = ((x_eval-x[j+k])*(1./(x_eval-x[j]))-1);
          vorfaktor =  1./vorvorfaktor;
          aitkenVektor[j] += (aitkenVektor[j] - aitkenVektor[j+1])*vorfaktor;
      }
      ++k;
    }
    return aitkenVektor[0]; 
}

int main(){
    hdnum::Vector<double> x(2); 
    hdnum::Vector<double> y(2);
    x[0] = 1; y[0] = 3;
    x[1] = 2; y[1] = 5;
    double x_eval = 20;
    double sol;
    sol = interpolation(x,y,x_eval); 
    std::cout << sol << std::endl; 
    return 0; 
}
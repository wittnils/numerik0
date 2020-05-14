#include<iostream> 
#include<iomanip>
#include<cmath>
#include<limits>
#include<math.h>
#include<pthread.h>
// Skript zur Berechnung von so Funktionenfolgensachen 

// Geometrische Reihe der Form 2^{-i} für i=1,...,n als Intervallgrenze
long double geo_sum(int n){
    long double sum = 0;
    if(n != 0){
        for(int i=1; i<=n; i++){
        sum += pow(2,-i);
        }
    }
    return sum; 
}

long double fn(int n, long double x, long double lower_bound){
    return sin(pow(2,n+2)*M_PI*(x-lower_bound));
}

void calc(int from, int to, long double lowerBound, long double upperBound, long double increment){
    int i; 
    for(int count = from; count <= to; count++){
        i = 0;
        // Berechne die obere und untere Intervallgrenze 
        lowerBound = geo_sum(count);
        upperBound = geo_sum(count+1); 
        // Solange man sich innerhalb eines Intervalls I_n bewegt berechne f_n
        while(lowerBound+i*increment < upperBound){
            // Gibt Tupel (f_n(x),x) aus und trennt sie mit Leerzeichen 
            std::cout << std::setprecision(std::numeric_limits<long double>::digits10 + 1) 
            << fn(count,lowerBound+i*increment,lowerBound) 
            << " " << std::setprecision(std::numeric_limits<long double>::digits10 + 1) 
            << lowerBound+i*increment << std::endl; 
            i += 1; 
        }
    }
}

int main(){
    int i, from, to;;
    long double lowerBound, upperBound;
    long double increment = pow(2,-20);

    from = 0;
    to = 50;
    calc(from, to, lowerBound, upperBound, increment);
    return 0; 
}
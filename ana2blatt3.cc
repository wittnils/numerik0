#include<iostream> 
#include<iomanip>
#include<cmath>
#include<limits>
#include<math.h>
#include<quadmath.h>
// Skript zur Berechnung von so Funktionenfolgensachen 

long double geo_sum(int n){
    long double sum = 0;  
    for(int i=1; i<=n; i++){
        sum += pow(2,-i);
    }
    return sum; 
}
long double fn(int n, long double x){
    long double b_n = geo_sum(n);
    return sin(pow(2,n+2)*M_PI*(x-b_n));
}

int main(){
    int nMax, i;
    long double lowerBound, upperBound;
    long double increment = pow(2,-20);
    nMax = 50; 
    if(nMax >= 1){
        for(int count = 1; count <= nMax; count++){
            i = 0; 
            lowerBound = geo_sum(count);
            upperBound = geo_sum(count+1);
            std::cout << upperBound << std::endl; 
            //std::cout << upperBound << lowerBound << std::endl; 
            while(lowerBound+i*increment < upperBound){
                std::cout << std::setprecision(std::numeric_limits<long double>::digits10 + 1) 
                << fn(count,lowerBound+i*increment) 
                << " " << std::setprecision(std::numeric_limits<long double>::digits10 + 1) 
                << lowerBound+i*increment << std::endl; 
                i += 1; 
            }
        }
    }else{
        return 1;
    }
    return 0; 
}
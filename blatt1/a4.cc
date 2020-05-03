#include <iostream>
#include <iomanip>
#include <math.h>
#include <limits>

int main(){
    double x; 
    std::cout << "Eingabe: "; 
    std::cin >> x; 
    std::cout << std::setprecision(std::numeric_limits<double>::digits10 + 1) << std::sqrt(3) << std::endl; 
}

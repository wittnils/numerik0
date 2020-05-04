#include <iostream>
#include <iomanip>
#include <math.h>
#include <limits>

int main(){
    double sum_x = 2;
    double x = 1.; 
    float sum_y = 2;
    float y = 1.; 
    int i = 0; 
    while(sum_y!=1 or sum_x!=1){
        if(sum_y != 1){
            y = pow(10, -i);
            sum_y = 1+y;
        }
        if(sum_x != 0){
            x = pow(10,-i);
            sum_x = 1+x;
        }
        std::cout << "double x: " << x << "\nfloat y: " <<  y << std::endl;
        std::cout << "double 1+x: " << std::setprecision(std::numeric_limits<double>::digits10) << sum_x << std::endl;
        std::cout << "float 1+x: "<< std::setprecision(std::numeric_limits<float>::digits10) << sum_y << std::endl;  
        ++i; 
    } 
    std::cout << "Kleinste Zahl, die bei Addition zu keiner Nulladition führt: \n" << "double: " << x << "\nfloat: " << y << std::endl; 
}

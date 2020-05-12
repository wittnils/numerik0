#include<iostream>

double chaos(double x){
    double y;
    if(x >= 0 && x <= 1){
        if (x<0.5){
            y = 2*x;
        }
        else{
            y= 2-2*x;
        }
    return y;
    }
}

double ftilde (double x){
    double y;
    if(x >= 0 && x <= 1){
        if (x<0.5){
            y = 1.999999* x;
        }
        else{
            y= (1-x)*1.999999;
        }
    return y;
    }
}

int main(){
    double a = 0.01401;
    double b;
    for (int i=0; i<100; i++){
        std::cout<< a<< std::endl;
        b= ftilde(a);
        a=b;
    }
    return 0;
}
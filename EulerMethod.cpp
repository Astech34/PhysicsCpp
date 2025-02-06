#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

// This is the y'
double f(double x){
    return sin(x);
}

int main(){
    
    // Creating list of x values
    double x_min = 0.0, x_max = 10.0, step = 0.1;
    vector<double> x_values;
    for (double x = x_min; x < x_max; x += step){
        x_values.push_back(x);
    }

    // Apply f(x)
    vector<double> y_values(x_values.size());
    transform(x_values.begin(), x_values.end(), y_values.begin(), f);

    // Output the results
    for (size_t i = 0; i < x_values.size(); ++i){
        cout << "f(" << x_values[i] << ") = " << y_values[i] << "\n";
    }

}
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
using namespace std;

// Physics Constants
float K = 8.99 * 1e9;
float ep = 1e-9; // Smoothing

//Space parameters
// nz, ny, nx
// k, j, i
int N[] = {100, 100, 100};
int L[] = {10, 10, 10}; //meters

float nL = 10/100; //meters

// Initialize Scalarfield
float Vfield[100][100][100] = {0};
float rhoField[100][100][100] = {0};

// x,y,z in index
// Uniform sphere of charge total_charge
float rho(float x, float y, float z){
    float r2 = pow(x * 0.1, 2) + pow(y * 0.1, 2) + pow(z * 0.1, 2);
    float vol = M_PI * 4;
    float total_charge = 1.0; // C
    float val = 0;

    if (r2 < 4){
        val = total_charge/vol; // C m^-3
    }

    return val;
}

//nx, ny, nz in are index of space
//qx, qy, qz are index of space of source 
//q in coloumbs
float potential(float nx, float ny, float nz, float qx, float qy, float qz, float dq){
    float r = sqrt(pow((nx-qx) * 0.1, 2) + pow((ny-qy) * 0.1, 2) + pow((nz-qz) * 0.1, 2));
    return (K * dq)/(r-ep);
}

// Calculate the potential at a point
void calcPotentialPoint(float qx, float qy, float qz, float dq){
    for (int k=0; k < 100; k++){ // Shift plane in z
        for (int j=0; j < 100; j++){ // Shift column
            for (int i=0; i < 100; i++){ // Iterate over row
                Vfield[k][j][i] += potential(i, j, k, qx, qy, qz, dq);
            }
        }
    }
}

void calcPotentialDistribution(){
    for (int k=0; k < 100; k++){ // Shift plane in z
        for (int j=0; j < 100; j++){ // Shift column
            for (int i=0; i < 100; i++){ // Iterate over row
                float dq = rho(i, j, k);
                calcPotentialPoint(i, j, k, dq);
            }
        }
    }
}

void save2file(){

    ofstream myfile ("potential.txt");
    if (myfile.is_open()){

        for (int k=0; k < 10; k++){
            //2D Slice
            for (int j=0; j < 100; j++){ // Shift column
                for (int i=0; i < 100; i++){ // Iterate over row
                    myfile << Vfield[k][j][i] << "\t";
                }
                myfile << "\n";
            }
            myfile << "\n";
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    return;
}

int main(){

    //calcPotentialPoint(0, 0, 0);
    calcPotentialDistribution();
    save2file();

    // print z=0
    /*
    for (int j=0; j < 10; j++){ // Shift column
            for (int i=0; i < 10; i++){ // Iterate over row
                cout << Vfield[0][j][i] << " ";
            }
            cout << endl;
        }
    */
}
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
// I'm going to use isotropic voxels
const int N = 30; // # voxels in single dimension
const int L = 10; // meters

float nL = 0.33; //meters per voxel
int sK = 15; // Slice index

float niL[3] = {0.33, 0.33, 0.33};

// Initialize Scalar field
float Vfield2D[N][N] = {0};
float rhoField[N][N][N] = {0};

// I don't know how to properly make this data structure so
// I'm creating three scalar fields
float EX[N][N] = {0};
float EY[N][N] = {0};
float EZ[N][N] = {0};

// For all my functions I'm passing in the index and then doing the conversion to meters
// within the function, then I return in SI units.

// x,y,z in index
// niL list of conversions from index to meters
// Uniform sphere of charge total_charge
float rho(float x, float y, float z, float* niL){
    float r2 = pow((x-10) * niL[0], 2) + pow((y-10) * niL[1], 2) + pow((z-10) * niL[2], 2);
    float vol = M_PI * 1;
    float total_charge = 1.0; // C
    float val = 0;

    if (r2 < 1){
        val = total_charge/vol; // C m^-3
    }

    return val;
}

//nix = {nx, ny, nz} in are index of space
//qix = {qx, qy, qz} are index of space of source 
//q in coloumbs
float potential(int* nix, int* qix, float dq){
    float r = sqrt(pow((nix[0]-qix[0]) * nL, 2) + pow((nix[1]-qix[1]) * nL, 2) + pow((nix[2]-qix[2]) * nL, 2));
    return (K * dq)/(r-ep);
}

float efield(int* nix, int* qix, float dq){
    float r = sqrt(pow((nix[0]-qix[0]) * nL, 2) + pow((nix[1]-qix[0]) * nL, 2) + pow((nix[2]-qix[2]) * nL, 2));
    float r2 = pow(r-ep, 2); //I know this is slower I'm trying to make smoothing the same
    return (K * dq)/r2;
}

// Calculate the potential from a point charge
void calcPotentialPoint(int* qix, float dq){
    //for (int k=0; k < 100; k++){ // Plane = 50
    for (int j=0; j < N; j++){ // Shift column
        for (int i=0; i < N; i++){ // Iterate over row
            int nix[3] = {i, j, sK};
            Vfield2D[j][i] += potential(nix,qix,dq);
        }
    }
    //}
}

// Calculate the electric field from a point charge
void calcEFieldPoint(int* qix, float dq){
    //for (int k=0; k < 100; k++){ // Plane = 50
    for (int j=0; j < N; j++){ // Shift column
        for (int i=0; i < N; i++){ // Iterate over row
            int nix[3] = {i, j, sK};
            float Emag = efield(nix, qix, dq); // Magnitude
            
            // Projections
            // Assume Voxels so index to meter relation is isotropic
            float rmr = sqrt(pow(i-qix[0], 2) + pow(j-qix[1], 2) + pow(sK-qix[2], 2)); // this is |r' - r|
            
            EX[j][i] += Emag * (i - qix[0])/(rmr-ep); // This is x^hat dot r-r' hat
            EY[j][i] += Emag * (j - qix[1])/(rmr-ep); // y^hat
            EZ[j][i] += Emag * (sK - qix[2])/(rmr-ep); // z^hat
        }
    }
    //}
}

// qBounds for charge distribution (x0, y0, z0, xf, yf, zf)
// sBounds for space integration (x0, y0, xf, yf, z) - 2d plane
// qN for number of distribution points (Nx, Ny, Nz)
// sN for number of space points (Nx, Ny)
void calcFields(float* qBounds, int* qN, float* sBounds, int* sN){
    // Summing over charge distribution
    for (int k=0; k < qN[3]; k++){ // Shift plane in z
        for (int j=0; j < qN[2]; j++){ // Shift column
            for (int i=0; i < qN[1]; i++){ // Iterate over row
                float dq = rho(i, j, k, niL);
                int qix[3] = {i, j, k};
                calcPotentialPoint(qix, dq);
                calcEFieldPoint(qix, dq);
            }
        }
    }
}

// For 2D Slice
// Sorry this is written so horribly
void save2file2D(){

    ofstream myfile ("fields.txt");
    if (myfile.is_open()){
        //2D Slice of Potential
        for (int j=0; j < N; j++){ // Shift column
            for (int i=0; i < N; i++){ // Iterate over row
                myfile << Vfield2D[j][i] << "\t";
            }
            myfile << "\n";
        }
        myfile << "\n";

        //2D Slice of EX
        for (int j=0; j < N; j++){ // Shift column
            for (int i=0; i < N; i++){ // Iterate over row
                myfile << EX[j][i] << "\t";
            }
            myfile << "\n";
        }
        myfile << "\n";

        //2D Slice of EY
        for (int j=0; j < N; j++){ // Shift column
            for (int i=0; i < N; i++){ // Iterate over row
                myfile << EY[j][i] << "\t";
            }
            myfile << "\n";
        }
        myfile << "\n";

        //2D Slice of EZ
        for (int j=0; j < N; j++){ // Shift column
            for (int i=0; i < N; i++){ // Iterate over row
                myfile << EZ[j][i] << "\t";
            }
            myfile << "\n";
        }
        myfile << "\n";
        
        myfile.close();
    }
    else cout << "Unable to open file";

    return;
}

int main(){
    calcFields();
    save2file2D();
}
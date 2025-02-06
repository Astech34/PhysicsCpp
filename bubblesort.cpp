#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(){
    int arr[10] = {};

    // Initialize random array
    //Seed
    srand(time(NULL));

    for (int i = 0; i<10; i++){
        arr[i] = rand() % 10 + 1;
    }

    // Print array pre sorting
    for (int i = 0; i<10; i++){
        cout << arr[i] << " ";
    }
    cout << endl;

    // Bubble Sort
    for (int j = 9; j > 0; j--){
            for (int i = 0; i<j; i++){
            // Index i and i + 1 element
            int ele1 = arr[i];
            int ele2 = arr[i+1];
            
            //Compare elements
            if (ele1 > ele2){
                // If out of order then reorder them i.e 2, 1 --> 1, 2
                arr[i] = ele2;
                arr[i + 1] = ele1;
            }
        }
    }

    // Print array post sorting
    for (int i = 0; i<10; i++){
        cout << arr[i] << " ";
    }
}
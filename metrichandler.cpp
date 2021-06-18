#include "metrichandler.h"

float findMin(float* arr, int size){
    float min = arr[0];
    for (int i = 1; i < size; i++){
        if (arr[i] < min)
            min = arr[i];
    }
    return min;
}

float findMax(float* arr, int size){
    float max = arr[0];
    for (int i = 1; i < size; i++){
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

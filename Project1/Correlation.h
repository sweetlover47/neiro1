#pragma once
#define attrCount 29

void runCorrelations(float** table);
float calculateCorrelation(float* icol, float* jcol);
float calculateAverage(float* column);
float calculateStandartDeviation(float *column, float aver);
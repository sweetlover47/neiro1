#include <cmath>
#include <fstream>
#include "Correlation.h"

void runCorrelations(float **table) {
	float** cormap = new float*[attrCount];
	for (int i = 0; i < attrCount; ++i)
		cormap[i] = new float[attrCount];
	for (int i = 0; i < attrCount - 1; ++i) {
		for (int j = i + 1; j < attrCount; ++j) {
			cormap[i][j] = calculateCorrelation(table[i], table[j]);
			cormap[j][i] = cormap[i][j];
		}
		cormap[i][i] = 1;
	}
	cormap[attrCount - 1][attrCount - 1] = 1;
	std::ofstream out("correlation.txt");
	for (int i = 0; i < attrCount; ++i) {
		for (int j = 0; j < attrCount; ++j)
			out << cormap[i][j] << ";";
		out << std::endl;
	}
	return;
}

float calculateCorrelation(float* icol, float* jcol) {
	float x_aver = calculateAverage(icol);
	float y_aver = calculateAverage(jcol);
	float x_stdev = calculateStandartDeviation(icol, x_aver);
	float y_stdev = calculateStandartDeviation(jcol, y_aver);
	float result = 0.0;
	for (int i = 0; i < attrCount; ++i)
		result = result + (icol[i] - x_aver)*(jcol[i] - y_aver);
	return result / x_stdev / y_stdev;
}

float calculateAverage(float* column) {
	float aver = 0.0;
	int zeroes = 0;
	for (int i = 0; i < attrCount; ++i) {
		if (column[i] == 0)
			zeroes++;
		aver += column[i];
	}
	return (float)aver / (attrCount - zeroes);
}

float calculateStandartDeviation(float *col, float aver) {
	float stdev = 0.0;
	for (int i = 0; i < attrCount; ++i) {
		stdev += pow(col[i]-aver, 2);
	}
	return sqrt(stdev);
}
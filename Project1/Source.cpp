#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <array>
#include "GainRatio.h"
#include "Histogram.h"
#include "Correlation.h"

#define attrCount 29
#define classCount 3
#define allCount (attrCount+classCount)
#define totalCount (allCount-1)

bool isFloat(std::string s) {
	std::istringstream iss(s);
	float dummy;
	iss >> std::noskipws >> dummy;
	return iss && iss.eof();     // Result converted to bool
}

float** transponate(std::vector<float*> table) {
	float** matrix = new float*[attrCount];
	for (int column = 0; column < attrCount; ++column) {
		matrix[column] = new float[table.size()];
		for (int row = 0; row < table.size(); ++row)
			matrix[column][row] = table.at(row)[column];
	}
	return matrix;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> title = std::vector<std::string>();
	std::vector<float*> values = std::vector<float*>();
	std::string stmp;
	float ftmp;
	std::ifstream in("dataset.txt");
	std::string titles;
	std::getline(in, titles);
	std::istringstream ss(titles);
	while (std::getline(ss, stmp, ';')) {
		if (stmp != "")
			title.push_back(stmp);
	}
	std::getline(in, stmp); // skip row with measuring units
	int column = 0;
	int row = 0;
	while (std::getline(in, stmp)) { //line by line
		column = 0;
		ss = std::istringstream(stmp);
		std::getline(ss, stmp, ';');
		std::getline(ss, stmp, ';');
		values.push_back(new float[allCount]()); // initialization new row
		while (std::getline(ss, stmp, ';')) {	//delimeters ;
			if (isFloat(stmp)) {
				ftmp = std::stof(stmp);
				values.at(row)[column] = ftmp;
			}
			column = ++column % allCount;
		}
		if (values.at(row)[allCount - 3] == 0		//if all classes are null, remove
			&& values.at(row)[allCount - 2] == 0
			&& values.at(row)[allCount - 1] == 0)
			values.pop_back();
		else
			row++;
	}
	in.close();
	//union columns
	for (auto it = values.begin(); it != values.end(); ++it) {
		if ((*it)[allCount - 2] == 0 && (*it)[allCount - 1] != 0)
			(*it)[allCount - 2] = (*it)[allCount - 1] * 1000;
	}
	//gainRatio
	float* gR = gainRatio(values, title);
	std::ofstream out("gainRatio.txt");
	//greater sorting gainRatio
	std::vector<std::pair<float, std::string>> sorting;
	for (int i = 0; i < attrCount; ++i)
		sorting.push_back(std::pair<float, std::string>(gR[i], title[i]));
	std::sort(sorting.begin(), sorting.end());
	for (auto it = sorting.end() - 1; it != sorting.begin(); --it)
		out << it->second << "\t" << it->first << std::endl;
	out << sorting.begin()->second << "\t" << sorting.begin()->first << std::endl;
	out.close();

	//histograms
	for (int i = 0; i < attrCount; ++i)
		calculateHistogram(values, i);

	//correlation
	runCorrelations(transponate(values));

	system("pause");
	for (auto it = values.begin(); it != values.end(); ++it)
		delete[] (*it);
	values.clear();
	free(gR);
	title.clear();
	ss.clear();

	return 0;
}
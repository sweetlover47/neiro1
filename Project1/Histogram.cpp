#include <fstream>
#include <string>
#include "Histogram.h"
#include "GainRatio.h"

void calculateHistogram(std::vector<float*> table, int index) {
	std::map<float, int> uniq = countUniqFeatures(table, index);
	std::string path = "histograms\\histogram" + std::to_string(index) + ".txt";
	std::ofstream out(path);
	for (auto it = uniq.begin(); it != uniq.end(); ++it) {
		out << it->first << "\t" << it->second << "\t";
		for (int i = 0; i < it->second; ++i)
			out << "[]";
		out << std::endl;
	}
	out.close();
}

#include "GainRatio.h"


float* gainRatio(std::vector<float*> values, std::vector<std::string> titles)
{
	std::map<std::pair<float, float>, int> uniqClass = countUniqClass(values);
	const int n_S = values.size();
	const int columnCount = totalCount - 2;
	float* p_S = countClassesProbability(uniqClass, n_S);
	float entropy = calculateEntropy(p_S, uniqClass.size());
	float* gRatio = new float[columnCount];
	for (int i = 0; i < columnCount; ++i) {
		gRatio[i] = calculateGainRatio(values, uniqClass, entropy, i, columnCount);
	}
	free(p_S);
	uniqClass.clear();
	return gRatio;
}

std::map<std::pair<float, float>, int> countUniqClass(std::vector<float*> table) {
	std::map<std::pair<float, float>, int> m = std::map<std::pair<float, float>, int>();
	std::map<std::pair<float, float>, int>::iterator it;
	for (auto rowIterator = table.begin(); rowIterator != table.end(); ++rowIterator) {
		std::pair<float, float> p = std::pair<float, float>((*rowIterator)[totalCount - 2], (*rowIterator)[totalCount - 1]);
		it = m.find(p);
		if (it != m.end())	// class exists
			m[p] = ++(it->second);
		else
			m.insert(std::pair<std::pair<float, float>, int>(p, 1));
	}
	return m;
}

float* countClassesProbability(std::map<std::pair<float, float>, int> m, int n) {
	
	float* p_S = (float*)calloc(m.size(), sizeof(float));
	int i = 0;
	for (auto it = m.begin(); it != m.end(); ++it) {
		p_S[i++] = (float)it->second / n;
	}
	return p_S;
}

float calculateEntropy(float* p_S, int n) {
	float entropy = 0.0;
	for (int i = 0; i < n; ++i) {
		if (p_S[i] != 0)
			entropy += (p_S[i] * log2(p_S[i])); //else log2()=0, entropy has not changed
	}
	entropy *= (-1);
	return entropy;
}

float calculateGainRatio(std::vector<float*> table, std::map<std::pair<float, float>, int> uniqClass, float entropy, int index, int columnCount) {
	std::map<float, int> uniqFeatures = countUniqFeatures(table, index);
	float gain = calculateGain(table, uniqFeatures, uniqClass, entropy, index, columnCount);
	float splitinfo = calculateSplitInfo(uniqFeatures, table.size());
	uniqFeatures.clear();
	return gain / splitinfo;
}

std::map<float, int> countUniqFeatures(std::vector<float*> table, int index) {
	std::map<float, int> m = std::map<float, int>();
	std::map<float, int>::iterator mit;
	for (auto it = table.begin(); it != table.end(); ++it) {
		float value = (*it)[index];
		mit = m.find(value);
		if (mit != m.end()) //feature exists
			m[value] = ++(mit->second);
		else
			m.insert(std::pair<float, int>(value, 1));
	}
	return m;
}

float calculateGain(std::vector<float*> table, std::map<float, int> uniqFeatures, std::map<std::pair<float, float>, int> uniqClass, float entropy, int index, int columnCount) {
	const int rows = table.size();
	float** p_Si = countFeaturesProbability(uniqFeatures, uniqClass, table, index);
	float* featuresEntropy = new float[uniqFeatures.size()];
	for (int i = 0; i < uniqFeatures.size(); ++i) //count Entropy(Decision|Wind = i)
		featuresEntropy[i] = calculateEntropy(p_Si[i], uniqClass.size()); //calculate entropy for each feature
	float gain = entropy;
	int i = 0;
	for (auto it = uniqFeatures.begin(); it != uniqFeatures.end(); ++it, ++i) {
		gain -= (((float)it->second / rows) * featuresEntropy[i]);
	}
	for (int i = 0; i < uniqFeatures.size(); ++i)
		free(p_Si[i]);
	free(p_Si);
	free(featuresEntropy);
	return gain;
}

float** countFeaturesProbability(std::map<float, int> uniqFeatures, std::map<std::pair<float, float>, int> uniqClass, std::vector<float*> table, int index) {
	float** p_Si = new float*[uniqFeatures.size()]();
	for (int i = 0; i < uniqFeatures.size(); ++i)
		p_Si[i] = new float[uniqClass.size()]();
	int i = 0;
	for (auto row = table.begin(); row != table.end(); ++row) {
		for (auto it = uniqFeatures.begin(); it != uniqFeatures.end(); ++it, ++i) {
			if (it->first == (*row)[index]) { // if feature in row coincides it from uniqFeatures
				// then increment p_Si
				std::pair<float, float> p = std::pair<float, float>((*row)[totalCount - 2], (*row)[totalCount - 1]);
				int j = std::distance(uniqClass.begin(), uniqClass.find(p)); //find index of class in uniqClass
				p_Si[i][j] += 1;
				break;
			}
		}
		i = 0;
	}
	i = 0;
	for (auto it = uniqFeatures.begin(); it != uniqFeatures.end(); ++it, ++i)
		for (int j = 0; j < uniqClass.size(); ++j)
			p_Si[i][j] /= (it->second);
	return p_Si;
}

float calculateSplitInfo(std::map<float, int> uniqFeatures, int n) {
	float split = 0.0;
	for (auto it = uniqFeatures.begin(); it != uniqFeatures.end(); ++it) {
		float p = (float)it->second / n;
		if (p == 0) continue;
		split -= (p * log2(p));
	}
	return split;
}
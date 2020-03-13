#pragma once
#include <map>
#include <vector>

#define attrCount 29
#define classCount 3
#define allCount (attrCount+classCount)
#define totalCount (allCount-1)

float* gainRatio(std::vector<float*> values, std::vector<std::string> titles);
std::map<std::pair<float, float>, int> countUniqClass(std::vector<float*> table);
float* countClassesProbability(std::map<std::pair<float, float>, int> m, int n);
float calculateEntropy(float * p_S, int n);
float calculateGainRatio(std::vector<float*> table, std::map<std::pair<float, float>, int> uniqClass, float entropy, int index, int columnCount);
std::map<float, int> countUniqFeatures(std::vector<float*> table, int index);
float calculateGain(std::vector<float*> table, std::map<float, int> uniqFeatures, std::map<std::pair<float, float>, int> uniqClass, float entropy, int index, int columnCount);
float** countFeaturesProbability(std::map<float, int> uniqFeatures, std::map<std::pair<float, float>, int> uniqClass, std::vector<float*> table, int index);
float calculateSplitInfo(std::map<float, int> uniqFeatures, int n);

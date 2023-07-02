#pragma once

#include <vector>
#include <functional>
#include <map>
#include <algorithm>

std::vector<float> SvennsAlgorithm(float DeltaLambda, std::pair<float, float> StartingPoint, std::pair<float, float> Direction, std::function<float(float, float)> func);
std::pair<float, float> GoldenSectionSearch(std::pair<float, float> LambdaInterval, std::pair<float, float> StartingPoint, std::function<float(float, float)> func, std::pair<float, float> Direction, float epsilon);
std::pair<float, float> DSCPowell(std::vector<float> LambdaInterval, std::pair<float, float> StartPoint, std::function<float(float, float)> func, std::pair<float, float> Direction, float epsilon);
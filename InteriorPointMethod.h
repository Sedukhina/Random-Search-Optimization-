#pragma once
#include <vector>
#include <math.h>
#include <functional>
#include <utility>

class InteriorPointMethod
{
public:

	InteriorPointMethod(std::pair<float, float>StartingPoint, float Radius, int PointsAmount, std::function<float(float, float)> TargetFunction, float SectorAngle, float epsilon, float lambda, float deltaLambda, std::vector<std::function<float(float, float)>> EqualityBarriers, std::vector<std::function<float(float, float)>> InequalityBarriers);

	std::pair<std::pair<float, float>, float> CalculateDynamic();
	std::pair<std::pair<float, float>, float> Calculate();

private:
	std::function<float(float, float)> TargetFunction;
	std::vector<std::function<float(float, float)>> EqualityBarriers;
	std::vector<std::function<float(float, float)>> InequalityBarriers;
	int PenaltyParametr = 100;
	std::pair<float, float>StartingPoint;
	float Radius;
	int PointsAmount;
	float SectorAngle; 
	float Epsilon; 
	float Lambda; 
	float DeltaLambda;
	float StartingPointFuncValue;

	float CalculatePenaltyFunction(float x1, float x2);

	bool StoppingCriteriaSatisfied(std::pair<float, float>NewPoint, float NewPointFuncValue);
};


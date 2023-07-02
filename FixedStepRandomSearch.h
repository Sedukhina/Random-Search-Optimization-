#pragma once
#include <utility>
#include <random>
#include <functional>

class FixedStepRandomSearch
{
public:
	FixedStepRandomSearch(std::pair<float, float>StartingPoint, float Radius, int PointsAmount, std::function<float(float, float)> TargetFunction, float SectorAngle, float epsilon, float lambda, float deltaLambda);

	std::pair<std::pair<float, float>, float> Calculate();

private:
	bool StoppingCriteriaSatisfied(std::pair<float, float>StartingPoint, float StartingPointFuncValue, std::pair<float, float>NewPoint, float NewPointFuncValue);

	std::pair<std::pair<std::pair<float, float>, float>, float> CalculateMinPointOnCircle(std::pair<std::pair<float, float>, float> Center, std::pair<float, float>CurrentSectorAngle);

	std::vector<float> GenerateAngles(std::pair<float, float>CurrentSectorAngle);

	const float PIdiv180 = 0.01745f;

	std::pair<float, float>Point;
	float Radius;
	int PointsAmount;
	std::function<float(float, float)> TargetFunction;
	float SectorAngle;
	float Epsilon;
	float Lambda;
	float DeltaLambda;

	bool GoldenSection = 1;
};


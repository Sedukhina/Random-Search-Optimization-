#include "InteriorPointMethod.h"
#include "FixedStepRandomSearch.h"

InteriorPointMethod::InteriorPointMethod(std::pair<float, float>startingPoint, float radius, int pointsAmount, std::function<float(float, float)> targetFunction, float sectorAngle, float epsilon, float lambda, float deltaLambda, std::vector<std::function<float(float, float)>> equalityBarriers, std::vector<std::function<float(float, float)>> inequalityBarriers)
{
	TargetFunction = targetFunction;
	EqualityBarriers = equalityBarriers;
	InequalityBarriers = inequalityBarriers;
	StartingPoint = startingPoint;
	Radius = radius;
	PointsAmount = pointsAmount;
	SectorAngle = sectorAngle;
	Epsilon = epsilon;
	Lambda = lambda;
	DeltaLambda = deltaLambda;
}

std::pair<std::pair<float, float>, float> InteriorPointMethod::CalculateDynamic()
{
	PenaltyParametr = 100;
	StartingPointFuncValue = TargetFunction(StartingPoint.first, StartingPoint.second);
	while (true) {
		std::function<float(float, float)> f = [=](float x1, float x2) {
			return this->CalculatePenaltyFunction(x1, x2);
		};
		FixedStepRandomSearch FSRS = FixedStepRandomSearch(StartingPoint, Radius, PointsAmount, f, SectorAngle, Epsilon, Lambda, DeltaLambda);
		std::pair<std::pair<float, float>, float> UnconstrainedResult = FSRS.Calculate();
		float NewPointFuncValue = TargetFunction(UnconstrainedResult.first.first, UnconstrainedResult.first.second);
		if (StoppingCriteriaSatisfied(UnconstrainedResult.first, NewPointFuncValue))
			return { UnconstrainedResult.first, NewPointFuncValue };
		else
		{
			StartingPoint = UnconstrainedResult.first;
			StartingPointFuncValue = NewPointFuncValue;
			PenaltyParametr = PenaltyParametr * 10;
		}
	}
}

std::pair<std::pair<float, float>, float> InteriorPointMethod::Calculate()
{
	StartingPointFuncValue = TargetFunction(StartingPoint.first, StartingPoint.second);
	while (true) {
		std::function<float(float, float)> f = [=](float x1, float x2) {
			return this->CalculatePenaltyFunction(x1, x2);
		};
		FixedStepRandomSearch FSRS = FixedStepRandomSearch(StartingPoint, Radius, PointsAmount, f, SectorAngle, Epsilon, Lambda, DeltaLambda);
		std::pair<std::pair<float, float>, float> UnconstrainedResult = FSRS.Calculate();
		float NewPointFuncValue = TargetFunction(UnconstrainedResult.first.first, UnconstrainedResult.first.second);
		return { UnconstrainedResult.first, NewPointFuncValue };
	}
}

float InteriorPointMethod::CalculatePenaltyFunction(float x1, float x2)
{
	float result = 0;
	result += TargetFunction(x1, x2);
	for (std::function<float(float, float)> f : EqualityBarriers)
	{
		result += PenaltyParametr * pow(f(x1, x2), 2.0f);
	}
	for (std::function<float(float, float)> f : InequalityBarriers)
	{
		float Inside = f(x1, x2);
		if (Inside > 0)
			result -= (1.f / PenaltyParametr) * log(Inside);
		else
			return 1000000.f;
	}
	return result;
}

bool InteriorPointMethod::StoppingCriteriaSatisfied(std::pair<float, float>NewPoint, float NewPointFuncValue)
{
	float PointDiffSqr = abs((NewPoint.first - StartingPoint.first)* (NewPoint.first - StartingPoint.first) + (NewPoint.second - StartingPoint.second) * (NewPoint.second - StartingPoint.second));
	float StartingPoinAbsSqr = abs(StartingPoint.first*StartingPoint.first + StartingPoint.second*StartingPoint.second);
	bool criteria1 = (PointDiffSqr/StartingPoinAbsSqr) < (Epsilon*Epsilon);
	bool criteria2 = (NewPointFuncValue - StartingPointFuncValue) <  Epsilon;
	return criteria1 && criteria2;
}

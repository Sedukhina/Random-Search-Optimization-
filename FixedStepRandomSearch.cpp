#include "FixedStepRandomSearch.h"
#include "Search.h"

bool FixedStepRandomSearch::StoppingCriteriaSatisfied(std::pair<float, float>StartingPoint, float StartingPointFuncValue, std::pair<float, float>NewPoint, float NewPointFuncValue)
{
	std::pair<float, float> Direction = { NewPoint.first - StartingPoint.first, NewPoint.second - StartingPoint.second };
	float DirectionABS = Direction.first * Direction.first + Direction.second * Direction.second;
	float StartingPointABS = StartingPoint.first * StartingPoint.first + StartingPoint.second * StartingPoint.second;
	bool criteria1 = DirectionABS / StartingPointABS <= Epsilon * Epsilon;
	bool criteria2 = abs(NewPointFuncValue - StartingPointFuncValue) < Epsilon;
	return criteria1 && criteria2;
}

FixedStepRandomSearch::FixedStepRandomSearch(std::pair<float, float> StartingPoint, float radius, int pointsAmount, std::function<float(float, float)> func, float sectorAngle, float epsilon, float lambda, float deltaLambda)
{
	Point = StartingPoint;
	Radius = radius;
	PointsAmount = pointsAmount;
	TargetFunction = func;
	SectorAngle = sectorAngle;
	Epsilon = epsilon;
	Lambda = lambda;
	DeltaLambda = deltaLambda;
}

std::pair<std::pair<float, float>, float> FixedStepRandomSearch::Calculate()
{
	//First sector is a full circle
	std::pair<float, float>SectorAngles = { 0.0f, 360.0f };
	int ReductionCount = 0;
	float StartingPointFunctionValue = TargetFunction(Point.first, Point.second);
	while (true) {
		//Selecting points on circle(sector) and choosing one with minimum function value 
		std::pair<std::pair<std::pair<float, float>, float>, float> MinPointAndAngle = CalculateMinPointOnCircle({ Point,  StartingPointFunctionValue }, SectorAngles);
		if (MinPointAndAngle.second == 361.0f)
		{
			ReductionCount++;
			if (ReductionCount == 5)
			{
				SectorAngles = { 0.0f, 360.0f };
			}
			if (ReductionCount > 50)
			{
				return { Point, StartingPointFunctionValue };
			}
			//Radius reduction
			Radius = Radius / 2;
			if (Radius < Epsilon)
				Radius = Epsilon;
		}
		else
		{
			std::pair <std::pair<float, float>, float> MinPoint = MinPointAndAngle.first;
			//Setting direction for search
			std::pair<float, float> Direction = { MinPoint.first.first - Point.first, MinPoint.first.second - Point.second };
			//Calculating minimum on this direction
			std::vector<float> LambdaInterval = SvennsAlgorithm(DeltaLambda, Point, Direction, TargetFunction);
			if ((LambdaInterval[1] - LambdaInterval[0]) > Epsilon)
			{
				ReductionCount = 0;
				std::pair<float, float> NewPoint;
				if(GoldenSection)
					NewPoint = GoldenSectionSearch({ LambdaInterval.front(), LambdaInterval.back() }, Point, TargetFunction, Direction, Lambda);
				else
					NewPoint = DSCPowell(LambdaInterval, Point, TargetFunction, Direction, Lambda);
				//Check if minimum found
				if (StoppingCriteriaSatisfied(Point, StartingPointFunctionValue, NewPoint, TargetFunction(NewPoint.first, NewPoint.second)))
				{
					return MinPoint;
				}
				Radius = Radius * 1.2f;
				SectorAngles = { MinPointAndAngle.second - SectorAngle / 2, MinPointAndAngle.second + SectorAngle / 2 };
				Point = NewPoint;
				StartingPointFunctionValue = TargetFunction(Point.first, Point.second);
			}
		}
		//std::cout << "Iteration end on f(" << MinPointAndAngle.first.first.first << "; " << MinPointAndAngle.first.first.second << ") = " << MinPointAndAngle.first.second << " and Radius = " << Radius << " and k = " << k << "\n";
	}
}

std::vector<float> FixedStepRandomSearch::GenerateAngles(std::pair<float, float>CurrentSectorAngle) {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_real_distribution<> Dist(CurrentSectorAngle.first, CurrentSectorAngle.second);
	std::vector<float> Points;
	for (int i = 0; i < PointsAmount; i++)
		Points.push_back((float)Dist(gen));
	return Points;
}

//Returns Point and Value
std::pair<std::pair<std::pair<float, float>, float>, float> FixedStepRandomSearch::CalculateMinPointOnCircle(std::pair<std::pair<float, float>, float> Center, std::pair<float, float>CurrentSectorAngle) {
	//Generating Points to check
	std::vector<float> Angles = GenerateAngles(CurrentSectorAngle);
	//Min point is a result which contatins point, function value for this point and angle
	std::pair<std::pair<float, float>, float> MinPoint = Center;
	float MinPointAngle = 361.0f;
	for (float Angle : Angles)
	{
		//Current Point is Center Point + radius rotated on angle
		std::pair<std::pair<float, float>, float> CurrentPoint = Center;
		float AngleRad = Angle * PIdiv180;
		CurrentPoint.first.first += Radius * cos(AngleRad);
		CurrentPoint.first.second += Radius * sin(AngleRad);
		CurrentPoint.second = TargetFunction(CurrentPoint.first.first, CurrentPoint.first.second);
		//If Minimum Point value is bigger then current, current point become MinPoint
		if (CurrentPoint.second < MinPoint.second) {
			MinPoint = CurrentPoint;
			MinPointAngle = Angle;
		}
	}
	return { MinPoint, MinPointAngle };
}

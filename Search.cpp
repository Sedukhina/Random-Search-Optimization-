#pragma once
#include "Search.h"

std::vector<float> SvennsAlgorithm(float DeltaLambda, std::pair<float, float> StartingPoint, std::pair<float, float> Direction, std::function<float(float, float)> func)
{
	//Points stored as lambda values
	float PreviousPoint = 0;
	float CurrentPoint = 0;
	float NextPoint = 0;
	float CurPointFunctionValue;
	float NextPointFunctionValue = func(StartingPoint.first, StartingPoint.second);
	int i = 0;
	while (true) {

		PreviousPoint = CurrentPoint;
		CurrentPoint = NextPoint;
		CurPointFunctionValue = NextPointFunctionValue;

		//Calculating Next Point
		NextPoint += DeltaLambda * (float)pow(2, i);
		NextPointFunctionValue = func(StartingPoint.first + NextPoint * Direction.first, StartingPoint.second + NextPoint * Direction.second);

		//If next point function value is bigger then minimum is between current and next point
		if (NextPointFunctionValue > CurPointFunctionValue) {
			//Final algortithm step 
			float LastPoint = (CurrentPoint + NextPoint) / 2;
			float LastPointFunctionValue = func(StartingPoint.first + LastPoint, StartingPoint.second + LastPoint);

			if (LastPointFunctionValue < CurPointFunctionValue)
			{
				return { CurrentPoint, LastPoint, NextPoint };
			}
			else
			{
				return { PreviousPoint, CurrentPoint, LastPoint };
			}
		}
		i++;
	}
}

std::pair<float, float> GoldenSectionSearch(std::pair<float, float> LambdaInterval, std::pair<float, float> StartingPoint, std::function<float(float, float)> func, std::pair<float, float> Direction, float epsilon)
{
	float IntervalLength = LambdaInterval.second - LambdaInterval.first;
	float Lambda1 = LambdaInterval.first + 0.382f * IntervalLength;
	float Lambda2 = LambdaInterval.first + 0.618f * IntervalLength;
	//Function value calculations
	float Lambda1FunctionValue = func(StartingPoint.first + Lambda1 * Direction.first, StartingPoint.second + Lambda1 * Direction.second);
	float Lambda2FunctionValue = func(StartingPoint.first + Lambda2 * Direction.first, StartingPoint.second + Lambda2 * Direction.second);
	//Choosing right or left branch by the function value
	while (true) {
		if (IntervalLength < epsilon || isnan(Lambda1FunctionValue) || isnan(Lambda2FunctionValue))
		{
			return{ StartingPoint.first + ((LambdaInterval.first + LambdaInterval.second) / 2) * Direction.first, StartingPoint.second + ((LambdaInterval.second + LambdaInterval.second) / 2) * Direction.second };
		}
		if (Lambda1FunctionValue <= Lambda2FunctionValue) {
			LambdaInterval.second = Lambda2;
			IntervalLength = LambdaInterval.second - LambdaInterval.first;
			Lambda2 = Lambda1;
			Lambda1 = LambdaInterval.first + 0.382f * IntervalLength;
			Lambda2FunctionValue = Lambda1FunctionValue;
			Lambda1FunctionValue = func(StartingPoint.first + Lambda1 * Direction.first, StartingPoint.second + Lambda1 * Direction.second);
		}
		if (Lambda1FunctionValue >= Lambda2FunctionValue) {
			LambdaInterval.first = Lambda1;
			IntervalLength = LambdaInterval.second - LambdaInterval.first;
			Lambda1 = Lambda2;
			Lambda2 = LambdaInterval.first + 0.618f * IntervalLength;
			Lambda1FunctionValue = Lambda2FunctionValue;
			Lambda2FunctionValue = func(StartingPoint.first + Lambda2 * Direction.first, StartingPoint.second + Lambda2 * Direction.second);
		}
	}
}

std::pair<float, float> DSCPowell(std::vector<float> LambdaInterval, std::pair<float, float> StartPoint, std::function<float(float, float)> func, std::pair<float, float> Direction, float epsilon)
{
	std::map<float, float> LambdasFunctionValues;
	std::vector<float> CurrentLambdas;
	for (float lambda : LambdaInterval) {
		CurrentLambdas.push_back(lambda);
		LambdasFunctionValues.insert({ lambda, func(StartPoint.first + lambda * Direction.first, StartPoint.second + lambda * Direction.second) });
	}
	float PreviousLambda = CurrentLambdas[1];

	while (true)
	{
		float a1 = (LambdasFunctionValues.at(CurrentLambdas[1]) - LambdasFunctionValues.at(CurrentLambdas[0])) / (CurrentLambdas[1] - CurrentLambdas[0]);
		float a2 = ((LambdasFunctionValues.at(CurrentLambdas[2]) - LambdasFunctionValues.at(CurrentLambdas[1]) / (CurrentLambdas[2] - CurrentLambdas[1])) - a1) / (CurrentLambdas[2] - CurrentLambdas[1]);
		float lambda = ((CurrentLambdas[0] + CurrentLambdas[1]) / 2) - (a1 / (2 * a2));
		LambdasFunctionValues.insert({ lambda, func(StartPoint.first + lambda * Direction.first, StartPoint.second + lambda * Direction.second) });

		//Stopping Criterias
		if (isnan(lambda))
			return { StartPoint.first + PreviousLambda * Direction.first, StartPoint.second + PreviousLambda * Direction.second };

		std::pair<float, float> LambdaX = { abs(lambda - CurrentLambdas[1]) * Direction.first, abs(lambda - CurrentLambdas[1]) * Direction.second };
		float LambdaXABS = (LambdaX.first * LambdaX.first + LambdaX.second * LambdaX.second);

		if (((abs(LambdasFunctionValues.at(CurrentLambdas[1]) - LambdasFunctionValues.at(lambda)) < epsilon) && (LambdaXABS < epsilon * epsilon)) || (PreviousLambda == lambda))
			return { StartPoint.first + lambda * Direction.first, StartPoint.second + lambda * Direction.second };

		PreviousLambda = lambda;

		CurrentLambdas.push_back(lambda);
		sort(CurrentLambdas.begin(), CurrentLambdas.end());

		std::pair<int, float> LambdaMinIndexAndFuncValue = { 0, LambdasFunctionValues.at(CurrentLambdas[0]) };
		for (int i = 1; i < 4; i++)
		{
			float CurrentLambdasI = CurrentLambdas[i];
			float CurrentValue = LambdasFunctionValues[CurrentLambdasI];
			if (CurrentValue < LambdaMinIndexAndFuncValue.second)
			{
				LambdaMinIndexAndFuncValue.first = i;
				LambdaMinIndexAndFuncValue.second = CurrentValue;
			}

		}

		if (LambdaMinIndexAndFuncValue.first == 0)
			LambdaMinIndexAndFuncValue.first = 1;
		if (LambdaMinIndexAndFuncValue.first == 3)
			LambdaMinIndexAndFuncValue.first = 2;

		std::vector<float> NewCurrentLambdas;
		for (int i = -1; i < 2; i++)
		{
			NewCurrentLambdas.push_back(CurrentLambdas[LambdaMinIndexAndFuncValue.first + i]);
		}
		CurrentLambdas = NewCurrentLambdas;
	}
}
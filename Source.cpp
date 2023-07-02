#pragma once

#include "InteriorPointMethod.h"

#include <iostream>
#include <math.h>
#include <string>
#include <boost/algorithm/string/replace.hpp>

int k = 0;

float RosenbrockFunction(float x1, float x2) {
	k++;
	return 100 * pow((x1 * x1 - x2), 2.0f) + pow((x1 - 1), 2.0f);
}
float RGRFunction(float x1, float x2) {
	k++;
	return 3 * pow((x1 - 22), 2.0f) - x1 * x2 + 4 * x2 * x2;
}

float Test1a(float x1, float x2)
{
	return 5-x1*x1-x2*x2;
}

float Test1b_1(float x1, float x2)
{
	return x1 + 3;
}

float Test1b_2(float x1, float x2)
{
	return 5 - x2;
}

float Test2a(float x1, float x2)
{
	return pow((x1 + 1), 2.0f) + pow((x2 - 1.5), 2.0f) - 2;
}

float Test2b(float x1, float x2)
{
	return x1 * x1 * exp(x1) - x2;
}

float Test2c_1(float x1, float x2)
{
	return 5 - x1 * x1 - x2 * x2;
}

float Test2c_2(float x1, float x2)
{
	return pow((x1 - 0.5), 2.0f) + pow((x2 + 1), 2.0f) - 3.f;
}

float Test3a(float x1, float x2)
{
	return 5.f - pow((2.f*x1 + 3), 2.0f) - x2*x2;
}

float Test3b_1(float x1, float x2)
{
	return x2 + 2.f;
}

float Test3b_2(float x1, float x2)
{
	return 0.5f - x2;
}

float Test4a(float x1, float x2)
{
	return 0.5 -x1 * x2;
}

float Test4b_1(float x1, float x2)
{
	return 10 - x1 * x1 - x2 * x2;
}

float Test4b_2(float x1, float x2)
{
	return pow((x1 - 2), 2.0f) - pow((x2 + 1), 2.0f) + 2.f;
}

//Unconstained optimization test
/*
int main() 
{
	float Averagex1 = 0;
	float Averagex2 = 0;
	float AverageResult = 0;
	float Averagek = 0;

	int n = 100;

	for (int i = 0; i < n; i++) {
		FixedStepRandomSearch Work = FixedStepRandomSearch({ -1.2f, 0.0f }, 1.45f, 7, RosenbrockFunction, 120.0f, 0.001f, 0.01f, 1.f);
		std::pair<std::pair<float, float>, float> willit = Work.Calculate();
		///std::cout << "\nPoint: ( " << willit.first.first << "; " << willit.first.second << "); " << "Value: " << willit.second << "; k = " << k <<"\n";
		//std::cout << "\n" << willit.first.first << " " << willit.first.second << " " << " " << willit.second << " " << k;
		Averagex1 += willit.first.first;
		Averagex2 += willit.first.second;
		AverageResult += willit.second;
		Averagek += k;
		k = 0;
	}
	std::string res = "\n\n\nx1 = ";
	res += std::to_string(Averagex1 / n);
	res += " x2 = ";
	res += std::to_string(Averagex2 / n);
	res += " Result = ";
	res += std::to_string(AverageResult / n);
	res += " k = ";
	res += std::to_string(Averagek / n);
	boost::replace_all(res, ".", ",");
	std::cout << res;
	return 0;
}
*/

//Constained optimization test
int main()
{
	float Averagex1 = 0;
	float Averagex2 = 0;
	float AverageResult = 0;
	float Averagek = 0;

	int n = 100;

	float min = 100000;
	float mink = 100000;

	for (int i = 0; i < n; i++) {
		std::vector<std::function<float(float, float)>> EqualityBarriers;
		std::vector<std::function<float(float, float)>> InequalityBarriers;
		InequalityBarriers.emplace_back(Test4a);
		InteriorPointMethod PleaseWork = InteriorPointMethod({ -1.2f, 0.f }, 1.45f, 7, RosenbrockFunction, 70.0f, 0.001f, 0.01f, 1.f, EqualityBarriers, InequalityBarriers);
		std::pair<std::pair<float, float>, float> willit = PleaseWork.Calculate();
		Averagex1 += willit.first.first;
		Averagex2 += willit.first.second;
		AverageResult += willit.second;
		if (willit.second < min)
			min = willit.second;
		Averagek += k;
		if (k < mink)
			mink = k;
		std::cout <<"Iteration " << i+1 <<": Point: ( " << willit.first.first << "; " << willit.first.second << "); " << "Value: " << willit.second << "; k = " << k << "\n";
		k = 0;
	}
	std::string res = "\n\n\nx1 = ";
	res += std::to_string(Averagex1 / n);
	res += " x2 = ";
	res += std::to_string(Averagex2 / n);
	res += " Result = ";
	res += std::to_string(AverageResult / n);
	res += " k = ";
	res += std::to_string(Averagek / n);
	boost::replace_all(res, ".", ",");
	std::cout << res << " min: " << min << " min k: " << mink << "\n";
	//std::cout << (Averagex1 / n) * (Averagex1 / n) + (Averagex2 / n) * (Averagex2 / n) << "\n";
	return 0;
}


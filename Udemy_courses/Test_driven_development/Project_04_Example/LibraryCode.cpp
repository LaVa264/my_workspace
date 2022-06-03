#pragma once
#include "LibraryCode.hpp"

bool isPositive(int x){
	return x >= 0;
}

int countPositives(std::vector<int> const& inputVector)
{
	return std::count_if(inputVector.begin(), inputVector.end(), isPositive);
}

#pragma once
#include <stdexcept>
#include <cmath>

template <typename T>
int Int(T num, int round_option)
{
	switch (round_option)
	{
	case 0:
		return static_cast<int>(num);
	case 1:
		return static_cast<int>(std::round(num));
	case 2:
		return static_cast<int>(std::floor(num));
	case 3:
		return static_cast<int>(std::ceil(num));
	default:
		throw std::invalid_argument("Wrong rounding option");
	}
}

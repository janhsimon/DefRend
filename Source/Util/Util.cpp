#include "Error.hpp"
#include "Util.hpp"

bool Util::checkMemory(void *pointer)
{
	if (!pointer)
	{
		Error::report("Error", "Failed to allocate memory.");
		return false;
	}

	return true;
}

float Util::convertHorizontalToVerticalFOV(float horizontal, float screenWidth, float screenHeight)
{
	return horizontal * (screenWidth / screenHeight);
}
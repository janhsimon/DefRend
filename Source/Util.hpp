#pragma once

class Util
{
public:
	static bool checkMemory(void *pointer);
	static float convertHorizontalToVerticalFOV(float horizontal, float screenWidth, float screenHeight);
};
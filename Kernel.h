#pragma once
#include "Definitions.h"
#include "png_toolkit.h"


class Kernel {

private:

	matrix weights;

//	int sumWeights = 0;

	int radius;

public:

	Kernel(matrix setWeights);

	~Kernel() {}

	stbi_uc Apply(int xCentre, int yCentre, int color, active_rectangle& activeArea, image_data& pictureData);


};
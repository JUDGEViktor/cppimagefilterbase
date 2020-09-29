#pragma once
#include "Definitions.h"
#include "png_toolkit.h"


class Kernel {

private:

	matrix weights;

	//int sumWeights;

	int radius;

public:

	Kernel(matrix setWeights);

	~Kernel() {}

	int Apply(int xCentre, int yCentre, int color, active_rectangle& activeArea, image_data& pictureData);


};
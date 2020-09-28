#include "Kernel.h"


Kernel::Kernel(matrix setWeights) {
	weights = setWeights;
	for (auto i = 0; i < weights.size(); i++) {
		for (auto j = 0; j < weights[i].size(); j++) {
			sumWeights += weights[i][j];
		}
	}
}


stbi_uc Kernel::Apply(int xCentre, int yCentre, active_rectangle& activeArea, image_data& pictureData) {
	double res = 0;
	for (auto y = yCentre - radius, yMatrix = 0; y <= yCentre + radius; y++, yMatrix++) {
		for (auto x = xCentre - radius, xMatrix = 0; x <= xCentre + radius; x++, xMatrix++) {
			if (x > activeArea.leftColumn && x < activeArea.rightColumn &&
				y > activeArea.upperLine && y < activeArea.bottomLine) {
				unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
					+ x * pictureData.compPerPixel;
				res += weights[yMatrix][xMatrix] * p[colors::R];
			}
		}
	}
	res /= sumWeights;
	if (res > 255)
		res = 255;
	else if (res < 0)
		res = 0;
	return (stbi_uc)res;
}
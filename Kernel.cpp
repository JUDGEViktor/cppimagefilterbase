#include "Kernel.h"


Kernel::Kernel(matrix setWeights) {
	weights = setWeights;
	/*sumWeights = 0;
	for (auto i = 0; i < weights.size(); i++) {
		for (auto j = 0; j < weights[i].size(); j++) {
			sumWeights += weights[i][j];
		}
	}*/
	radius = 1;
}


int Kernel::Apply(int xCentre, int yCentre, int color, active_rectangle& activeArea, image_data& pictureData) {
	int res = 0;
	int sumWeights = 0;
	for (auto y = yCentre - radius, yMatrix = 0; y <= yCentre + radius; y++, yMatrix++) {
		for (auto x = xCentre - radius, xMatrix = 0; x <= xCentre + radius; x++, xMatrix++) {
			if (x > activeArea.leftColumn && x < activeArea.rightColumn &&
				y > activeArea.upperLine && y < activeArea.bottomLine) {
				unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
					+ x * pictureData.compPerPixel;
				res += weights[yMatrix][xMatrix] * p[color];
				sumWeights += weights[yMatrix][xMatrix];
			}
		}
	} 
	res /= sumWeights;
	if (res > 255)
		res = 255;
	else if (res < 0)
		res = 0;
	return res;
}
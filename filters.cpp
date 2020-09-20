#include "Filters.h"

filters_map filters = {
	{"blackWhite", filters_type::blackWhite},
	{"Red1", filters_type::red}
};


Filter* Filter::Create(std::string filterName, std::vector<int> coordinates) {
	auto necessaryFilter = filters.find(filterName);
	if (necessaryFilter != filters.end()) {
		switch (necessaryFilter->second) {
		case (filters_type::blackWhite):
			return new BlackWhiteFilter(coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
		case (filters_type::red):
			return new RedFilter(coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
		}
	}
	return NULL;
}


void BlackWhiteFilter::apply(image_data& pictureData) {

	int leftColumn = activeArea.xLeft == 0 ? 0 : pictureData.w / activeArea.xLeft;
	int rightColumn = activeArea.xRight == 0 ? 0 : pictureData.w / activeArea.xRight;
	int upperLine = activeArea.yUpper == 0 ? 0 : pictureData.h / activeArea.yUpper;
	int bottomLine = activeArea.yBottom == 0 ? 0 : pictureData.h / activeArea.yBottom;


	uint8_t val;
	for (auto y = upperLine; y < bottomLine; y++) {
		for (auto x = leftColumn; x < rightColumn; x++) {
			unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel 
				+ x * pictureData.compPerPixel;
			val = (uint8_t)0.3 * *p + 0.6 * *(p + 1) + 1.0 * *(p + 2);
			*(p) = val;
			*(p + 1) = val;
			*(p + 2) = val;
		}
	}
	return;
}

void RedFilter::apply(image_data& pictureData) {

	int leftColumn = activeArea.xLeft == 0 ? 0 : pictureData.w / activeArea.xLeft;
	int rightColumn = activeArea.xRight == 0 ? 0 : pictureData.w / activeArea.xRight;
	int upperLine = activeArea.yUpper == 0 ? 0 : pictureData.h / activeArea.yUpper;
	int bottomLine = activeArea.yBottom == 0 ? 0 : pictureData.h / activeArea.yBottom;


	for (auto y = upperLine; y < bottomLine; y++) {
		for (auto x = leftColumn; x < rightColumn; x++) {
			unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
				+ x * pictureData.compPerPixel;
			*(p) = 255;
			*(p + 1) = 0;
			*(p + 2) = 0;
		}
	}
	return;
}

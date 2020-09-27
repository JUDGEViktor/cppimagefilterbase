#include "Filters.h"
#include "Filters.h"

filters_map filters = {
	{"blackWhite", filters_type::blackWhite},
	{"red", filters_type::red}
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
filters_map filters = {
	{"BlackWhite", filters_type::blackWhite},
	{"Red", filters_type::red},
	{"Treshold", filters_type::treshold}
};

bool Filter::IsInActiveArea(int x, int y) {
	return(x >= activeArea.leftColumn && x <= activeArea.rightColumn && y >= activeArea.upperLine && y <= activeArea.bottomLine);
}

int Filter::GetMedianValueInBox(int xCentre, int yCentre, int radius, image_data& pictureData) {
	std::vector<int> res;

	for (auto y = yCentre - radius; y <= yCentre + radius; y++) {
		for (auto x = xCentre - radius; x <= xCentre + radius; x++) {
			if (IsInActiveArea(x, y)) {
				unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
					+ x * pictureData.compPerPixel;
				res.push_back(p[colors::R]);
			}
		}
	}

	return res[res.size() / 2];

}


Filter* Filter::Create(std::string filterName, std::vector<int> coordinates, png_toolkit& const image) {
	auto necessaryFilter = filters.find(filterName);

	image_data imageData = image.getPixelData();

	int upperLine = coordinates[0] == 0 ? 0 : imageData.h / coordinates[0];
	int leftColumn = coordinates[1] == 0 ? 0 : imageData.w / coordinates[1];
	int bottomLine = coordinates[2] == 0 ? 0 : imageData.h / coordinates[2];
	int rightColumn = coordinates[3] == 0 ? 0 : imageData.w / coordinates[3];

	if (necessaryFilter != filters.end()) {
		switch (necessaryFilter->second) {
		case (filters_type::blackWhite):
			return new BlackWhiteFilter(upperLine, leftColumn, bottomLine, rightColumn);
		case (filters_type::red):
			return new RedFilter(upperLine, leftColumn, bottomLine, rightColumn);
		case (filters_type::treshold):
			return new Treshold(upperLine, leftColumn, bottomLine, rightColumn);
		}
	}
	return NULL;
}


void BlackWhiteFilter::Apply(image_data& pictureData) {
	uint8_t val;
	for (auto y = activeArea.upperLine; y < activeArea.bottomLine; y++) {
		for (auto x = activeArea.leftColumn; x < activeArea.rightColumn; x++) {
			unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel 
				+ x * pictureData.compPerPixel;
			val = (uint8_t)0.3 * p[colors::R] + 0.6 * p[colors::G] + 1.0 * p[colors::B];
			p[colors::R] = val;
			p[colors::G] = val;
			p[colors::B] = val;
		}
	}
	return;
}

void RedFilter::Apply(image_data& pictureData) {
	for (auto y = activeArea.upperLine; y < activeArea.bottomLine; y++) {
		for (auto x = activeArea.leftColumn; x < activeArea.rightColumn; x++) {
			unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
				+ x * pictureData.compPerPixel;
			p[colors::R] = 255;
			p[colors::G] = 0;
			p[colors::B] = 0;
		}
	}
	return;
}

void Treshold::Apply(image_data& pictureData) {
	BlackWhiteFilter(activeArea.upperLine, activeArea.leftColumn, activeArea.bottomLine, activeArea.rightColumn).Apply(pictureData);
	for (auto y = activeArea.upperLine; y < activeArea.bottomLine; y++) {
		for (auto x = activeArea.leftColumn; x < activeArea.rightColumn; x++) {
			int medValue = GetMedianValueInBox(x, y, 2, pictureData);
			unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
				+ x * pictureData.compPerPixel;
			if (p[colors::R] < medValue) {
				p[colors::R] = 0;
				p[colors::G] = 0;
				p[colors::B] = 0;
			}
		}
	}

	return;
}

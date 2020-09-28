#include "Filters.h"

filters_map filters = {
	{"BlackWhite", filters_type::blackWhite},
	{"Red", filters_type::red},
	{"Threshold", filters_type::threshold},
	{"Edge", filters_type::edge}
};

bool Filter::IsInActiveArea(int x, int y) {
	return(x > activeArea.leftColumn && x < activeArea.rightColumn&& y > activeArea.upperLine && y < activeArea.bottomLine);
}


Filter* Filter::Create(std::string filterName, std::vector<int> coordinates, image_data& imageData) {
	auto necessaryFilter = filters.find(filterName);

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
		case (filters_type::threshold):
			return new Threshold(upperLine, leftColumn, bottomLine, rightColumn);
		case(filters_type::edge):
			return new Edge(upperLine, leftColumn, bottomLine, rightColumn);
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
			val = stbi_uc(int((3 * p[colors::R] + 6 * p[colors::G] + 1 * p[colors::B]) / 10));
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

void Threshold::Apply(image_data& pictureData) {
	BlackWhiteFilter(activeArea.upperLine, activeArea.leftColumn, activeArea.bottomLine, activeArea.rightColumn).Apply(pictureData);
	image_data copiedPictureData = pictureData.DeepCopy();
	for (auto y = activeArea.upperLine; y < activeArea.bottomLine; y++) {
		for (auto x = activeArea.leftColumn; x < activeArea.rightColumn; x++) {
			unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
				+ x * pictureData.compPerPixel;
			int medValue = GetMedianValueInBox(x, y, copiedPictureData);
			if (p[colors::R] < medValue) {
				p[colors::R] = 0;
				p[colors::G] = 0;
				p[colors::B] = 0;
			}
		}
	}
	copiedPictureData.FreePixels();

	return;
}

int Threshold::GetMedianValueInBox(int xCentre, int yCentre, image_data& pictureData) {
	std::vector<stbi_uc> buff;

	for (auto y = yCentre - radius; y <= yCentre + radius; y++) {
		for (auto x = xCentre - radius; x <= xCentre + radius; x++) {
			if (IsInActiveArea(x, y)) {
				unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
					+ x * pictureData.compPerPixel;
				buff.push_back(p[colors::R]);
			}
		}
	}

	std::sort(buff.begin(), buff.end());

	return buff[buff.size() / 2];
}

void Edge::Apply(image_data& pictureData) {
	BlackWhiteFilter(activeArea.upperLine, activeArea.leftColumn, activeArea.bottomLine, activeArea.rightColumn).Apply(pictureData);
	int val = 0;
	image_data copiedPictureData = pictureData.DeepCopy();
	for (auto y = activeArea.upperLine; y < activeArea.bottomLine; y++) {
		for (auto x = activeArea.leftColumn; x < activeArea.rightColumn; x++) {
			val = kernel.Apply(x, y, activeArea, copiedPictureData);
			unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
				+ x * pictureData.compPerPixel;
			p[colors::R] = val;
			p[colors::G] = val;
			p[colors::B] = val;
		}
	}
	copiedPictureData.FreePixels();
	return;
}
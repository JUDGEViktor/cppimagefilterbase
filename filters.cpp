#include "Filters.h"

filters_map filters = {
	{"BlackWhite", filters_type::blackWhite},
	{"Red", filters_type::red},
	{"Treshold", filters_type::treshold}
};

//bool Filter::IsInActiveArea(int x, int y) {
//	return(x >= activeArea.leftColumn && x <= activeArea.rightColumn && y >= activeArea.upperLine && y <= activeArea.bottomLine);
//}


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
		/*case (filters_type::treshold):
			return new Treshold(upperLine, leftColumn, bottomLine, rightColumn);*/
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

//void Treshold::Apply(image_data& pictureData) {
//	BlackWhiteFilter blw = BlackWhiteFilter(activeArea.upperLine, activeArea.leftColumn, activeArea.bottomLine, activeArea.rightColumn);
//	blw.Apply(pictureData);
//	image_data copiedPictureData = pictureData.DeepCopy();
//	for (auto y = activeArea.upperLine; y < activeArea.bottomLine; y++) {
//		for (auto x = activeArea.leftColumn; x < activeArea.rightColumn; x++) {
//			int medValue = GetMedianValueInBox(x, y, 2, copiedPictureData);
//			unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
//				+ x * pictureData.compPerPixel;
//			if (p[colors::R] < medValue) {
//				p[colors::R] = 0;
//				p[colors::G] = 0;
//				p[colors::B] = 0;
//			}
//		}
//	}
//	blw.~BlackWhiteFilter();
//	copiedPictureData.FreePixels();
//
//	return;
//}
//
//int Treshold::GetMedianValueInBox(int xCentre, int yCentre, int radius, image_data& pictureData) {
//	std::vector<stbi_uc> buff;
//
//	for (auto y = yCentre - radius; y <= yCentre + radius; y++) {
//		for (auto x = xCentre - radius; x <= xCentre + radius; x++) {
//			if (IsInActiveArea(x, y)) {
//				unsigned char* p = pictureData.pixels + y * pictureData.w * pictureData.compPerPixel
//					+ x * pictureData.compPerPixel;
//				buff.push_back(p[colors::R]);
//			}
//		}
//	}
//
//	std::sort(buff.begin(), buff.end(), [](stbi_uc a, stbi_uc b) {
//		return a > b;
//		});
//
//	return buff[buff.size() / 2];
//}
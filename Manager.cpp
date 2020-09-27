#include "Manager.h"

void Manager::parseFile(char* fileConfigName) {
	int tmp;
	std::string filterName;
	std::vector<int> curBoundaries;
	std::ifstream configFile(fileConfigName);
	if (!configFile.is_open())
		return;
	while (!configFile.eof()) {
		configFile >> filterName;
		for (auto i = 0; i < 4; i++) {
			configFile >> tmp;
			curBoundaries.push_back(tmp);
		}
		filterAndArea.push_back(std::pair<std::string, std::vector<int>>(filterName, curBoundaries));
		curBoundaries.clear();
	}
}

void Manager::initFilters(png_toolkit& const image) {
	for (auto i = 0; i < filterAndArea.size(); i++) {
		filters.push_back(Filter::Create(filterAndArea[i].first, filterAndArea[i].second, image));
	}
}

void Manager::run(char* fileConfigName, char* fileInputName, char* fileOutputName) {
	png_toolkit picture;
	picture.load(fileInputName);
	parseFile(fileConfigName);
	initFilters(picture);

	image_data imageData = picture.getPixelData();
	for (auto filter : filters) {
		if (filter != NULL) {
			filter->Apply(imageData);
		}
	}

	picture.save(fileOutputName);
}

Manager::~Manager() {
	for (auto filter : filters) {
		delete(filter);
	}
}
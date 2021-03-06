#pragma once
#include "Libraries.h"
#include "png_toolkit.h"
#include "Filters.h"


class Manager {

private:

	std::vector<Filter*> filters;

	std::vector<std::pair<std::string, std::vector<int>>> filterAndArea;

	void parseFile(char* fileConfigName);

	void initFilters(image_data& imageData);


public:

	void run(char* fileConfigName, char* fileInputName, char* fileOutputName);

	~Manager();

};
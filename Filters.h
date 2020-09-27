#pragma once
#include "Libraries.h"
#include "png_toolkit.h"

enum colors {
	R,
	G,
	B
};

struct active_rectangle {
	int upperLine, leftColumn, bottomLine, rightColumn;
};

enum class filters_type {
	blackWhite,
	red,
	treshold,
	blur,
	edge
};


typedef std::map<std::string, filters_type> filters_map;


class Filter {

private:

	bool IsInActiveArea(int x, int y);

protected:

	active_rectangle activeArea;

	int GetMedianValueInBox(int x, int y, int radius, image_data& pictureData);

public:

	//virtual constructor
	static Filter* Create(std::string filterName, std::vector<int> coordinates, image_data& imageData);

	Filter(int U, int L, int B, int R) {
		activeArea.upperLine = U;
		activeArea.leftColumn = L;
		activeArea.bottomLine = B;
		activeArea.rightColumn = R;
	}
	virtual ~Filter() {}

	virtual void Apply(image_data& pictureData) = 0;

};



class BlackWhiteFilter : public Filter {

public:

	BlackWhiteFilter(int U, int L, int B, int R) : Filter(U, L, B, R) {}

	~BlackWhiteFilter() {}

	void Apply(image_data& pictureData);

};


class RedFilter : public Filter {

public:

	RedFilter(int U, int L, int B, int R) : Filter(U, L, B, R) {}

	~RedFilter() {}

	void Apply(image_data& pictureData);

};


class Treshold : public Filter {

public:

	Treshold(int U, int L, int B, int R) : Filter(U, L, B, R) {}

	~Treshold() {}

	void Apply(image_data& pictureData);
};
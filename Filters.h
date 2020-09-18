#pragma once
#include "Libraries.h"
#include "png_toolkit.h"

struct active_rectangle {
	int yUpper, xLeft, yBottom, xRight;
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

public:

	//virtual constructor
	static Filter* Create(std::string filterName, std::vector<int> coordinates);

	Filter(int U, int L, int B, int R) {
		activeArea.yUpper = U;
		activeArea.xLeft = L;
		activeArea.yBottom = B;
		activeArea.xRight = R;
	}
	virtual ~Filter() {}

	virtual void apply(image_data& pictureData) = 0;

protected:

	active_rectangle activeArea;

};



class BlackWhiteFilter : public Filter {
private:

public:

	BlackWhiteFilter(int U, int L, int B, int R) : Filter(U, L, B, R) {}

	~BlackWhiteFilter() {}

	void apply(image_data& pictureData);

};


class RedFilter : public Filter {
private:

public:

	RedFilter(int U, int L, int B, int R) : Filter(U, L, B, R) {}

	~RedFilter() {}

	void apply(image_data& pictureData);

};
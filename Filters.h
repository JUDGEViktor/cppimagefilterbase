#pragma once
#include "Definitions.h"
#include "png_toolkit.h"
#include "Kernel.h"


enum class filters_type {
	blackWhite,
	red,
	threshold,
	blur,
	edge
};


using filters_map = std::map<std::string, filters_type>;


class Filter {

private:

protected:

	active_rectangle activeArea;

	bool IsInActiveArea(int x, int y);

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


class Threshold : public Filter {

private:

	int radius = 2;

	int GetMedianValueInBox(int x, int y, image_data& pictureData);

public:

	Threshold(int U, int L, int B, int R) : Filter(U, L, B, R) {}

	~Threshold() {}

	void Apply(image_data& pictureData);
};


//class Convolution : public Filter {
//
//protected:
//
//	Kernel kernel;
//
//public:
//
//	Convolution(int U, int L, int B, int R) : Filter(U, L, B, R), kernel({{ -1, -1, -1 },
//																		  { -1,  9, -1 },
//																		  { -1, -1, -1 }}) {}
//
//	~Convolution() {}
//
//	void Apply(image_data& pictureData) = 0;
//
//};
//
//
//class Edge : public Convolution {
//
//private:
//
//
//public:
//
//	Edge(int U, int L, int B, int R) : Convolution(U, L, B, R) {}
//
//	~Edge() {}
//
//	void Apply(image_data& pictureData);
//
//};
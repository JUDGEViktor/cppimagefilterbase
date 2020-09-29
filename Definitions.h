#pragma once
#include"Libraries.h"

using matrix = std::vector<std::vector<double>>;

enum colors {
	R,
	G,
	B
};

struct active_rectangle {
	int upperLine, leftColumn, bottomLine, rightColumn;
};
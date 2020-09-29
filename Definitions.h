#pragma once
#include"Libraries.h"

using matrix = std::vector<std::vector<int>>;

enum colors {
	R,
	G,
	B
};

struct active_rectangle {
	int upperLine, leftColumn, bottomLine, rightColumn;
};
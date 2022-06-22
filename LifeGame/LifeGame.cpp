#include"LifeGame.h"

using namespace lg;

/*
* BasicCell
*/
namespace lg {

	BasicCell::BasicCell(CELL_TYPE _type) :
		type(_type)
	{}

}//End of BasicCell

/*
* CellPattern
*/
namespace lg {

	const CELL_TYPE* CellPattern::operator[](int x)const {
		return patternArray + height * x;
	}

	CellPattern MakeCellPattern(const CELL_TYPE* patternArray, int width, int height, int focusX, int focusY) {
		CELL_TYPE* pA = new CELL_TYPE[width * height];
		for (int i = 0; i != width; ++i) {
			for (int j = 0; j != height; ++j) {
				pA[i * height + j] = patternArray[j * width + i];
			}
		}
		return { pA, width, height, focusX, focusY };
	}

}//End of CellPattern


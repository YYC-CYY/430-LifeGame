#pragma once

#include <iostream>
#include "LifeGame.h"

namespace lg { //lg stand for life game

	template<class Cell = BasicCell, class Rule = BasicRule<Cell>>
	struct ConsoleWorld :public World<Cell, Rule> {
		virtual void Display()const;
		ConsoleWorld(int _sizeX, int _sizeY);
	};

    struct UIParam {
        int worldX;
        int worldY;
        int worldWidth;
        int worldHeight;
        int mapX;
        int mapY;
        int cellWidth;
        int cellHeight;
        int intervalX;
        int intervalY;
        int mapWidth;
        int mapHeight;
    };

	template<class Cell = BasicCell, class Rule = BasicRule<Cell>>
	class GUIWorld :public World<Cell, Rule> {
	protected:
		UIParam uiParam;
        std::deque<Point> highlightedCells;
		
        virtual void PaintBackground()const;
        virtual void PaintCell(int x, int y)const = 0;
        virtual void HighlightCell(int x, int y)const = 0;
        virtual void UnhighlightCell(int x, int y)const = 0;
	public:
		Point MapToUi(int x, int y)const;
		bool UiToMap(int x, int y, Point& point)const;
		virtual void Initialize();
        virtual bool HighlightPattern(int x, int y, const CellPattern& pattern, int rotation = 0);
		virtual void Display()const;
		GUIWorld(int _sizeX, int _sizeY, const UIParam& _uiParam);
	};

}//End of namespace


/*------------------------------------------
*
* Definition part
*
*-----------------------------------------*/

/*
* ConsoleWorld
*/
namespace lg {

	template<class Cell, class Rule>
	void ConsoleWorld<Cell, Rule>::Display()const {
		for (int i = 0; i != this->map.Height(); ++i) {
			for (int j = 0; j != this->map.Width(); ++j) {
				std::cout << this->map[j][i].type << " ";
			}
			std::cout << std::endl;
		}
	}

	template<class Cell, class Rule>
	ConsoleWorld<Cell, Rule>::ConsoleWorld(int _sizeX, int _sizeY) :
		World<Cell, Rule>::World(_sizeX, _sizeY)
	{}
}


/*
* GUIWorld
*/
namespace lg {

	template<class Cell, class Rule>
    void GUIWorld<Cell, Rule>::PaintBackground()const {}

    template<class Cell, class Rule>
    bool GUIWorld<Cell, Rule>::HighlightPattern(int x, int y, const CellPattern& pattern, int rotation) {
        for(Point p : highlightedCells) {
            UnhighlightCell(p.x, p.y);
        }
        highlightedCells.clear();
        if(!this->MapCellPattern(highlightedCells, x, y, pattern, rotation))
            return false;
        for(Point p : highlightedCells) {
            HighlightCell(p.x, p.y);
        }
        return true;
    }

	template<class Cell, class Rule>
	inline Point GUIWorld<Cell, Rule>::MapToUi(int x, int y)const {
		return {
			uiParam.worldX + uiParam.mapX + x * (uiParam.cellWidth + uiParam.intervalX),
			uiParam.worldY + uiParam.mapY + y * (uiParam.cellHeight + uiParam.intervalY)
        };
	}

	template<class Cell, class Rule>
	bool GUIWorld<Cell, Rule>::UiToMap(int x, int y, Point& point)const {
		x -= uiParam.worldX + uiParam.mapX;
		y -= uiParam.worldY + uiParam.mapY;
		if (x < 0 || x >= uiParam.mapWidth || y < 0 || y >= uiParam.mapHeight)
			return false;
		int gridWidth = uiParam.cellWidth + uiParam.intervalX;
		int gridHeight = uiParam.cellHeight + uiParam.intervalY;
		if (uiParam.cellWidth <= x % gridWidth ||
			uiParam.cellHeight <= y % gridHeight)
			return false;
		point.x = x / gridWidth;
		point.y = y / gridHeight;
		return true;
	}

	template<class Cell, class Rule>
	void GUIWorld<Cell, Rule>::Initialize() {
        this->rule.Initialize(this->map);
	}

	template<class Cell, class Rule>
	void GUIWorld<Cell, Rule>::Display()const {
		for (int i = 0; i != this->sizeX; ++i) {
			for (int j = 0; j != this->sizeY; ++j) {
				PaintCell(i, j);
			}
		}
        //for(Point p : highlightedCells) {
        //    HighlightCell(p.x, p.y);
        //}
	}

	template<class Cell, class Rule>
	GUIWorld<Cell, Rule>::GUIWorld(int _sizeX, int _sizeY, const UIParam& _uiParam) :
		World<Cell, Rule>::World(_sizeX, _sizeY),
		uiParam(_uiParam)
	{
		uiParam.mapWidth = uiParam.cellWidth * this->sizeX + uiParam.intervalX * (this->sizeX - 1);
        uiParam.mapHeight = uiParam.cellHeight * this->sizeY + uiParam.intervalY * (this->sizeY - 1);
	}
}//End of GUIWorld

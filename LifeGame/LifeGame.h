#pragma once

//#include <string>
#include <deque>

namespace lg { //lg stand for life game

	enum BASIC_CELL_TYPE {
		CT_NIHIL = -1,
		CT_NULL = 0,
		CT_MATRIX = 1
	};

	typedef int CELL_TYPE;

	struct BasicCell {
		CELL_TYPE type;
		BasicCell(CELL_TYPE _type = CT_NULL);
	};

	struct CellPattern {
		const CELL_TYPE* patternArray;
		const int width;
		const int height;
		const int focusX;
		const int focusY;
		const CELL_TYPE* operator[](int x)const;
	};

	CellPattern MakeCellPattern(const CELL_TYPE* patternArray, int width, int height, int focusX, int focusY);

	template<class Cell = BasicCell>
	class CellMap {
		Cell* cells;
		int width;
		int height;
        int numCells;
        int score;
        int lastScore;
        int elements;
	public:
		struct CellLine {
		private:
			Cell* cells;
		public:
			Cell& operator[](int y)const;
			CellLine(Cell* _cells);
		};
		int Width()const;
		int Height()const;
        int NumCells()const;
        int Score() const;
        int Elements() const;
        virtual void UpdateScore();
        virtual void RewardElements(int interval = 10);
        void ChangeNumCells(int num);
        void ChangeElements(int num);
		CellLine operator[](int x)const;
		CellMap& operator=(const CellMap& map);
        CellMap(int _width, int _height, int _initElements = 5);
		CellMap(const CellMap& map);
		~CellMap();
	};

	template<class Cell = BasicCell>
	struct BaseRule {
        virtual void Initialize(CellMap<Cell>& map) = 0;
		virtual void Update(CellMap<Cell>& map) = 0;
	};

	template<class Cell = BasicCell>
	struct BasicRule :public BaseRule<Cell> {
        virtual void Initialize(CellMap<Cell>& map);
		virtual void Update(CellMap<Cell>& map);
	};

	struct Point {
		int x;
		int y;
	};

	template<class Cell = BasicCell, class Rule = BasicRule<Cell>>
	class World {
	protected:
		int sizeX;
		int sizeY;
		int age;
        CellMap<Cell> map;
		Rule rule;
	public:
		virtual void Initialize();
		//typename CellMap<Cell>::CellLine operator[](int x)const;
		bool MapCellPattern(std::deque<Point>& mapping, int x, int y, const CellPattern& pattern, int rotation = 0);
        bool SetCell(int x, int y, const Cell& cell);
        bool SetCell(int x, int y, const CellPattern& pattern, int rotation = 0);
        int GetScore() const;
        int GetElements() const;
        virtual void Update();
		virtual void Display()const = 0;
        World(int _sizeX, int _sizeY);
		virtual ~World();
	};



} //End of namespace


/*------------------------------------------
* 
* Definition part
* 
*-----------------------------------------*/

/*
* CellMap
*/
namespace lg {

	template<class Cell>
	inline Cell& CellMap<Cell>::CellLine::operator[](int y)const {
		return *(cells + y + 1);
	}

	template<class Cell>
	CellMap<Cell>::CellLine::CellLine(Cell* _cells) :
		cells(_cells)
	{}

	template<class Cell>
	inline int CellMap<Cell>::Width()const {
		return width;
	}

	template<class Cell>
	inline int CellMap<Cell>::Height()const {
		return height;
	}

    template<class Cell>
    inline int CellMap<Cell>::NumCells()const {
        return numCells;
    }

    template<class Cell>
    inline void CellMap<Cell>::ChangeNumCells(int num) {
        numCells += num;
    }

    template<class Cell>
    inline void CellMap<Cell>::ChangeElements(int num) {
        elements += num;
    }

    template<class Cell>
    inline int CellMap<Cell>::Score() const {
        return score;
    }

    template<class Cell>
    inline int CellMap<Cell>::Elements() const {
        return elements;
    }

    template<class Cell>
    inline void CellMap<Cell>::UpdateScore() {
        score = NumCells();
    }

    template<class Cell>
    inline void CellMap<Cell>::RewardElements(int interval) {
        if (score - lastScore >= interval) {
            ChangeElements((score - lastScore) / interval);
            lastScore = (score / interval) * interval;
        }
    }

	template<class Cell>
	inline typename CellMap<Cell>::CellLine CellMap<Cell>::operator[](int x)const {
		return cells + (x + 1) * (height + 2);
	}

	template<class Cell>
	CellMap<Cell>& CellMap<Cell>::operator=(const CellMap<Cell>& map) {
		if (width != map.width || height != map.height) {
			throw "Exception in lg::CellMap<>::opertor=(): a map assigned to another in a different size.";
		}
        score = map.score;
        elements = map.elements;
        lastScore = map.lastScore;
        numCells = map.numCells;
		memcpy(cells, map.cells, sizeof(Cell) * (width + 2) * (height + 2));
		return *this;
	}


	template<class Cell>
    CellMap<Cell>::CellMap(int _width, int _height, int _initElements) :
		width(_width),
		height(_height),
        numCells(0),
        score(0),
        lastScore(0),
        elements(_initElements),
		cells(new Cell[(_width + 2) * (_height + 2)])
	{
		for (int i = -1; i <= height; ++i) {
			(*this)[-1][i].type = (*this)[width][i].type = CT_NIHIL;
		}
		for (int i = -1; i <= width; ++i) {
			(*this)[i][-1].type = (*this)[i][height].type = CT_NIHIL;
		}
	}

	template<class Cell>
	CellMap<Cell>::CellMap(const CellMap& map) :
		width(map.width),
		height(map.height),
        numCells(map.numCells),
        score(map.score),
        lastScore(map.lastScore),
        elements(map.elements),
		cells(new Cell[(map.width + 2) * (map.height + 2)])
	{
		memcpy(cells, map.cells, sizeof(Cell) * (width + 2) * (height + 2));
	}

	template<class Cell>
	CellMap<Cell>::~CellMap() {
        delete[] cells;
	}

}//End of CellMap

/*
* BasicRule
*/
namespace lg {

	template<class Cell>
    void BasicRule<Cell>::Initialize(CellMap<Cell>& map) {

	}

	template<class Cell>
	void BasicRule<Cell>::Update(CellMap<Cell>& map) {
		CellMap<Cell> newMap(map);
		for (int i = 0; i != map.Width(); ++i) {
			for (int j = 0; j != map.Height(); ++j) {
				int count = 0;
				if (map[i - 1][j - 1].type > 0) ++count;
				if (map[i - 1][j].type > 0) ++count;
				if (map[i - 1][j + 1].type > 0) ++count;
				if (map[i][j - 1].type > 0) ++count;
				if (map[i][j + 1].type > 0) ++count;
				if (map[i + 1][j - 1].type > 0) ++count;
				if (map[i + 1][j].type > 0) ++count;
				if (map[i + 1][j + 1].type > 0) ++count;
                if (count < 2 || count > 3) {
					newMap[i][j] = CT_NULL;
                    if (map[i][j].type > 0) {
                        newMap.ChangeNumCells(-1);
                    }
                }
                if (count == 3) {
                    newMap[i][j].type = CT_MATRIX;
                    if (map[i][j].type <= 0) {
                        newMap.ChangeNumCells(1);
                    }
                }
			}
		}
        newMap.UpdateScore();
        newMap.RewardElements();
		map = newMap;
	}

}//End of Basic Rule

/*
* World
*/
namespace lg {

	template<class Cell, class Rule>
	void World<Cell, Rule>::Initialize() {
		rule.Initialize(map);
	}

	//template<class Cell, class Rule>
	//inline typename CellMap<Cell>::CellLine World<Cell, Rule>::operator[](int x)const {
	//	return map[x];
	//}

	template<class Cell, class Rule>
	bool World<Cell, Rule>::MapCellPattern(std::deque<Point>& mapping, int x, int y, const CellPattern& pattern, int rotation) {
		int w = pattern.width, h = pattern.height,
			fX = pattern.focusX, fY = pattern.focusY;
		rotation %= 4;
		switch (rotation) {
		case 0:
			break;
		case 1:
			fX = h - 1 - pattern.focusY;
			fY = pattern.focusX;
			break;
		case 2:
			fX = w - 1 - pattern.focusX;
			fY = h - 1 - pattern.focusY;
			break;
		case 3:
			fX = pattern.focusY;
			fY = w - 1 - pattern.focusX;
			break;
		default:
			break;
		}
		int borderX = x - fX, borderY = y - fY;
		int rotatedW = w, rotatedH = h;
		if (rotation % 2 == 1) {
			rotatedW = h;
			rotatedH = w;
		}
		if (borderX < 0 || borderY < 0 || borderX + rotatedW > sizeX || borderY + rotatedH > sizeY) {
			return false;
		}
		for (int i = 0; i != w; ++i) {
			for (int j = 0; j != h; ++j) {
				if (pattern[i][j] != 0) {
					switch (rotation) {
					case 0:
						mapping.push_back({ borderX + i, borderY + j });
						break;
					case 1:
						mapping.push_back({ borderX + h - 1 - j, borderY + i });
						break;
					case 2:
						mapping.push_back({ borderX + w - 1 - i, borderY + h - 1 - j });
						break;
					case 3:
						mapping.push_back({ borderX + j, borderY + w - 1 - i });
						break;
					default:
						break;
					}
				}
			}
		}
		return true;
	}

	template<class Cell, class Rule>
    bool World<Cell, Rule>::SetCell(int x, int y, const Cell& cell) {
        if (x < 0 || y < 0 || x >= sizeX || y >= sizeY) {
            return false;
		}
        if (map.Elements() <= 0) {
            // warning
            return false;
        }
        map.ChangeElements(-1);
        if (map[x][y].type <= 0 && cell.type > 0) {
            map.ChangeNumCells(1);
        }
        if (map[x][y].type > 0 && cell.type <= 0) {
            map.ChangeNumCells(-1);
        }
		map[x][y] = cell;
	}

	template<class Cell, class Rule>
    bool World<Cell, Rule>::SetCell(int x, int y, const CellPattern& pattern, int rotation) {
		std::deque<Point> mapping;
        if (!MapCellPattern(mapping, x, y, pattern, rotation)) {
            return false;
		}
        if (map.Elements() <= 0) {
            // warning
            return false;
        }
        map.ChangeElements(-1);
		Point p;
		int i = -1;
		while (!mapping.empty()) {
			p = mapping.front();
			mapping.pop_front();
			while (pattern.patternArray[++i] == CT_NULL);
            if (map[p.x][p.y].type <= 0 && pattern.patternArray[i] > 0) {
                map.ChangeNumCells(1);
            }
            if (map[p.x][p.y].type > 0 && pattern.patternArray[i] <= 0) {
                map.ChangeNumCells(-1);
            }
			map[p.x][p.y] = pattern.patternArray[i];
		}
        return true;
	}

    template<class Cell, class Rule>
    inline int World<Cell, Rule>::GetScore() const{
        return map.Score();
    }

    template<class Cell, class Rule>
    inline int World<Cell, Rule>::GetElements() const{
        return map.Elements();
    }

	template<class Cell, class Rule>
	void World<Cell, Rule>::Update() {
		rule.Update(map);
		++age;
        Display();
	}

	template<class Cell, class Rule>
    World<Cell, Rule>::World(int _sizeX, int _sizeY) :
		sizeX(_sizeX),
		sizeY(_sizeY),
		age(0),  
		map(sizeX, sizeY)
	{}

	template<class Cell, class Rule>
	World<Cell, Rule>::~World() {

	}

}//End of World

#ifndef LIFEGAMEFORQT_H
#define LIFEGAMEFORQT_H

#include "LifeGameExtended.h"
//#include <QtWidgets/QMainWindow>
//#include <QtWidgets/QLabel>
#include <QtWidgets>


namespace lg { //lg stand for life game

    template<class Cell = BasicCell, class Rule = BasicRule<Cell>>
    class QtWorld :public GUIWorld<Cell, Rule>{
    protected:
        struct QtWidgets{
            QMainWindow* window;
            QLabel** cells;
        };

        QtWidgets qtWidgets;

        virtual void PaintCell(int x, int y)const;
        virtual void HighlightCell(int x, int y)const;
        virtual void UnhighlightCell(int x, int y)const;
    public:
        QtWorld(int _sizeX, int _sizeY, const UIParam& _uiParam, QMainWindow* qWindow);
        virtual ~QtWorld();
    };
}


/*------------------------------------------
*
* Definition part
*
*-----------------------------------------*/

/*
* QtWorld
*/
namespace lg {

    template<class Cell, class Rule>
    void QtWorld<Cell, Rule>::PaintCell(int x, int y)const {
        if(this->map[x][y].type == CT_NULL){
            qtWidgets.cells[x * this->sizeY + y]->clear();
        }
        else if(this->map[x][y].type == CT_MATRIX){
            qtWidgets.cells[x * this->sizeY + y]->setText("〇");
        }
        //qtWidgets.cells[x * this->sizeY + y]->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(0, 170, 255);");
        //qtWidgets.cells[x * this->sizeY + y]->show();
    }

    template<class Cell, class Rule>
    inline void QtWorld<Cell, Rule>::HighlightCell(int x, int y)const {
        qtWidgets.cells[x * this->sizeY + y]->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
    }

    template<class Cell, class Rule>
    inline void QtWorld<Cell, Rule>::UnhighlightCell(int x, int y)const {
        qtWidgets.cells[x * this->sizeY + y]->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(50, 170, 200);");
    }

    template<class Cell, class Rule>
    QtWorld<Cell, Rule>::QtWorld(int _sizeX, int _sizeY, const UIParam& _uiParam, QMainWindow* qWindow) :
        GUIWorld<Cell, Rule>::GUIWorld(_sizeX, _sizeY, _uiParam)
    {
            qtWidgets.window = qWindow;
            qtWidgets.cells = new QLabel*[_sizeX * _sizeY];
            Point pt;
            for(int i = 0; i != _sizeX; ++i) {
                for(int j = 0; j != _sizeY; ++j) {
                    pt = this->MapToUi(i, j);
                    qtWidgets.cells[i * _sizeY + j] = new QLabel(qWindow);
                    qtWidgets.cells[i * _sizeY + j]->setGeometry(QRect(pt.x, pt.y, this->uiParam.cellWidth, this->uiParam.cellHeight));
                    qtWidgets.cells[i * _sizeY + j]->setFrameShape (QFrame::Box);
                    qtWidgets.cells[i * _sizeY + j]->setAttribute(Qt::WA_TransparentForMouseEvents,true);
                    UnhighlightCell(i, j);
                }
            }
    }

    template<class Cell, class Rule>
    QtWorld<Cell, Rule>::~QtWorld(){
        for(int i = 0; i != this->sizeX * this->sizeY; ++i) {
            delete qtWidgets.cells[i];
        }
        delete[] qtWidgets.cells;
    }


}//End of QtWorld

#endif // LIFEGAMEFORQT_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LifeGameForQt.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    lg::QtWorld<>* world;
    lg::UIParam uiParam;
    int selectedPattern;
    int rotation;
    QTimer* timer;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void Refresh();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void on_pushButton_2_clicked();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "./ui_mainwindow.h"

const int line[3] = {
    1, 1, 1
};
const int dingzi[6] = {
    0, 1, 0,
    1, 1, 1
};
const int l[6] = {
    1, 0,
    1, 0,
    1, 1
};

lg::CellPattern patterns[4] ={
    { 0 },
    lg::MakeCellPattern(line, 3, 1, 1, 0),
    lg::MakeCellPattern(dingzi, 3, 2, 1, 1),
    lg::MakeCellPattern(l, 2, 3, 0, 2)
};



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedPattern(0)
    , rotation(0)
{
    ui->setupUi(this);
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->score->hide();
    ui->elements->hide();

    uiParam.worldHeight = 600;
    uiParam.worldWidth = 600;
    uiParam.worldX = 50;
    uiParam.worldY = 50;
    uiParam.mapX = 0;
    uiParam.mapY = 0;
    uiParam.cellWidth = 20;
    uiParam.cellHeight = 20;
    uiParam.intervalX = 2;
    uiParam.intervalY = 2;
    world = new lg::QtWorld<>(25, 20, uiParam, this);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(Refresh()));
    connect(timer, &QTimer::timeout, [=](){
            ui->score->display(world->GetScore());               // 显示分数
            ui->elements->display(world->GetElements());         // 显示生命元个数
        });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete world;
    delete timer;
}


void MainWindow::on_pushButton_clicked()
{
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    ui->pushButton_4->show();
    ui->pushButton_5->show();
    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();
    world->Initialize();
    ui->score->show();
    ui->elements->show();
    timer->start();
}

void MainWindow::Refresh()
{
    world->Update();
    world->Display();
}



void MainWindow::on_pushButton_3_clicked()
{
    rotation = 0;
    selectedPattern = 1;
    ui->centralwidget->setMouseTracking(true);
    this->setMouseTracking(true);
}


void MainWindow::on_pushButton_4_clicked()
{
    rotation = 0;
    selectedPattern = 2;
    ui->centralwidget->setMouseTracking(true);
    this->setMouseTracking(true);
}


void MainWindow::on_pushButton_5_clicked()
{
    rotation = 0;
    selectedPattern = 3;
    ui->centralwidget->setMouseTracking(true);
    this->setMouseTracking(true);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(!selectedPattern) return;
    lg::Point p = { event->pos().x(), event->pos().y() };
    if(!world->UiToMap(p.x, p.y, p)) {
        world->HighlightPattern(p.x, p.y, patterns[0]);
        return;
    }
    if(!world->HighlightPattern(p.x, p.y, patterns[selectedPattern], rotation)) return;
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(!selectedPattern) return;
    lg::Point p = { event->pos().x(), event->pos().y() };
    if(!world->UiToMap(p.x, p.y, p)) return;
    if(!world->SetCell(p.x, p.y, patterns[selectedPattern], rotation)) return;
    world->HighlightPattern(p.x, p.y, patterns[0]);
    world->Display();
    selectedPattern = 0;
    ui->centralwidget->setMouseTracking(false);
    this->setMouseTracking(false);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(!selectedPattern) return;
    if(event->key() == Qt::Key_R)
        rotation = (rotation + 1) % 4;
    int x = this->mapFromGlobal(QCursor().pos()).x();
    int y = this->mapFromGlobal(QCursor().pos()).y();
    QMouseEvent e(QEvent::MouseMove, QPointF(x, y), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    QApplication::sendEvent(this, &e);
}


void MainWindow::on_pushButton_2_clicked()
{
    static bool stopped = false;
    if(!stopped){
        timer->stop();
        ui->pushButton_2->setText("继续");
        stopped = true;
    }
    else{
        timer->start();
        ui->pushButton_2->setText("暂停");
        stopped = true;
    }
}



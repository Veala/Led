#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Commands << "set-led-state" << "get-led-state" <<
                "set-led-color" << "get-led-color" <<
                "set-led-rate" << "get-led-rate";
    Arguments[0] << "on" << "off";
    Arguments[2] << "red" << "green" << "blue";
    Arguments[4] << "0" << "1" << "2" << "3" << "4" << "5";
    state = "off";  rate = 0;   color = Qt::white;
    //int RateTimer = startTimer(1000/rate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    painter.begin(this);
    if (rate!=0) {
             if (color == Qt::red)          color = Qt::darkRed;
        else if (color == Qt::darkRed)      color = Qt::red;
        else if (color == Qt::blue)         color = Qt::darkBlue;
        else if (color == Qt::darkBlue)     color = Qt::blue;
        else if (color == Qt::green)        color = Qt::darkGreen;
        else if (color == Qt::darkGreen)    color = Qt::green;
    }
    painter.setBrush(QBrush(color));
    QRectF rect(0,0,this->width(),this->height());
    painter.drawEllipse(rect);
    QFont serifFont("Arial", 10);
    painter.setFont(serifFont);
    painter.drawText(rect,"LED");
    painter.end();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    //repaint();
    update();
}

void MainWindow::ServerOperations(QString)
{

}

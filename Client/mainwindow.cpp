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
    ui->comboBox->addItems(Commands);
    ui->comboBox_2->addItems(Arguments[0]);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),SLOT(changeArg(int)));
    connect(ui->pushButton,SIGNAL(clicked()),SLOT(send()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeArg(int index)
{
    ui->comboBox_2->clear();
    ui->comboBox_2->addItems(Arguments[index]);
}

void MainWindow::send()
{
    QString suffix = " "; if (ui->comboBox_2->currentText() == "") suffix="";
    QString message = ui->comboBox->currentText() + suffix + ui->comboBox_2->currentText() + "\n";
}

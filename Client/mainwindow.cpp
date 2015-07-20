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

    clientTcpSocket = new QTcpSocket(this);

    connect(clientTcpSocket,SIGNAL(connected()),this,SLOT(connected()));
    connect(clientTcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(clientTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
    connect(ui->pushButton,SIGNAL(clicked()),SLOT(sendToServer()));
    connect(ui->pushButton_2,SIGNAL(clicked()),SLOT(connectClick()));
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

void MainWindow::connectClick()
{
    clientTcpSocket->connectToHost(ui->lineEdit->text(), ui->lineEdit_2->text().toInt());
}

void MainWindow::readyRead()
{
    if (clientTcpSocket->bytesAvailable() >= 2) {
        QByteArray data = clientTcpSocket->readAll();
        ui->plainTextEdit->appendPlainText(QString(data));
    }
}

void MainWindow::error(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(clientTcpSocket->errorString())
                    );
    ui->plainTextEdit->appendPlainText(strError);
}

void MainWindow::sendToServer()
{
    QString suffix = " "; if (ui->comboBox_2->currentText() == "") suffix="";
    QString message = ui->comboBox->currentText() + suffix + ui->comboBox_2->currentText() + "\n";
    QByteArray Command; Command.append(message);
    clientTcpSocket->write(Command);
}

void MainWindow::connected()
{
    ui->plainTextEdit->appendPlainText("Received the connected() signal");
}

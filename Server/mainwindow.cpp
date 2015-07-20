#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Commands << "set-led-state" << "get-led-state\n" <<
                "set-led-color" << "get-led-color\n" <<
                "set-led-rate" << "get-led-rate\n";
    Arguments[0] << "on" << "off";
    Arguments[2] << "red" << "green" << "blue";
    Arguments[4] << "0" << "1" << "2" << "3" << "4" << "5";
    state = "off";  rate = 0;   color = Qt::white;

    tcpServer = new QTcpServer(this);
    nPort=3388;
    if (!tcpServer->listen(QHostAddress::Any,nPort)) {
        QMessageBox::critical(0, "Server Error", "Unable to start the server: " + tcpServer->errorString());
        tcpServer->close();
        return;
    }
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*)
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
    repaint();
    //update();
}

void MainWindow::stateChange(QString& newState)
{
    QTcpSocket *clientTcpSocket = (QTcpSocket*)sender();
    if ((state==newState) | ((state!="on") & (state!="off"))) {
        messageTo = "FAILED\n";
        sendToClient(clientTcpSocket,messageTo);
        return;
    }
    state=newState;
    if (state=="on") {
        if (rate!=0)    currentTimerId = startTimer(1000/rate);
    } else if (state=="off") {
        if (rate!=0)    killTimer(currentTimerId);
    }
    messageTo = "OK\n";
    sendToClient(clientTcpSocket,messageTo);
}

void MainWindow::colorChange(QString& newColor)
{
    QTcpSocket *clientTcpSocket = (QTcpSocket*)sender();
    if ((color==newColor) | ((newColor!="green") & (newColor!="blue") & (newColor!="red")) | (state=="off")) {
        messageTo = "FAILED\n";
        sendToClient(clientTcpSocket,messageTo);
        return;
    }
    color=newColor;
    repaint();
    messageTo = "OK\n";
    sendToClient(clientTcpSocket,messageTo);
}

void MainWindow::rateChange(QString& newRate)
{
    QTcpSocket *clientTcpSocket = (QTcpSocket*)sender();
    int nRate = newRate.toInt();
    if ((rate==nRate) | ((nRate>5) | (nRate<0)) | (state=="off")) {
        messageTo = "FAILED\n";
        sendToClient(clientTcpSocket,messageTo);
        return;
    }
    rate=nRate;
    killTimer(currentTimerId);
    if (rate!=0) currentTimerId = startTimer(1000/rate);
    messageTo = "OK\n";
    sendToClient(clientTcpSocket,messageTo);
}

void MainWindow::ServerOperations(QByteArray data)
{
    QString Line(data);
    QStringList dataList = Line.split(" ");
    bool tf=false;
    QTcpSocket* clientTcpSocket = (QTcpSocket*)sender();
    for (int i=0;i<Commands.size();i++)
        if (dataList[0] == Commands[i]) tf=true;
    if (!tf) {
        messageTo = "FAILED\n";
        sendToClient(clientTcpSocket,messageTo);
        return;
    }
    QStringList typeCommand = dataList[0].split("-");
    if (typeCommand[0]=="set") {
        dataList[1].chop(1);
        if (typeCommand[2]=="state")        stateChange(dataList[1]);
        else if (typeCommand[2]=="color")   colorChange(dataList[1]);
        else if (typeCommand[2]=="rate")    rateChange(dataList[1]);
    } else  if (typeCommand[0]=="get") {
        typeCommand[2].chop(1);
        if (typeCommand[2]=="state") {
            messageTo = "OK " + state + "\n";
            sendToClient(clientTcpSocket,messageTo);
        } else if (typeCommand[2]=="color") {
            messageTo = "OK " + color.name() + "\n";
            sendToClient(clientTcpSocket,messageTo);
        } else if (typeCommand[2]=="rate") {
            messageTo = "OK " + QString::number(rate) + "\n";
            sendToClient(clientTcpSocket,messageTo);
        } else {
            messageTo = "FAILED\n";
            sendToClient(clientTcpSocket,messageTo);
        }
    }
}

void MainWindow::sendToClient(QTcpSocket *clientTcpSocket, QString message)
{
    QByteArray  answer; answer.append(message);
    clientTcpSocket->write(answer);
}

void MainWindow::newConnection()
{
    QTcpSocket* clientTcpSocket = tcpServer->nextPendingConnection();
    connect(clientTcpSocket, SIGNAL(disconnected()), clientTcpSocket, SLOT(deleteLater()));
    connect(clientTcpSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
    sendToClient(clientTcpSocket, "Server Response: Connected!");
}

void MainWindow::readClient()
{
    QTcpSocket* clientTcpSocket = (QTcpSocket*)sender();
    if (clientTcpSocket->bytesAvailable() >= 2) {
        QByteArray data = clientTcpSocket->readAll();
        qDebug() << data;
        //if all data recieved
        ServerOperations(data);
    }
}

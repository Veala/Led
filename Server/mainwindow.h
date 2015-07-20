#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTcpServer *tcpServer;
    int nPort;

    QPainter painter;
    int currentTimerId;

    QString state;
    int rate;
    QColor color;

    QStringList Commands;
    QStringList Arguments[6];

    QString messageFrom;
    QString messageTo;

private:
    void sendToClient(QTcpSocket*, QString);

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent *);

public slots:
    void stateChange(QString&);
    void colorChange(QString&);
    void rateChange(QString&);

    void ServerOperations(QByteArray);
    void newConnection();
    void readClient();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

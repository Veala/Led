#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTcpSocket *clientTcpSocket;

    QStringList Commands;
    QStringList Arguments[6];

public slots:
    void changeArg(int);
    void connectClick();
    void readyRead   ();
    void error       (QAbstractSocket::SocketError);
    void sendToServer();
    void connected   ();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

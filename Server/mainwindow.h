#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPainter painter;
    QString state;
    int rate;
    QColor color;

    QStringList Commands;
    QStringList Arguments[6];

    QString messageFrom;
    QString messageTo;

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent *);

public slots:
    void ServerOperations(QString);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

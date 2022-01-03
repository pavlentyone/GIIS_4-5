#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QPoint>
#include <QMessageBox>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QRgb>
#include <QKeyEvent>
#include <QBrush>
#include <QPalette>
#include <QTime>
#include "dialog.h"
#include "about.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Dialog *dialogWindow;
    About *aboutWindow;

    QTime myTimer;
    int fifteenWidth;
    int fifteenHeight;
    int poorBlockNum;
    QString filePath;
    QImage currentImage;
    QImage currentBackground;
    QImage poorBlockImage;
    QVector<int> blockPosition;
    QVector<QLabel *> labels;
    bool isImageNow;
    bool isBackGroundNow;
    bool isWin;
    void on_actionShake_triggered();
    void on_actionLoadImage_triggered();
    void on_actionOrder_triggered();
    void on_actionStandartBlocks_triggered();
    void on_actionLoadBackground_triggered();
    void on_actionStandartBackground_triggered();
    void on_actionAbout_triggered();
    void fillBlocks();
    bool blockSwap(int index1, int index2);
    void checkWin();
protected:
    bool eventFilter(QObject *target, QEvent *event);
    void keyPressEvent(QKeyEvent *);
};

#endif // MAINWINDOW_H

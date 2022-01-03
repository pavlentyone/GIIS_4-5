#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QVector>
#include <QListView>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0, int secResult = 9999999999);
    ~Dialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Dialog *ui;
    QVector<QString> results;
};

#endif // DIALOG_H

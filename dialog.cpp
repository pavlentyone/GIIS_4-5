#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent, int secResult) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->lResult->setText(QString::number(secResult) + " sec");

    QString fileName = QFileDialog::getOpenFileName(this,
             tr("Open results book"), "",
             tr("All Files (*)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
            return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_5);
    results.empty();   // очищаем существующие контакты
    in >> results;
    if (results.isEmpty())
        QMessageBox::information(this, tr("No results in file"),
                    tr("your result will be first"));
    ui->twResults->setColumnCount(1);
    QStringList headerList;
    headerList.push_back("Name(result)");
    ui->twResults->setHorizontalHeaderLabels(headerList);
    for(int i = 0; i < results.length(); i++){
        ui->twResults->insertRow(i);
        QTableWidgetItem *twItem = new QTableWidgetItem;
        twItem->setText(results[i]);
        ui->twResults->setItem(i,0, twItem);
    }
    ui->twResults->resizeColumnsToContents();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if((QPushButton *)button == ui->buttonBox->button(QDialogButtonBox::Ok)){
        QString fileName = QFileDialog::getSaveFileName(this,
                 tr("Save result book"), "",
                 tr("All Files (*)"));
        if (fileName.isEmpty())
            return;
         else {
             QFile file(fileName);
             if (!file.open(QIODevice::WriteOnly)) {
                 QMessageBox::information(this, tr("Unable to open file"),
                     file.errorString());
                 return;
             }
             QString res;
             res = ui->leName->text() +" ("+ ui->lResult->text()+")";
             results.push_back(res);
             QDataStream out(&file);
             out.setVersion(QDataStream::Qt_4_5);
             out << results;
         }
    }
}


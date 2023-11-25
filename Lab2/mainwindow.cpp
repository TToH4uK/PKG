#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtWidgets/qheaderview.h"
#include "QtWidgets/qtablewidget.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QImageWriter>
#include <QList>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    ui->table->setDisabled(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Show() {
    ui->table->clearContents();
    ui->table->setRowCount(0);
    for(int i = 0, j = 0; i < img.size(); i++) {
        if(QImage(img[i]).format() != 0) {
            ui->table->insertRow(j);
            ui->table->setItem(j, 0, new QTableWidgetItem(img[i].right(img[i].size() - img[i].toStdString().find_last_of('/') - 1)));
            ui->table->setItem(j, 1, new QTableWidgetItem(QString::number(QImage(img[i]).size().rwidth()) + " x " + QString::number(QImage(img[i]).size().rheight())));
            ui->table->setItem(j, 2, new QTableWidgetItem(img[i].right(img[i].size() - img[i].toStdString().find_last_of('.') - 1)));
            ui->table->setItem(j, 3, new QTableWidgetItem(QString::number(QImage(img[i]).depth())));
            ui->table->setItem(j, 4, new QTableWidgetItem(QString::number(QImageWriter(img[i]).compression())));
            j++;
        }
    }
    this->show();
}

int findLast(QString &str, char ch) {
    int i = str.size() - 1;
    while (str[i] != ch) {
        if(i == 0) {
            break;
        }
        i--;
    }
    return i;
}



void MainWindow::on_ChooseFileButton_clicked() {
    QFileDialog *files = new QFileDialog;
    files->setWindowTitle("Откройте файлы");
    img = files->getOpenFileNames();
    Show();
}

template <typename T1, typename T2>

void Bubble(T1 &arr, T2 arr2) {
    for(int i = 0; i < arr2.size(); i++) {
        for(int j = 1; j < arr2.size(); j++) {
            if(arr2[j - 1] > arr2[j]) {
                std::swap(arr2[j], arr2[j - 1]);
                std::swap(arr[j], arr[j - 1]);
            }
        }
    }
}

void MainWindow::on_pushButton_clicked() {
    if(ui->table->currentColumn() == 0)
        Bubble(img, img);
    if(ui->table->currentColumn() == 1) {
        std::vector<int> buf(img.size());
        for (int i  = 0; i < img.size(); i++) {
            buf[i] = QImage(img[i]).size().rwidth() + QImage(img[i]).size().rheight();
        }
        Bubble(img, buf);
    }
    Show();
}

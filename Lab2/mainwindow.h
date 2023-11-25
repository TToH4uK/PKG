#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Show();

signals:
    //    void on_listView_doubleClicked(const QModelIndex &index);

private slots:
    void on_ChooseFileButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QDir qurentFolder_;
    QFileInfoList *aDirList;
    QStringList img;
};
#endif // MAINWINDOW_H

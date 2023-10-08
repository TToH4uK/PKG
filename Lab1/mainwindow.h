#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <string>
#include <QVBoxLayout>
#include <cmath>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    QVector<float> rgbToSrgb(int r, int g, int b);
    QVector<float> rgbToxyz(int r, int g, int b);
    QVector<float> rgbToLab(int r, int g, int b);
    QVector<int> LabToRgb(float l, float a, float b);

    float f(float x);
    float f2(float x);
    float g(float x);
    float g2(float x);

    void changeButtonColor();
private slots:
    void pushButtonClicked();
    void RGBChanged();
    void HUEChanged();
    void LABChanged();
private:
    Ui::MainWindow *ui;

    QColor *currentColor;
    QVBoxLayout *vLayout;

    bool buttonWasClicked;
    bool RGBWasChanged;
    bool HUEWasChanged;
    bool LABWasChanged;
};
#endif // MAINWINDOW_H

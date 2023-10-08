#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vLayout = new QVBoxLayout;
    vLayout->addWidget(ui->pushButton);
    buttonWasClicked = false;
    RGBWasChanged = false;
    HUEWasChanged = false;
    LABWasChanged = false;
    setFixedSize(800, 600);
    currentColor = new  QColor(Qt::black);
    ui->redText->setValue(0);
    ui->greenText->setValue(0);
    ui->blueText->setValue(0);
    ui->hueText->setValue(0);
    ui->saturationText->setValue(0);
    ui->valueText->setValue(0);
    ui->lText->setValue(0);
    ui->aText->setValue(0);
    ui->bText->setValue(0);
    std::string str = std::to_string(currentColor->red())+", "+std::to_string(currentColor->green())+", "+std::to_string(currentColor->blue());
    QString color = QString::fromStdString(str);
    ui->pushButton->setStyleSheet("background-color: rgb("+color+")");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<float> MainWindow::rgbToSrgb(int r, int g, int b)
{
    QVector<float> sRgb;
    sRgb.push_back(MainWindow::g((float)r/255));
    sRgb.push_back(MainWindow::g((float)g/255));
    sRgb.push_back(MainWindow::g((float)b/255));
    return sRgb;
}

QVector<float> MainWindow::rgbToxyz(int red, int green, int blue)
{
    QVector<float> xyz;
    QVector<float> srgb = rgbToSrgb(red, green, blue);
    float r = srgb[0]; float g = srgb[1]; float b = srgb[2];
    float temp;
    temp = 0.412453*r + 0.357580*g + 0.180423*b;
    xyz.push_back(temp);
    temp = 0.212671*r + 0.715160*g + 0.072169*b;
    xyz.push_back(temp);
    temp = 0.019334*r + 0.119193*g + 0.950227*b;
    xyz.push_back(temp);
    return xyz;
}

QVector<float> MainWindow::rgbToLab(int r, int g, int b)
{
    QVector<float> xyz = rgbToxyz(r, g, b);
    QVector<float> lab;
    float temp;
    temp = 116*f(xyz[1]) - 16;
    lab.push_back(temp);
    temp = 500*(f(xyz[0]/0.9642) - f(xyz[1]));
    lab.push_back(temp);
    temp = 200*(f(xyz[1]) - f(xyz[2]/1.08883));
    lab.push_back(temp);
    return lab;
}

QVector<int> MainWindow::LabToRgb(float l, float a, float b)
{
    QVector<int> rgb;
    float x = f2(a/500.0+(l+16.0)/116.0) * 95.047;
    float y = f2((l+16.0)/116.0) * 100.0;
    float z = f2((l+16.0)/116.0-b/200) * 108.883;
    double R = g2(3.2406 * x  - 1.5372 * y  - 0.4986 * z);
    if(round(R) > 255)
    {
        rgb.push_back(255);
    }
    else if(round(R) < 0)
    {
        rgb.push_back(0);
    }
    else
        rgb.push_back(round(R));
    double G = g2(-0.9689 * x + 1.8758 * y + 0.0415 * z);
    if(round(G) > 255)
    {
        rgb.push_back(255);
    }
    else if(round(G) < 0)
    {
        rgb.push_back(0);
    }
    else
        rgb.push_back(round(G));
    double B = g2(0.0557 * x - 0.2040 * y + 1.0570 * z);
    if(round(B) > 255)
    {
        rgb.push_back(255);
    }
    else if(round(B) < 0)
    {
        rgb.push_back(0);
    }
    else
        rgb.push_back(round(B));
    return rgb;
}

float MainWindow::f(float x)
{
    if(x>=0.008856)
    {
        return pow(x, 0.3333333);
    }
    return x*7.787+((float)4/29);
}

float MainWindow::f2(float x)
{
    if (pow(x,3)>=0.008856)
    {
        return pow(x,3);
    }
    return (x-16.0/116.0)/7.787;
}

float MainWindow::g(float x)
{
    if (x >= 0.04045)
    {
        return pow(((x + 0.055) / 1.055), 2.4);
    }
    return x/12.92;
}

float MainWindow::g2(float x)
{
    if (x > 0.0031308)
    {
        return 255 * (1.055 * pow((x / 255), (float)1 / 2.4) - 0.055);
    }
    return 12.92 * (x) * 255;
}

void MainWindow::changeButtonColor()
{
    std::string str = std::to_string(currentColor->red())+", "+std::to_string(currentColor->green())+", "+std::to_string(currentColor->blue());
    QString color = QString::fromStdString(str);
    ui->pushButton->setStyleSheet("background-color: rgb("+color+")");
}

void MainWindow::pushButtonClicked()
{
    buttonWasClicked = true;
    *currentColor = QColorDialog::getColor(*currentColor);
    changeButtonColor();
    ui->redText->setValue(currentColor->red());
    buttonWasClicked = true;
    ui->greenText->setValue(currentColor->green());
    buttonWasClicked = true;
    ui->blueText->setValue(currentColor->blue());
    buttonWasClicked = true;
    ui->hueText->setValue(currentColor->hue());
    buttonWasClicked = true;
    ui->saturationText->setValue(currentColor->saturation());
    buttonWasClicked = true;
    ui->valueText->setValue(currentColor->value());
    buttonWasClicked = true;
    ui->lText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[0]);
    buttonWasClicked = true;
    ui->aText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[1]);
    buttonWasClicked = true;
    ui->bText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[2]);
    buttonWasClicked = true;
}
void MainWindow::RGBChanged()
{
    if(!buttonWasClicked && !HUEWasChanged && !LABWasChanged)
    {
        RGBWasChanged = true;
        currentColor->setRgb(ui->redText->value(), ui->greenText->value(), ui->blueText->value());
        changeButtonColor();
        ui->hueText->setValue(currentColor->hue());
        RGBWasChanged = true;
        ui->saturationText->setValue(currentColor->saturation());
        RGBWasChanged = true;
        ui->valueText->setValue(currentColor->value());
        RGBWasChanged = true;
        ui->lText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[0]);
        RGBWasChanged = true;
        ui->aText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[1]);
        RGBWasChanged = true;
        ui->bText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[2]);
        RGBWasChanged = false;
    }
    buttonWasClicked = false;
}

void MainWindow::HUEChanged()
{
    if(!buttonWasClicked && !RGBWasChanged && !LABWasChanged)
    {
        HUEWasChanged = true;
        currentColor->setHsv(ui->hueText->value(), ui->saturationText->value(), ui->valueText->value());
        changeButtonColor();
        ui->redText->setValue(currentColor->red());
        HUEWasChanged = true;
        ui->greenText->setValue(currentColor->green());
        HUEWasChanged = true;
        ui->blueText->setValue(currentColor->blue());
        HUEWasChanged = true;
        ui->lText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[0]);
        HUEWasChanged = true;
        ui->aText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[1]);
        HUEWasChanged = true;
        ui->bText->setValue(rgbToLab(currentColor->red(), currentColor->green(),currentColor->blue())[2]);
        HUEWasChanged = false;
    }
    buttonWasClicked = false;
}

void MainWindow::LABChanged()
{
    if(!buttonWasClicked && !RGBWasChanged && !HUEWasChanged)
    {
        LABWasChanged = true;
        QVector<int> rgb = LabToRgb(ui->lText->value(), ui->aText->value(), ui->bText->value());
        currentColor->setRgb(rgb[0], rgb[1], rgb[2]);
        changeButtonColor();
        ui->redText->setValue(currentColor->red());
        LABWasChanged = true;
        ui->greenText->setValue(currentColor->green());
        LABWasChanged = true;
        ui->blueText->setValue(currentColor->blue());
        LABWasChanged = true;
        ui->hueText->setValue(currentColor->hue());
        LABWasChanged = true;
        ui->saturationText->setValue(currentColor->saturation());
        LABWasChanged = true;
        ui->valueText->setValue(currentColor->value());
        LABWasChanged = false;
    }
    buttonWasClicked = false;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    glWidget = new GLWidget;
    ui->verticalLayout_2->addWidget( glWidget );    
    connect( glWidget , SIGNAL( rotationsChanged() ), this, SLOT( updateRotations() ) );
    connect(ui->skinBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateSkin()));
    connect(ui->checkBox_anim,SIGNAL(clicked()),this,SLOT(animate()));
    connect(ui->checkBox_bg,SIGNAL(clicked()),this,SLOT(activateBackground()));
    connect(ui->checkBox_cn,SIGNAL(clicked()),this,SLOT(showNames()));
    connect(ui->checkBox_curve,SIGNAL(clicked()),this,SLOT(showCurve()));
    connect(ui->checkBox_earth,SIGNAL(clicked()),this,SLOT(showEarth()));
    connect(ui->checkBox_longlat,SIGNAL(clicked()),this,SLOT(showLines()));
    connect(ui->checkBox_texture,SIGNAL(clicked()),this,SLOT(showTexture()));
    connect(ui->checkBox_axis,SIGNAL(clicked()),this,SLOT(showAxis()));
    connect(ui->checkBox_light,SIGNAL(clicked()),this,SLOT(turnLignt()));
    connect(ui->latlonButton,SIGNAL(clicked()),this,SLOT(rotateEarth()));

    list << "Simple"<<"Elevation"<<"Politic";
    ui->skinBox->addItems(list);
}

void MainWindow::updateRotations()
{
    //ui->label_X->setNum(glWidget->getRotX());
    //ui->label_Y->setNum(glWidget->getRotY());
   //ui->label_Z->setNum(glWidget->getRotZ());
}

void MainWindow::rotateEarth()
{
    glWidget->rotateBy(ui->longitude->text().toFloat(),ui->latitude->text().toFloat(),0);
}

void MainWindow::updateSkin()
{
    glWidget->setSkin(ui->skinBox->currentIndex());
    glWidget->updateEarth();
}

void MainWindow::activateBackground()
{
    glWidget->activateBackground = false;
    if (ui->checkBox_bg->isChecked())
    {
        glWidget->activateBackground = true;
    }
    glWidget->updateEarth();
}

void MainWindow::showEarth()
{
    glWidget->showEarth = false;
    if (ui->checkBox_earth->isChecked())
    {
        glWidget->showEarth = true;
    }
    glWidget->updateEarth();
}

void MainWindow::showTexture()
{
    glWidget->activateTexture = false;
    if (ui->checkBox_texture->isChecked())
    {
        glWidget->activateTexture = true;
    }
    glWidget->updateEarth();
}

void MainWindow::showLines()
{
    glWidget->showLatLong = false;
    if (ui->checkBox_longlat->isChecked())
    {
        glWidget->showLatLong = true;
    }
    glWidget->updateEarth();
}

void MainWindow::showAxis()
{
    glWidget->showAxis = false;
    if (ui->checkBox_axis->isChecked())
    {
        glWidget->showAxis = true;
    }
    glWidget->updateEarth();
}

void MainWindow::showNames()
{
    glWidget->showCountryNames = false;
    if (ui->checkBox_cn->isChecked())
    {
        glWidget->showCountryNames = true;
    }
    glWidget->updateEarth();
}

void MainWindow::turnLignt()
{
    glWidget->activateLight = false;
    if (ui->checkBox_light->isChecked())
    {
        glWidget->activateLight = true;
    }
    glWidget->updateEarth();
}

void MainWindow::showCurve()
{
    glWidget->showCurve = false;
    if (ui->checkBox_curve->isChecked())
    {
        glWidget->showCurve = true;
    }
    glWidget->updateEarth();
}

void MainWindow::animate()
{
    glWidget->animate = false;
    if (ui->checkBox_anim->isChecked())
    {
        glWidget->startAnimation();
        glWidget->animate = true;
    }
    else
    {
        glWidget->StopAnimation();
    }
    glWidget->updateEarth();
}

MainWindow::~MainWindow()
{
    delete ui;
}

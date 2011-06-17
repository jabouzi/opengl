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
}

void MainWindow::updateRotations()
{
    ui->label_X->setNum(glWidget->getRotX());
    ui->label_Y->setNum(glWidget->getRotY());
    ui->label_Z->setNum(glWidget->getRotZ());
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QLineF line(0.0, 0.0, 100.0, 100.0);
	QPainter painter(this);
	painter.setPen(Qt::white);
	painter.drawLine(line);
}

MainWindow::~MainWindow()
{
    delete ui;
}

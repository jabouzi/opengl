#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->_visualizer = new Visualizer(this);

    this->setCentralWidget(this->_visualizer);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->_visualizer;
}

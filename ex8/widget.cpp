#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    glWidget = new GLWidget;
    //QVBoxLayout *mainLayout = new QVBoxLayout;
    ui->verticalLayout->addWidget(glWidget);
    //setLayout(mainLayout);
}

Widget::~Widget()
{
    delete ui;
}

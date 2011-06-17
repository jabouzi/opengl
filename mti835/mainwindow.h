#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> 
#include <QPainter>
#include "glwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GLWidget *glWidget;
    QStringList list;
    
protected:
    void paintEvent(QPaintEvent *); 

private slots:
     void updateRotations();
     void rotateEarth();
     void updateSkin();

};

#endif // MAINWINDOW_H

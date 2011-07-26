#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> 
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
    
private slots:
     void updateRotations();
     void rotateEarth();
     void updateSkin();
     void activateBackground();
     void showEarth();
     void showTexture();
     void showLines();
     void showAxis();
     void showNames();
     void turnLignt();
     void showCurve();
     void animate();

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QImage>
#include <QString>
#include <QFileDialog>

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "filtre.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void repeindre();
    bool open(QString url);
    void afficher(QImage* image);

    QImage* etiqueter();

public slots:
    bool openFilename();
    bool sauvegarder();
    bool gaussien();
    bool median();
    bool adaptatif();
    bool gradientX();
    bool gradientY();
    bool seuiller(int seuil);


private:
    Ui::MainWindow *ui;
    QImage* img;
    QString filename;
    QGraphicsScene *scene;
    QGraphicsPixmapItem itemPixmap;

    /*Actions pour boutons*/
    QAction *actionOuvrir;
    QAction *actionSauvegarder;
    QAction *actionQuitter;
    QAction *actionGauss;
    QAction *actionMedian;
    QAction *actionAdaptatif;
    QAction *actionGradientX;
    QAction *actionGradientY;
};

#endif // MAINWINDOW_H

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
#include "histogramme.h"
#include "geometrie.h"

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


public slots:
    bool openFilename();
    bool sauvegarder();

    bool gaussien();
    bool median();
    bool adaptatif();
    bool gradientX();
    bool gradientY();
    bool gradient();

    bool seuiller();
    bool negatif();
    bool etiqueter();
    bool compter();


    bool histo();
    bool inverser();
    bool etaler();

    bool pointsInteret();
    bool harris();
    bool correspondance();

    bool kMeansRouge();
    bool kMeansVert();
    bool kMeansBleu();
    bool kMeansRGB();


private:
    Ui::MainWindow *ui;
    QImage* img;
    QString filename;
    QGraphicsScene *scene;
    QGraphicsPixmapItem itemPixmap;
    Histogramme h;

    /*Actions pour boutons*/
    QAction *actionOuvrir;
    QAction *actionSauvegarder;
    QAction *actionQuitter;
    /**/
    QAction *actionGauss;
    QAction *actionMedian;
    QAction *actionAdaptatif;
    QAction *actionGradientX;
    QAction *actionGradientY;
    QAction *actionGradient;


    /**/
    QAction *actionPointsInteret;
    QAction *actionHarris;
    QAction *actionCorrespondance;

    /**/
    QAction *actionHisto;
    QAction *actionInverser;
    QAction *actionEtaler;

    /**/
    QAction *actionKMeansRouge;
    QAction *actionKMeansVert;
    QAction *actionKMeansBleu;
    QAction *actionKMeansRGB;


    /**/
    QAction *actionSeuiller;
    QAction *actionNegatif;
    QAction* actionEtiqueter;
    QAction* actionCompter;
};

#endif // MAINWINDOW_H

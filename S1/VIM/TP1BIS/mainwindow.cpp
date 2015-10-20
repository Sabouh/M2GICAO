#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = new QImage();
    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);


    /*Barre de menu*/
    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
    actionOuvrir = new QAction("&Ouvrir",this);
    actionSauvegarder = new QAction("&Sauvegarder",this);
    actionQuitter = new QAction("&Quitter", this);
    menuFichier->addAction(actionOuvrir);
    menuFichier->addAction(actionSauvegarder);
    menuFichier->addAction(actionQuitter);

    QMenu *menuFiltre =  menuBar()->addMenu("&Filtres");
    actionGauss = new QAction("&Gaussien",this);
    actionMedian = new QAction("&Median",this);
    actionAdaptatif = new QAction("&Adaptatif",this);
    actionGradientX = new QAction("&GradientX",this);
    actionGradientY = new QAction("&GradientY",this);
    menuFiltre->addAction(actionGauss);
    menuFiltre->addAction(actionMedian);
    menuFiltre->addAction(actionAdaptatif);
    menuFiltre->addAction(actionGradientX);
    menuFiltre->addAction(actionGradientY);

    /*connexion aux slots correspondants*/
    QObject::connect(actionOuvrir,SIGNAL(triggered()),this,SLOT(openFilename()));
    QObject::connect(actionSauvegarder,SIGNAL(triggered()),this,SLOT(sauvegarder()));
    QObject::connect(actionQuitter,SIGNAL(triggered()),this,SLOT(quitter()));

    QObject::connect(actionGauss,SIGNAL(triggered()),this,SLOT(gaussien()));
    QObject::connect(actionMedian,SIGNAL(triggered()),this,SLOT(median()));
    QObject::connect(actionAdaptatif,SIGNAL(triggered()),this,SLOT(adaptatif()));
    QObject::connect(actionGradientX,SIGNAL(triggered()),this,SLOT(gradientX()));
    QObject::connect(actionGradientY,SIGNAL(triggered()),this,SLOT(gradientY()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete img;
    delete scene;
}

void MainWindow::afficher(QImage* image){
    for(int i=0;img->width();i++){
        for(int j=0;j<img->height();j++){
            std::cout << "" << image->pixel(i,j) <<" ";
        }
         std::cout << " " << std::endl;
    }
}

void MainWindow::repeindre(){
    scene->clear();
    QPixmap *piximg = new QPixmap();
    piximg->convertFromImage(*img);
    scene->addPixmap(*piximg);
    scene->setSceneRect(0,0,img->width(),img->height());
    scene->update();

    ui->graphicsView->show();

}

bool MainWindow::open(QString url){
    if (img->load(url))
    {
        filename = url;
          if(img->format() <= QImage::Format_Indexed8){
              cout<<"ola"<<endl;
             *img = img->convertToFormat(QImage::Format_RGB32);
        }

        repeindre();
        return true;
    }
    return false;
}

bool MainWindow::openFilename(){
    QString filename = QFileDialog::getOpenFileName(this,
        "Ouvrir une image");
    if (filename != "")
    {
        return open(filename);
    }
    return false;
}

bool MainWindow::sauvegarder(){
    QString filename = QFileDialog::getSaveFileName(this, "Save File");
    if (filename != ""){
        return img->save(filename, 0, -1);
       }
    return false;
}

/*Filtres*/

bool MainWindow::gaussien(){
    Filtre f;
    img = f.gaussien(img,3);
    repeindre();
    return true;
}

bool MainWindow::median(){
    Filtre f;
    img = f.median(img,3);
    repeindre();
    return true;
}

bool MainWindow::adaptatif(){
    Filtre f;
    img = f.adaptatif(img,3);
    repeindre();
    return true;
}

bool MainWindow::gradientX(){
    Filtre f;
    img = f.gradientX(img,3);
    repeindre();
    return true;
}
bool MainWindow::gradientY(){
    Filtre f;
    img = f.gradientY(img,3);
    repeindre();
    return true;
}

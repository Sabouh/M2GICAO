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
    actionGradient = new QAction("&Gradient",this);
    menuFiltre->addAction(actionGauss);
    menuFiltre->addAction(actionMedian);
    menuFiltre->addAction(actionAdaptatif);
    menuFiltre->addAction(actionGradientX);
    menuFiltre->addAction(actionGradientY);
    menuFiltre->addAction(actionGradient);

    QMenu *menuMorpho =  menuBar()->addMenu("&Morpho");
    actionSeuiller = new QAction("&Seuiller",this);
    actionNegatif = new QAction("&Negatif",this);
    actionEtiqueter = new QAction("&Etiqueter",this);
    actionCompter = new QAction("&Compter",this);
    menuMorpho->addAction(actionSeuiller);
    menuMorpho->addAction(actionNegatif);
    menuMorpho->addAction(actionEtiqueter);
    menuMorpho->addAction(actionCompter);


    QMenu *menuHisto =  menuBar()->addMenu("&Histogramme");
    actionHisto = new QAction("&Histogramme",this);
    actionInverser = new QAction("&Inverser",this);
    actionEtaler = new QAction("&Etaler",this);
    menuHisto->addAction(actionHisto);
    menuHisto->addAction(actionInverser);
    menuHisto->addAction(actionEtaler);


    QMenu *menuPoints =  menuBar()->addMenu("&Detection");
    actionPointsInteret = new QAction("&PointsInteret",this);
    actionHarris = new QAction("&Harris",this);
    actionCorrespondance = new QAction("&Correspondance",this);
    menuPoints->addAction(actionPointsInteret);
    menuPoints->addAction(actionHarris);
    menuPoints->addAction(actionCorrespondance);

    QMenu *menuSegmentation = menuBar()->addMenu("&Segmentation");
    actionKMeansRouge = new QAction("&KMeansRouge",this);
    actionKMeansVert = new QAction("&KMeansVert",this);
    actionKMeansBleu = new QAction("&KMeansBleu",this);
    actionKMeansRGB = new QAction("&KMeansRGB",this);
    menuSegmentation->addAction(actionKMeansRouge);
    menuSegmentation->addAction(actionKMeansVert);
    menuSegmentation->addAction(actionKMeansBleu);
    menuSegmentation->addAction(actionKMeansRGB);

    /*connexion aux slots correspondants*/
            /*MENU FICHIER*/
    QObject::connect(actionOuvrir,SIGNAL(triggered()),this,SLOT(openFilename()));
    QObject::connect(actionSauvegarder,SIGNAL(triggered()),this,SLOT(sauvegarder()));
    QObject::connect(actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));

    /*MENU FILTRE*/

    QObject::connect(actionGauss,SIGNAL(triggered()),this,SLOT(gaussien()));
    QObject::connect(actionMedian,SIGNAL(triggered()),this,SLOT(median()));
    QObject::connect(actionAdaptatif,SIGNAL(triggered()),this,SLOT(adaptatif()));
    QObject::connect(actionGradientX,SIGNAL(triggered()),this,SLOT(gradientX()));
    QObject::connect(actionGradientY,SIGNAL(triggered()),this,SLOT(gradientY()));
    QObject::connect(actionGradient,SIGNAL(triggered()),this,SLOT(gradient()));


    /*MENU HISTO*/
    QObject::connect(actionHisto,SIGNAL(triggered()),this,SLOT(histo()));
    QObject::connect(actionInverser,SIGNAL(triggered()),this,SLOT(inverser()));
    QObject::connect(actionEtaler,SIGNAL(triggered()),this,SLOT(etaler()));

    /*MENU POINTS*/
    QObject::connect(actionHarris,SIGNAL(triggered()),this,SLOT(harris()));
    QObject::connect(actionPointsInteret,SIGNAL(triggered()),this,SLOT(pointsInteret()));
    QObject::connect(actionCorrespondance,SIGNAL(triggered()),this,SLOT(correspondance()));

    /*MENU SEGMENTATION*/
    QObject::connect(actionKMeansRouge,SIGNAL(triggered()),this,SLOT(kMeansRouge()));
    QObject::connect(actionKMeansVert,SIGNAL(triggered()),this,SLOT(kMeansVert()));
    QObject::connect(actionKMeansBleu,SIGNAL(triggered()),this,SLOT(kMeansBleu()));
    QObject::connect(actionKMeansRGB,SIGNAL(triggered()),this,SLOT(kMeansRGB()));


    /*MENU MORPHO*/
    QObject::connect(actionSeuiller,SIGNAL(triggered()),this,SLOT(seuiller()));
    QObject::connect(actionNegatif,SIGNAL(triggered()),this,SLOT(negatif()));
    QObject::connect(actionEtiqueter,SIGNAL(triggered()),this,SLOT(etiqueter()));
    QObject::connect(actionCompter,SIGNAL(triggered()),this,SLOT(compter()));

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

    h.histo(img);
    h.val();

    ui->graphicsView->show();

}

bool MainWindow::open(QString url){
    if (img->load(url))
    {
        filename = url;
          if(img->format() <= QImage::Format_Indexed8){
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

bool MainWindow::gradient(){
    Filtre f;
    img = f.gradient(img,3);
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

bool MainWindow::pointsInteret(){
    Filtre f;
    img = f.pointsInteret(img,0.04,2000);
    repeindre();
    return true;
}

bool MainWindow::harris(){
    Filtre f;
    img = f.harris(img,0.04);
    repeindre();
    return true;
}

bool MainWindow::correspondance(){
    Filtre f;
    QImage *img1 = new QImage();
    QImage *img2 = new QImage();
    img1->load("../detectionPointsImage/cars/img_7019.ppm");
    *img1 = img1->convertToFormat(QImage::Format_RGB32);
    img2->load("../detectionPointsImage/cars/img_7021.ppm");
    *img2 = img2->convertToFormat(QImage::Format_RGB32);
    img = f.correspondance(img1,img2,500);
    repeindre();
    return true;
}

bool MainWindow::kMeansRouge(){

    Filtre f;
    img = f.kMeans(img,3,5,1);
    repeindre();
    return true;
}

bool MainWindow::kMeansVert(){

    Filtre f;
    img = f.kMeans(img,3,5,2);
    repeindre();
    return true;
}

bool MainWindow::kMeansBleu(){

    Filtre f;
    img = f.kMeans(img,3,5,3);
    repeindre();
    return true;
}

bool MainWindow::kMeansRGB(){

    Filtre f;
    img = f.kMeans(img,7,3,4);
    repeindre();
    return true;
}

/*Morpho*/
bool MainWindow::seuiller(){
    Filtre f;
    img = f.seuiller(img,100);
    repeindre();
    return true;
}

bool MainWindow::compter(){
    Filtre f;
    Geometrie g;
    QImage *img1,*img2,*img3;
    int seuil1 = 10;
    int seuil2 = 50;
    int seuil3 = 100;
    img1 = f.seuiller(img,seuil1);
    img2 = f.seuiller(img,seuil2);
    img3 = f.seuiller(img,seuil3);
    img = g.compterElements(img,img1,img2,img3,seuil1,seuil2,seuil3);
    repeindre();
    return true;

}

bool MainWindow::negatif(){
    Filtre f;
    img = f.negatif(img);
    repeindre();
    return true;
}

/*histogramme*/
bool MainWindow::inverser(){
    img = h.inverser(img);
    repeindre();
    return true;
}

bool MainWindow::etaler(){
    img = h.etalement(50,123,img);
    repeindre();
    return true;
}

bool MainWindow::etiqueter(){
    Geometrie g;
    img = g.etiqueter(img,4);
    repeindre();
    return true;
}

bool MainWindow::histo(){
    h.histo(img);
    h.show();
    return false;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = new QImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::open(QString url)
{
    if (img->load(url))
    {
        filename = url;
        if(img->format() <= QImage::Format_Indexed8){
            img->convertToFormat(QImage::Format_RGB32);
        }
        return true;
    }
    return false;
}

bool MainWindow::openFilename()
{
    QString filename = QFileDialog::getOpenFileName(this,
        "Ouvrir une image", QDir::currentPath() + "/../Projet-Image-M1-info/ressources", "Image Files (*.png *.jpg *.gif)");
    if (filename != "")
    {
        return open(filename);
    }
    return false;
}

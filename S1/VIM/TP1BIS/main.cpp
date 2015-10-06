#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    FILE* ifp;

      /* Test des arguments */
//      if ( argc != 2 ){
//        printf("\nUsage : gaussToPgm file \n\n");
//        exit(0);
//      }
      /*On crée la matrice correspondant à l'image*/
    //  Matrice mat(ifp);

    //On ouvre l'image
    //On la convertit en une QPixMap
    //On l'affiche ds la fenêtre

    return a.exec();
}

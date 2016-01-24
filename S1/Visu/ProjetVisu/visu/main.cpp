#include "mainwindow.h"
#include <QApplication>
/*fichiers inclus*/
#include "parseur.h"
#include <eigen3/Eigen/>
using namespace std;


/*lib incluses*/
#include<QMessageBox>

/* renvoie un Point dans les coordonnées respectives sont
 * (longitude, latitude et altitude) minimum des points dans lP
 */
Point minimum(QVector<Point> lP){

    if(lP.length() == 0){
        cout <<"ERREUR la liste de points est vide !! (fct minimum)"<<endl;
        return Point();
    }

    float xmin = (lP.at(0)).getX();
    float ymin = lP.at(0).getY();
    float zmin = lP.at(0).getZ();
    for(int i=0;i<lP.length();i++){
        if(lP.at(i).getX() < xmin){
            xmin = lP.at(i).getX();
        }
        if(lP.at(i).getY() < ymin){
            ymin = lP.at(i).getY();
        }
        if(lP.at(i).getZ() < zmin){
            zmin = lP.at(i).getZ();
        }
    }

    return Point(xmin,ymin,zmin);

}

/* renvoie un Point dans les coordonnées respectives sont
 * (longitude, latitude et altitude) maximum des points dans lP
 */
Point maximum(QVector<Point> lP){

    if(lP.length() == 0){
        cout <<"ERREUR la liste de points est vide !! (fct maximum)"<<endl;
        return Point();
    }

    float xmax = (lP.at(0)).getX();
    float ymax = lP.at(0).getY();
    float zmax = lP.at(0).getZ();
    for(int i=0;i<lP.length();i++){
        if(lP.at(i).getX() > xmax){
            xmax = lP.at(i).getX();
        }
        if(lP.at(i).getY() > ymax){
            ymax = lP.at(i).getY();
        }
        if(lP.at(i).getZ() > zmax){
            zmax = lP.at(i).getZ();
        }
    }

    return Point(xmax,ymax,zmax);
}

void Shepard(QVector<Point> stations, QVector<float> donnees){
    Point pmin = minimum(stations);
    Point pmax = maximum(stations);

}

void Hardy(QVector<Point> stations, QVector<float> donnees){

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*-------------LECTURE DES DONNEES---------------------------*/
    QFile stations("../Donnees/stations.csv");
    if(!stations.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", stations.errorString());
    }
    parseur stat;
    cout <<"stations parse"<<endl;
    stat.parse(&stations);

    QFile donnees("../Donnees/synop.2015110912.csv");
    if(!donnees.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", donnees.errorString());
    }
    parseur data;
    cout <<"donnees parse"<<endl;
    data.parse(&donnees);
    /*-----------------------------------------------------------*/


    /*-----------------------INTERPOLATION-----------------------*/

    cout <<"interpolation"<<endl;

    QVector<Point> listeP = stat.listePoints();
    //ici temperature, on peut changer comme on veut
    QVector<float> listeD = data.listeDonnees("t");

    Shepard(listeP,listeD);
    //Hardy(listeP,listeD);
    /*-----------------------------------------------------------*/


    /*--------------------COURBES ISO VALEURS--------------------*/



    /*-----------------------------------------------------------*/



    /*-------------------EXPORTATION FORMAT KML------------------*/


    /*-----------------------------------------------------------*/


    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include<QMessageBox>
#include <QPainter>
#include <iostream>
#include <sstream>
#include <string>

#include "parseur.h"
#include "cellule.h"
#include <eigen3/Eigen/Dense>
#include <limits>

using namespace std;
using namespace Eigen;


/**/
//On va utiliser une grille de taille N*N
int N = 200;
//mu contrôle la forme de l'interpolant , mu >= 1, par défaut = 2
int mu = 2;
float distx,disty, minD,maxD;
Point pmin,pmax;

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

/*Interpolation de Shepard*/
/*On renvoie les valeurs interpolées sous forme d'une matrice NxN*/
QVector<QVector<float> > shepard(QVector<Point> stations, QVector<float> donnees){
    QVector<QVector<float> > donnees_interpolees;
    QVector<float> ligne;
    QVector<Point> nouveaux_points;

    pmin = minimum(stations);
    pmax = maximum(stations);
    /*Sans tenir compte de l'altitude*/
    //Wi(X) = (1/d(X))² / Somme( 1/(d(X))²)
    //X = dist (x,xi)²
    distx = pmax.getX() -pmin.getX();
    disty = pmax.getY() -pmin.getY();
    float pasx = (pmax.getX() -pmin.getX() )/N;
    float pasy = (pmax.getY() -pmin.getY())/N;
    float xi,yi;
    float somme,poids,res ;
    //On parcourt les points de la grille
    //
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            xi = pmin.getX() + i*pasx;
            yi = pmin.getY() + j*pasy;

            somme =0;
            /*On calcule la sommee de l'inverse des distances à chaque stations pour le point (xi,xj) */
            for(int k=0;k<stations.length();k++){
                somme = somme + ( 1/pow(pmin.distance2(stations.at(k), Point(xi,yi,0)), mu) );
            }

            //F(X) = somme(wi(X) fi);
            poids = 0;
            res = 0;
            for(int k=0;k<stations.length();k++){
                //wi(X)
                poids = ( 1/pow( pmin.distance2(stations.at(k), Point(xi,yi,0)), mu) ) / somme ;
                //fi = donnees.at(k)
                res += poids*donnees.at(k);
            }
            //on sauvegarde la nouvelle valeur
            ligne.push_back(res);
            //on sauvegarde le point associé
            nouveaux_points.push_back(Point(xi,yi,0));

        }
        donnees_interpolees.push_back(ligne);
    }
    return donnees_interpolees;
}



/*Interpolation Hardy*/
void hardy(QVector<Point> stations, QVector<float> donnees){
    MatrixXf A = MatrixXf::Zero(donnees.length(),donnees.length());
    VectorXf b = VectorXf(donnees.length());
    VectorXf x;
}

/*dessine ligne iso*/
void dessinerLignesIso(QVector<QVector<Cellule> > grille,QImage* img, float lambda){

    img->fill(qRgb(0,0,0));
    QPainter* painter = new QPainter(img);
    painter->setPen(Qt::blue);

    /*representation graphique DU quadrillage, ici une cellule est de taille TxT*/
    int T = 10;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            painter->drawLine(i*T,j*T, i*T,(j+1)*T);
            painter->drawLine(i*T,j*T, (i+1)*T,j*T);
        }
    }
    painter->setPen(Qt::white);

    painter->setPen(Qt::white);
    for(int i=0;i<grille.length()-1;i++){
        for(int j=0;j<grille.at(i).length()-1;j++){

            Point p1,p2,p3,p4;
            float a1, a2, a3, a4;
            Cellule c = grille.at(i).at(j);
            a1 = (lambda -c.x )/(c.y - c.x);
            a2 = (lambda -c.y )/(c.z - c.y);
            a3 = (lambda -c.z )/(c.w - c.z);
            a4 = (lambda -c.w )/(c.x - c.w);
            p1 = Point(i*T,(j+a1)*T,0); //xy
            p2 = Point((i+a2)*T, (j+1)*T, 0);  //yz
            p3 = Point((i+1)*T,(j+a3)*T,0);  //zw
            p4 = Point((i+a4)*T,j*T,0);  //wx
            int conf = c.config;

            if(conf == 0 || conf ==15){
                painter->setPen(Qt::white);

            }else if((conf %3) == 0){
                if(conf == 3 || conf == 12){
                        painter->setPen(Qt::white);
                        painter->drawLine(p4.getX(),p4.getY(),p2.getX(),p2.getY() );
                }else{
                    painter->setPen(Qt::white);
                    painter->drawLine(p1.getX(),p1.getY(),p3.getX(),p3.getY());
                }

            }else if((conf % 5) == 0){
                //CAS PARTICULIER
                float moy = (c.x +c.y + c.z + c.w )/4;
                if(conf == 5){
                    if(moy > lambda){
                        painter->drawLine(p1.getX(),p1.getY(),p4.getX(),p4.getY());
                    }else{
                        painter->drawLine(p2.getX(),p2.getY(),p3.getX(),p3.getY());

                    }
                }else{
                    if(moy <= lambda){
                        painter->drawLine(p1.getX(),p1.getY(),p4.getX(),p4.getY());
                    }else{
                        painter->drawLine(p2.getX(),p2.getY(),p3.getX(),p3.getY());

                    }

                }
            }else{
                if(conf == 14 || conf == 1){
                    painter->setPen(Qt::white);
                    painter->drawLine(p1.getX(),p1.getY(),p4.getX(),p4.getY());


                }
                if(conf ==  13 || conf == 2){
                    painter->setPen(Qt::white);
                    painter->drawLine(p1.getX(),p1.getY(),p2.getX(),p2.getY());

                }
                if(conf == 11 || conf ==4){
                    painter->setPen(Qt::white);
                    painter->drawLine(p2.getX(),p2.getY(),p3.getX(),p3.getY());

                }
                if(conf == 7 || conf == 8){
                    painter->setPen(Qt::white);
                    painter->drawLine(p3.getX(),p3.getY(),p4.getX(),p4.getY());

                }
            }
        }
    }
    painter->end();
}


/*Marching Square*/
void marchingSquare( QVector<QVector<float> > valeurs, float lambda,QImage *img){

    //On veut reconstruire les courbes isovaleurs

    //pour les 4 cellules : égale à 1 si < lambda et 0 sinon
    //resp HG,HD , BD , BG
    int coin1,coin2,coin3,coin4;
    //config
    int config;

    //grille
    QVector<QVector<Cellule> > grille;
    //On regarde à chaque fois 4 cellules
    for(int i=0;i<N-1;i++){
        QVector< Cellule > ligne;
        for(int j=0;j<N-1;j++){

            coin1 = (valeurs.at(i).at(j) < lambda);
            coin2 = (valeurs.at(i).at(j+1) < lambda);
            coin3 = (valeurs.at(i+1).at(j+1) < lambda);
            coin4 = (valeurs.at(i+1).at(j) < lambda);

            ligne.push_back( Cellule(valeurs.at(i).at(j), valeurs.at(i).at(j+1), valeurs.at(i+1).at(j+1), valeurs.at(i+1).at(j),config) );
            config = coin1 + 2*coin2 + 4*coin3 + 8*coin4;
        }
        grille.push_back(ligne);
    }

    dessinerLignesIso(grille,img,lambda);

}

int main(int argc, char *argv[])
{

    /*-------------LECTURE DES DONNEES---------------------------*/

    QFile donnees("../Donnees/synop.2015110912.csv");
    if(!donnees.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", donnees.errorString());
    }
    parseur data;
    cout <<"donnees parse"<<endl;
    data.parse(&donnees);


    QFile stations("../Donnees/stations.csv");
    if(!stations.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", stations.errorString());
    }
    parseur stat;
    cout <<"stations parse"<<endl;
    stat.parse(&stations);
    /*-----------------------------------------------------------*/


    /*-----------------------INTERPOLATION-----------------------*/

    cout <<"interpolation"<<endl;

    QVector<Point> listeP = stat.listePoints(data);
    //ici temperature, on peut changer comme on veut
    QVector<float> listeD = data.listeDonnees("t");

    QVector<QVector<float> > interpolation = shepard(listeP,listeD);
    //hardy(listeP,listeD);
    /*-----------------------------------------------------------*/


    /*--------------------COURBES ISO VALEURS--------------------*/


    QImage *img = new QImage(N,N,QImage::Format_RGB32);
    /*On cherche les extremas des donnees*/
    minD = numeric_limits<float>::max();
    maxD = numeric_limits<float>::min();
    for(int i=0;i<listeD.length();i++){
        if(listeD.at(i) < minD){
            minD = listeD.at(i);
        }
        if(listeD.at(i)> maxD){
            maxD = listeD.at(i);
        }

    }
    float pas = (maxD-minD) / listeD.length();
    int i=0;
    ostringstream oss;
    //QImage *colorMap = new QImage(N,N,QImage::Format_RGB32);

    for(float lambda = minD;lambda<=maxD;lambda+=pas){
        marchingSquare(interpolation,lambda,img);
        ostringstream oss;

        oss << "../CompteRendu/image_";
        oss<<i;
        oss<<".png";
        string s = oss.str();
        QString st = QString::fromStdString(s);
        img->save(st);
        img->save("../CompteRendu/colorMap.png");
        QImage *img = new QImage(N,N,QImage::Format_RGB32);
        i++;

    }

    cout <<"FINI"<<endl;
    //marchingCube

    /*-----------------------------------------------------------*/



    /*---------------------EXPORT FORMAT KML---------------------*/


    /*-----------------------------------------------------------*/


}

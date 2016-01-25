#include "parseur.h"
using namespace std;

parseur::parseur()
{

}

QVector<QStringList> parseur::getDonnees(){
    return donnees;
}

QStringList parseur::getCles(){
    return cles;
}

void parseur::afficherQStringList(QStringList s){
    for(int i=0 ; i < s.length() ; i++){;
         cout << s.at(i).toStdString()<<endl;
     }
}

void parseur::parse(QFile *f){
    QTextStream in(f);

    /*On parse les clés*/
    QString ligne = in.readLine();
    cles = ligne.split(";");

    QStringList liste;
    while(!in.atEnd()){

        //on recupere la ligne sous forme d'une QStringList
        ligne = in.readLine();

        //on convertit la ligne en QStringList
        liste = ligne.split("\n");

        //On sauvegarde les données pour chaque station
        donnees.push_back(liste);

    }

    //On ferme le fichier
    f->close();


}

void parseur::join(parseur p1, parseur p2){


    //QStringList test= liste.filter("2 ");


}

/*ATTENTION SEULEMENT UTILISABLE SUR UN PARSEUR DE STATIONS*/
/*renvoie vecteur contenant la position de chaque station*/
QVector<Point> parseur::listePoints(parseur data){
    //On sait que latitude = position  2, longitude = position 3 , altitude = position 4
    QVector<Point> coordonnees;
    QStringList s;
    QString sl,stat ;

    int j=0;
    //On parcourt les donnees
    for(int i=0;i<data.donnees.length();i++){
        //On recupere la ligne i
        sl = data.donnees.at(i).at(0);
        s = sl.split(";");

        while(j<donnees.length() && ( (donnees.at(j).filter(s.at(0))).isEmpty()    )){
            j++;
        }

        sl = donnees.at(j).at(0);
        s = sl.split(";");

        coordonnees.push_back( Point( s.at(2).toFloat(), s.at(3).toFloat(), s.at(4).toFloat() ) );
    }
    return coordonnees;

}

/*ATTENTION SEULEMENT UTILISABLE SUR UN PARSEUR DE DATA*/
/*renvoie donnees d'un certain type pour chaque fonction*/
QVector<float> parseur::listeDonnees(string t){
    //On cherche l'index de la donnée qu'on veut
    //On suppose une donnée par station
    QVector<float> releves;
    QString sl;
    QStringList s;

    int k=0;

    while( k <  cles.length() && (cles.at(k).toStdString() != t)){
        k++;
    }
    if(k >= cles.length()){
        cout <<"ERREUR la clé n'a pas été trouvée (fonction listeDonnees fichier parseur.cpp)"<<endl;
        return releves;
    }

    //cout << cles.at(k).toStdString()<<endl;
    for(int i=0;i<donnees.length();i++){
        sl = donnees.at(i).at(0);
        s = sl.split(";");
        if(s.at(k).toStdString() == "mp"){
            releves.push_back(0);

        }else{
            releves.push_back(s.at(k).toFloat());
        }
    }
    return releves;
}

#ifndef PARSEUR_H
#define PARSEUR_H
#include <boost/tokenizer.hpp>
//#include <QxtCsvModel>
#include <iostream>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QVector>

#include "point.h"

class parseur
{
public:
    /*constructeur*/

    parseur();

    /*getters setters*/

    QStringList getCles();
    QVector<QStringList> getDonnees();

    /*fonctions*/

    void parse(QFile *f);
    void afficherQStringList(QStringList s);
    void join(parseur p1, parseur p2);
    QVector<Point> listePoints();
    QVector<float> listeDonnees(string t);

private:
    QString ligne;
    QStringList cles;
    QStringList liste;
    QVector<QStringList> donnees;
//public slots:

};

#endif // PARSEUR_H

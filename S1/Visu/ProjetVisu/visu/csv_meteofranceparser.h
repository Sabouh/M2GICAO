#ifndef CSV_METEOFRANCEPARSER_H
#define CSV_METEOFRANCEPARSER_H

#include <QString>
#include <map>
#include <vector>
#include <string>

typedef std::map<std::string,std::vector<std::string>*>::iterator csv_iterator;

class Csv_meteoFranceParser
{
public:
    Csv_meteoFranceParser(QString* filename);
    Csv_meteoFranceParser(std::map<std::string,std::vector<std::string>*>* infoMap);
    //template<int T>/*std::map<T,int>**/void* getAssocKeyFast(std::string upperKey,/*T(*funConvers)(std::string)*/void* funConv);
             /*nom de l'identifiant sensé etre unique,position dans le vecteur pour chaque clef*/
    std::map<int,int>* getAssocKeyFast(std::string upperKey,int(*funConvers)(std::string));
    std::map<std::string,int>* getAssocKeyFast(std::string upperKey);
    std::vector<std::string>* operator [](std::string key);

    bool keyExist(std::string key);
    std::vector<std::string>* keys();
    csv_iterator begin();
    csv_iterator end();
    void showMap();

private:
    std::map<std::string,std::vector<std::string>*> infoMap;//les clef sont ID,Nom,Latitute,Longitude,Altitude etc
    //chacune de ces clef renvoie vers un vecteur d'élément de ce type uniformisé sous la forme d'un vecteur de string infoMap["ID"][5] est le numero id de la ville infoMap["Nom"][5] ...
    void parse(QString* filename);
    std::vector<std::string> *parseLine(QString *str);

    void cloneMap(std::map<std::string,std::vector<std::string>*>* m);
};




Csv_meteoFranceParser* csv_join(Csv_meteoFranceParser* map1,Csv_meteoFranceParser* map2,std::string clef_map1,std::string clef_map2,std::string keyNewName);
//on assertie qu'il y ait autant d'éléments clefs et qu'ils sont tous identiques dans les deux maps. Si une clef autre que les clef utilisé pour join ont le même nom, une erreur est généré


Csv_meteoFranceParser* csv_join_force(Csv_meteoFranceParser* map1,Csv_meteoFranceParser* map2,std::string clef_map1,std::string clef_map2,std::string keyNewName);
//le map de sortie ne posséde pas une ligne si sa clef n'était présent que dans un des deux map

#endif // CSV_METEOFRANCEPARSER_H

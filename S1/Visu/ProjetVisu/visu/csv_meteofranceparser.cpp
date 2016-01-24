#include "csv_meteofranceparser.h"
#include <QFile>
#include <QTextStream>
#include <stdio.h>
#include <iostream>
#include <assert.h>

typedef void*(*funConvT)(std::string);

Csv_meteoFranceParser::Csv_meteoFranceParser(std::map<std::string,std::vector<std::string>*>* infoMap)
{
    this->infoMap=std::map<std::string,std::vector<std::string>*>();
    this->cloneMap(infoMap);
}

bool Csv_meteoFranceParser::keyExist(std::string key)
{
    return this->infoMap.find(key)!=this->infoMap.end();
}

std::vector<std::string>* Csv_meteoFranceParser::operator [](std::string key)
{
    assert(this->keyExist(key));
    return this->infoMap[key];
}


std::map<int,int>* Csv_meteoFranceParser::getAssocKeyFast(std::string upperKey,int(*funConvers)(std::string))
{
    /*assert upperKey in dico*/
    assert(this->infoMap.find(upperKey)!=this->infoMap.end());

    std::map<int,int>* retourMap=new std::map<int,int>();
    std::vector<std::string>* innerMapElements=this->infoMap[upperKey];
    int localTransformedElement;
    for (int i=0;i<innerMapElements->size();i++)
    {
        localTransformedElement=funConvers(innerMapElements->at(i));
        assert(retourMap->find(localTransformedElement)==retourMap->end());
        /*assert tous elements avec x!=y funConvers(dico[uk][x])!=funConvers(dico[uk][y])*/
        (*retourMap)[localTransformedElement]=i;
    }
    return retourMap;
}

std::map<std::string,int>* Csv_meteoFranceParser::getAssocKeyFast(std::string upperKey)
{
    assert(this->keyExist(upperKey));

    std::map<std::string,int>* retourMap=new std::map<std::string,int>();
    std::vector<std::string>* innerMapElements=this->infoMap[upperKey];
    std::string localTransformedElement;
    for (int i=0;i<innerMapElements->size();i++)
    {
        localTransformedElement=innerMapElements->at(i);
        assert(retourMap->find(localTransformedElement)==retourMap->end());
        /*assert tous elements avec x!=y funConvers(dico[uk][x])!=funConvers(dico[uk][y])*/
        (*retourMap)[localTransformedElement]=i;
    }
    return retourMap;
}

void Csv_meteoFranceParser::showMap()
{
    std::string str_tmp;
    std::vector<std::string>* elements;
    for(std::map<std::string,std::vector<std::string>*>::iterator i=this->infoMap.begin();i!=this->infoMap.end();i++)
    {
        str_tmp=i->first;
        elements=this->infoMap[str_tmp];
        printf("%s:\n",str_tmp.c_str());
        for (int j=0;j<elements->size();j++)
        {
            printf("\t%d->%s\n",j,elements->at(j).c_str());
        }
    }
}

Csv_meteoFranceParser::Csv_meteoFranceParser(QString* filename)
{
    infoMap=std::map<std::string,std::vector<std::string>*>();
    this->parse(filename);
}


std::vector<std::string>* Csv_meteoFranceParser::parseLine(QString* str)
{
    std::vector<std::string>* ret=new std::vector<std::string>();
    QString* strTmp=new QString();
    for(int i=0;i<str->size();i++)
    {
        if(str->at(i).toLatin1()/*toAscii()*/==';')
        {
            ret->push_back(strTmp->toStdString());//comment ça se passe en memoire pour cette string ?
            strTmp->clear();
        }
        else
        {
            strTmp->push_back(str->at(i));
        }
    }
    ret->push_back(strTmp->toStdString());
    delete(strTmp);
    return ret;
}

void Csv_meteoFranceParser::parse(QString* filename)
{
    QFile file(*filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    std::vector<std::string>* vect;
    std::vector<std::string>* internLineVect;
    QString line=in.readLine();
    std::string str;
    /*traitement des clef*/
    vect=this->parseLine(&line);

    //test keys
    for(int i=0;i<vect->size();i++)
    {
        str=(vect->at(i));
        //printf("%s\n",str.c_str());
    }
    //end test keys
    //init vector map
    for(int i=0;i<vect->size();i++)
    {
        str=(vect->at(i));
        this->infoMap[str]=new std::vector<std::string>();
    }

    /*traitement des valeurs jusqu'à la fin du fichier*/

    while (!in.atEnd())
    {
        line = in.readLine();
        //printf("%s\n",line.toStdString().c_str());
        internLineVect=this->parseLine(&line);
        assert(internLineVect->size()==vect->size());
        for(int i=0;i<vect->size();i++)
        {
            str=(vect->at(i));
            (this->infoMap[str])->push_back(internLineVect->at(i));
        }
        delete(internLineVect);


    }
}

std::vector<std::string>* cloneVector(std::vector<std::string>* inVect)
{
    std::vector<std::string>* retVect=new std::vector<std::string>();
    for(unsigned int i=0;i<inVect->size();i++)
    {
        retVect->push_back((*inVect)[i]);
    }
    return retVect;
}

void Csv_meteoFranceParser::cloneMap(std::map<std::string,std::vector<std::string>*>* m)
{
    std::string str_tmp;
    for(std::map<std::string,std::vector<std::string>*>::iterator i=m->begin();i!=m->end();i++)
    {
        str_tmp=i->first;
        this->infoMap[str_tmp]=cloneVector((*m)[str_tmp]);
    }
}

csv_iterator Csv_meteoFranceParser::begin()
{
    return this->infoMap.begin();
}

csv_iterator Csv_meteoFranceParser::end()
{
    this->infoMap.end();
}

std::vector<std::string>* Csv_meteoFranceParser::keys()
{
    std::string strLoc;
    std::vector<std::string>* output=new std::vector<std::string>();
    for(csv_iterator i=this->infoMap.begin();i!=this->infoMap.end();i++)
    {
        strLoc=i->first;
        output->push_back(strLoc);
    }
    return output;
}

bool assert_all_key_map1_in_map2(Csv_meteoFranceParser* map1,Csv_meteoFranceParser* map2,std::string clef_map1,std::string clef_map2)
{
    std::cerr<< "assert_all_key_map1_in_map2 in csv_meteofranceparser.cpp" <<std::endl;
    return true;
}

bool assert_all_key_map2_in_map1(Csv_meteoFranceParser* map1,Csv_meteoFranceParser* map2,std::string clef_map1,std::string clef_map2)
{
    std::cerr<< "assert_all_key_map2_in_map1 in csv_meteofranceparser.cpp" <<std::endl;
    return true;
}

bool assert_no_keys_with_same_name(Csv_meteoFranceParser* map1,Csv_meteoFranceParser* map2,std::string exeption_map1,std::string exeption_map2,std::string newKeyName)
{
    std::cerr<< "assert_no_keys_with_same_name in csv_meteofranceparser.cpp" <<std::endl;
    return true;
}

Csv_meteoFranceParser* csv_join(Csv_meteoFranceParser* map1,Csv_meteoFranceParser* map2,std::string clef_map1,std::string clef_map2,std::string keyNewName)
{
    assert(map1->keyExist(clef_map1));
    assert(map2->keyExist(clef_map2));
    assert(assert_all_key_map1_in_map2(map1,map2,clef_map1,clef_map2));
    assert(assert_all_key_map2_in_map1(map1,map2,clef_map1,clef_map2));
    return csv_join_force(map1,map2,clef_map1,clef_map2,keyNewName);
}

std::map<std::string,std::vector<std::string>*>* generate_infomap_for_join(Csv_meteoFranceParser* map1,Csv_meteoFranceParser* map2,std::string clef_map1,std::string clef_map2,std::string keyNewName)
{
    std::map<std::string,std::vector<std::string>*>* retMap=new std::map<std::string,std::vector<std::string>*>();
    std::map<std::string,int>* position_map1=map1->getAssocKeyFast(clef_map1);
    std::map<std::string,int>* position_map2=map2->getAssocKeyFast(clef_map2);
    std::map<std::string,int>* apartenanceKey=new std::map<std::string,int>();
    std::vector<std::string>* keys_map1=map1->keys();
    std::vector<std::string>* keys_map2=map2->keys();
    std::vector<std::string>* id_map1=map1->operator [](clef_map1);
    std::vector<std::string>* id_map2=map2->operator [](clef_map2);

    int posIdInMap1;
    int posIdInMap2;
    std::string idValue;

    std::string keyOfRetMap_local;

    retMap->operator [](keyNewName)=new std::vector<std::string>();
    apartenanceKey->operator [](keyNewName)=0;

    for(int i=0;i<keys_map1->size();i++)
    {
        if(clef_map1.compare(keys_map1->operator [](i))!=0)
        {
            retMap->operator [](keys_map1->operator [](i))=new std::vector<std::string>();
            apartenanceKey->operator [](keys_map1->operator [](i))=1;
        }
    }
    for(int i=0;i<keys_map2->size();i++)
    {
        if(clef_map2.compare(keys_map2->operator [](i))!=0)
        {
            retMap->operator [](keys_map2->operator [](i))=new std::vector<std::string>();
            apartenanceKey->operator [](keys_map2->operator [](i))=2;
        }
    }

    for(int i=0;i<id_map1->size();i++)
    {
        idValue=id_map1->operator [](i);
        if(position_map2->find(idValue)!=position_map2->end())
        {
            //l'id est dans les deux map
            posIdInMap1=i;
            posIdInMap2=position_map2->operator [](idValue);
            for(std::map<std::string,std::vector<std::string>*>::iterator iter=retMap->begin();iter!=retMap->end();iter++)
            {
                keyOfRetMap_local=iter->first;
                //(*retMap)[keyOfRetMap_local]->push_back(un truc)
                switch(apartenanceKey->operator [](keyOfRetMap_local))
                {
                    case 0:
                        (*retMap)[keyOfRetMap_local]->push_back((*map1)[clef_map1]->at(posIdInMap1));
                        break;
                    case 1:
                        (*retMap)[keyOfRetMap_local]->push_back((*map1)[keyOfRetMap_local]->at(posIdInMap1));
                        break;
                    case 2:
                        (*retMap)[keyOfRetMap_local]->push_back((*map2)[keyOfRetMap_local]->at(posIdInMap2));
                        break;
                    default:
                        assert(false);
                        break;
                }
            }
        }
    }

    delete(keys_map1);
    delete(keys_map2);
    delete(position_map1);
    delete(position_map2);
    delete(apartenanceKey);
    return retMap;
}


Csv_meteoFranceParser* csv_join_force(Csv_meteoFranceParser* map1,Csv_meteoFranceParser* map2,std::string clef_map1,std::string clef_map2,std::string keyNewName)
{
    assert(map1->keyExist(clef_map1));
    assert(map2->keyExist(clef_map2));
    //assert(no_keys_with_same_name(map1,map2,clef_map1));
    std::map<std::string,std::vector<std::string>*>* infoMap=generate_infomap_for_join(map1,map2,clef_map1,clef_map2,keyNewName);
    Csv_meteoFranceParser* retCsv=new Csv_meteoFranceParser(infoMap);
//    libere_infomap_for_join(infoMap);


    return retCsv;
}

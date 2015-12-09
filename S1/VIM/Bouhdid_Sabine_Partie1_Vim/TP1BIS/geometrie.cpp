#include "geometrie.h"

Geometrie::Geometrie()
{

}

Geometrie::~Geometrie()
{

}



Noeud* Geometrie::Find(Noeud *x){
    if(x->etiq == (x->id)->etiq){
        return x;
    }else{
        Noeud *root = Find(x->id);
        x->id = root;
        return root;
    }
    return x;
}

void Geometrie::Union(Noeud *x, Noeud *y){
    Noeud *xRoot = Find(x);
    Noeud *yRoot = Find(y);
    if(xRoot->etiq == yRoot->etiq){
        return;
    }
    if(xRoot->rank < yRoot->rank){
        xRoot->id = yRoot;
        yRoot->rank++;
    }else{
        yRoot->id = xRoot;
        xRoot->rank++;
    }
}

void Geometrie::MakeSet(Noeud* x){
    x->id = x;
}

/*On suppose img binaire : 2 valeurs 0 et 255*/
/* Cas particuliers : si bords noirs ? on rajoute un cadre blanc ATTENTION ne pas oublier d'enlever le cadre à la fin !*/
/* connectivite = 4 ou 8 */
QImage* Geometrie::etiqueter(QImage* img,int connectivite)
{
    QRgb blanc = qRgb(255,255,255);
    QRgb noir = qRgb(0,0,0);
    /*On rajoute un cadre blanc autour de l'image au cas où*/
    QImage* copie = new QImage(img->width()+2,img->height()+2,img->format());

    QPainter* painter = new QPainter(copie);
    painter->drawImage(1,1,*img);
    painter->end();


    for(int y=0;y<copie->height();y++){
        copie->setPixel(0,y,blanc);
        copie->setPixel(copie->width()-1,y,blanc);
    }
    for(int x=0;x<copie->width();x++){
        copie->setPixel(x,0,blanc);
        copie->setPixel(x,copie->height()-1,blanc);
    }
    QImage* res = new QImage(copie->copy(0,0,copie->width(),copie->height()));
    /******************************************************/

    /****PREMIERE PASSE****/
    int pas =1;
    bool voisIn;//vrai si au moins un voisin dans l'image
    int** etiquettes = new int*[copie->width()];
    for(int i=0;i<copie->width();i++){
        etiquettes[i] = new  int[copie->height()];
    }
    for(int i=0;i<copie->width();i++){
        for(int j=0;j<copie->height();j++){
            etiquettes[i][j] = -1;
        }
    }
    //Noeud* node;
    QPoint voisMin;
    vector< Noeud > etiqRef= vector<Noeud>();
    Noeud *ref;

    int etiq;
    int etiqV;
    int etiquetteNum=0;
    int nbObjet =0;

    for(int y=pas;y<copie->height()-pas;y++){
        for(int x=pas;x<copie->width()-pas;x++){
            if(copie->pixel(x,y)==noir){//si pixel est dans l'image
                voisIn = regarderVoisins(copie,x,y,pas,connectivite);
                if(voisIn){
                    //trouver le voisin dans l'image d'etiquette la plus petite
                    voisMin = minVoisin(etiquettes,x,y,pas,connectivite);
                    //stocker l'equivalence entre ces 2 etiquettes
                    etiqV = etiquettes[voisMin.x()][voisMin.y()];
                    etiq = etiquettes[x][y];
                    /* etiqueter tous les autres voisins*/
                    //etiquettes = etiqueterVoisins(etiquettes,x,y,pas,etiqVois,connectivite,&etiqRef);

                    if(connectivite==4){
                        int v1 =etiquettes[x-pas][y];
                        int v2 = etiquettes[x][y-pas];
                        if(v1 !=-1 && v1 < etiqV){
                            Union(&((etiqRef)[v1]),&((etiqRef)[etiqV]));
                            etiquettes[x-pas][y] = etiqV;
                            nbObjet--;
                        }
                        if(v2 !=-1 && v2 < etiqV){
                            Union(&((etiqRef)[v2]),&((etiqRef)[etiqV]));
                            etiquettes[x][y-pas] = etiqV;
                            nbObjet--;

                        }

                    }else{
                        int v1 = etiquettes[x-pas][y];
                        int v2 = etiquettes[x-pas][y-pas];
                        int v3 = etiquettes[x][y-pas];
                        int v4 = etiquettes[x+pas][y-pas];
                        if(v1!=-1 && v1 < etiqV){
                            Union(&((etiqRef)[v1]),&((etiqRef)[etiqV]));
                            etiquettes[x-pas][y] = etiqV;
                            nbObjet--;

                        }
                        if(v2!=-1 && v2 < etiqV){
                            Union(&((etiqRef)[v2]),&((etiqRef)[etiqV]));
                            etiquettes[x-pas][y-pas] = etiqV;
                            nbObjet--;

                        }
                        if(v3!=-1 && v3 < etiqV){
                            Union(&((etiqRef)[v3]),&((etiqRef)[etiqV]));
                            etiquettes[x][y-pas] = etiqV;
                            nbObjet--;

                        }
                        if(v4!=-1 && v4 < etiqV){
                            Union(&((etiqRef)[v4]),&((etiqRef)[etiqV]));
                            etiquettes[x+pas][y-pas] = etiqV;
                            nbObjet--;

                        }

                    }






                    //etiqueter avec cette etiquette
                    etiquettes[x][y] = etiqV;

                }else{
                    /*Si aucun voisin dans l'image*/
                    //etiqueter une nouvelle etiquette
                    etiquettes[x][y] = etiquetteNum;
                    ref= new Noeud(etiquetteNum);
                    MakeSet(ref);
                    etiqRef.push_back(*ref);
                    etiquetteNum++;
                    nbObjet++;

                }
            }else{
            }
        }
    }

    /****DEUXIEME PASSE****/
    Noeud *n;
    for(int y=pas;y<copie->height()-pas;y++){
        for(int x=pas;x<copie->width()-pas;x++){
            etiq = etiquettes[x][y];
            if(etiq!=-1){
                n = Find(&(etiqRef[etiq]));
                etiquettes[x][y] = n->etiq;
                res->setPixel(x,y,qRgb(n->etiq*50,0,n->etiq*20));

            }else{
                res->setPixel(x,y,blanc);
            }
        }
   }


    cout <<"Il y a "<<nbObjet<<" objets dans l'image!"<<endl;
    return res;
}

int** Geometrie::etiqueterVoisins(int **etiquettes,int x,int y,int pas,int etiqV,int connectivite, vector< Noeud > *etiqRef){
    if(connectivite==4){
        int v1 =etiquettes[x-pas][y];
        int v2 = etiquettes[x][y-pas];
        if(v1 !=-1 && v1 < etiqV){
            Union(&((*etiqRef)[v1]),&((*etiqRef)[etiqV]));
            etiquettes[x-pas][y] = etiqV;
        }
        if(v2 !=-1 && v2 < etiqV){
            Union(&((*etiqRef)[v2]),&((*etiqRef)[etiqV]));
            etiquettes[x][y-pas] = etiqV;

        }

    }else{
        int v1 = etiquettes[x-pas][y];
        int v2 = etiquettes[x-pas][y-pas];
        int v3 = etiquettes[x][y-pas];
        int v4 = etiquettes[x+pas][y-pas];
        if(v1!=-1 && v1 < etiqV){
            Union(&((*etiqRef)[v1]),&((*etiqRef)[etiqV]));
            etiquettes[x-pas][y] = etiqV;

        }
        if(v2!=-1 && v2 < etiqV){
            Union(&((*etiqRef)[v2]),&((*etiqRef)[etiqV]));
            etiquettes[x-pas][y-pas] = etiqV;

        }
        if(v3!=-1 && v3 < etiqV){
            Union(&((*etiqRef)[v3]),&((*etiqRef)[etiqV]));
            etiquettes[x][y-pas] = etiqV;

        }
        if(v4!=-1 && v4 < etiqV){
            Union(&((*etiqRef)[v4]),&((*etiqRef)[etiqV]));
            etiquettes[x+pas][y-pas] = etiqV;

        }

    }
    return etiquettes;

}

/*renvoie les coordonnées du voisin dans l'image avec la plus petite etiquette*/
QPoint Geometrie::minVoisin(int **etiquettes,int x, int y,int pas, int connectivite){
    if(connectivite == 4){
        int v1 =etiquettes[x-pas][y];
        int v2 = etiquettes[x][y-pas];
        if(v1==-1){//v1 pas dans l'image, on renvoit v2
            return QPoint(x,y-pas);
        }else if(v2 == -1){
            return QPoint(x-pas,y);

        }else if(v1 <= v2){//si les 2 sont dans l'image , on prend le plus petit
            return QPoint(x-pas,y);
        }else{
            return QPoint(x,y-pas);
        }
    }else{
        int v1 = etiquettes[x-pas][y];
        int v2 = etiquettes[x-pas][y-pas];
        int v3 = etiquettes[x][y-pas];
        int v4 = etiquettes[x+pas][y-pas];
        /*Si le voisin n'est pas dans l'image je lui passe une valeur max pour qu'il ne soit pas choisi comme ayant la plus petite etiquette*/
        if(v1 == -1){
            v1 = INT_MAX;
        }
        if(v2 == -1){
            v2 = INT_MAX;
        }
        if(v3 == -1){
            v3 = INT_MAX;
        }
        if(v4 == -1){
            v4 = INT_MAX;
        }
        if(v1 <= v2){
            if(v1 <= v3){
                if( v1 <= v4){
                    return QPoint(x-pas,y);
                }else{
                    return QPoint(x+pas,y-pas);
                }

            }else{
                if(v3 <= v4){
                    return QPoint(x,y-pas);
                }else{
                    return QPoint(x+pas,y-pas);
                }

            }
        }else{
            if(v2 <= v3){
                if(v2 <= v4){
                    return QPoint(x-pas,y-pas);
                }else{
                    return QPoint(x+pas,y-pas);
                }

            }else{
                if(v3 <= v4){
                    return QPoint(x,y-pas);
                }else{
                    return QPoint(x+pas,y-pas);
                }

            }

        }
    }
}

/*renvoie vrai si au moins un des voisins du pixel à la position(x,y) de l'image img est dans un objet (pixel noir)*/


bool Geometrie::regarderVoisins(QImage* img,int x,int y,int pas,int connectivite){
    QRgb noir = qRgb(0,0,0);
    /*Si la connectivite est de 4, on regarde le voisin du dessus et de gauche*/
    if(connectivite == 4){
        return ((img->pixel(x-pas,y)==noir)||(img->pixel(x,y-pas)==noir));
    }
    /*Si la connectivite est de 8,
     *on regarde les voisins de gauche, coin haut gauche, haut et coin haut droit
     */
    if(connectivite == 8){
       return((img->pixel(x-pas,y)==noir) || (img->pixel(x-pas,y-pas)==noir) || (img->pixel(x,y-pas)==noir) || (img->pixel(x+pas,y-pas)==noir));
    }
    return false;
}

/*COMPTER ELEMENTS D UNE IMAGE*/
QImage* Geometrie::compterElements(QImage* img, QImage* img1,QImage* img2, QImage* img3,int s1, int s2,int s3){
    QImage* copie = new QImage(img->copy(0,0,img->width(),img->height()));


    return copie;
}


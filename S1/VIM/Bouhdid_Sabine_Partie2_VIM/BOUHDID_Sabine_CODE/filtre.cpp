#include "filtre.h"

Filtre::Filtre()
{

}

Filtre::~Filtre()
{

}

QImage* Filtre::gaussien(QImage* img,int k)
{

    float **mat;
    /*allocation du noyau de convolution de taille kxk*/
    mat = new float*[k];
    for(int i=0;i<k;i++){
        mat[i] = new float[k];
    }
    if(k==3){
        mat[0][0] = 1.0;
        mat[0][1] = 2.0;
        mat[0][2] = 1.0;
        mat[1][0] = 2.0;
        mat[1][1] = 4.0;
        mat[1][2] = 2.0;
        mat[2][0] = 1.0;
        mat[2][1] = 2.0;
        mat[2][2] = 1.0;

    }
    int h = 16;
    int pas = k/2;
    int npixR=0;
    int npixB=0;
    int npixG=0;
    QRgb pix;
    QImage *copie = new QImage(img->width(), img->height(), img->format() );

    for(int x=pas;x <( img->width()-pas);x++){
        for(int y=pas;y < (img->height()-pas);y++){
            for(int k1=-pas;k1<=pas;k1++){
                for(int k2=-pas;k2<=pas;k2++){
                    pix = img->pixel(x+k1,y+k2);
                    npixR +=(qRed(pix)* mat[pas+k1][pas+k2]);
                    npixB +=(qBlue(pix)*  mat[pas+k1][pas+k2]);
                    npixG +=(qGreen(pix)*  mat[pas+k1][pas+k2]);
                }
            }
             npixR = npixR/h;
             npixB = npixB/h;
             npixG = npixG/h;
             pix = qRgb(npixR,npixG,npixB);
             copie->setPixel(x,y,pix);
             npixR = 0;
             npixB = 0;
             npixG = 0;

        }
    }
    return copie;

}

QImage* Filtre::adaptatif(QImage* img,int k)
{

    QImage* copie = new QImage(img->width(), img->height(), img->format() );
    int pas = k/2;
    /*matR, matG et matB sont les matrices de différence de densités*/
    float ** matR, **matG, **matB, **matCR, **matCG, **matCB;
    float sommeR =0;
    float sommeG =0;
    float sommeB =0;
    /*allocation du noyau de convolution de taille kxk*/
    matR = new float*[k];
    for(int i=0;i<k;i++){
        matR[i] = new float[k];
    }

    matG = new float*[k];
    for(int i=0;i<k;i++){
        matG[i] = new float[k];
    }

    matB = new float*[k];
    for(int i=0;i<k;i++){
        matB[i] = new float[k];
    }

    /**/

    matCR = new float*[k];
    for(int i=0;i<k;i++){
        matCR[i] = new float[k];
    }
    matCG = new float*[k];
    for(int i=0;i<k;i++){
        matCG[i] = new float[k];
    }
    matCB = new float*[k];
    for(int i=0;i<k;i++){
        matCB[i] = new float[k];
    }

    int pix;
    int npixR, npixG,npixB;
    /*construction du noyau et application*/
    for(int x =pas;x<img->width()-pas;x++){
        for(int y =pas; y<img->height()-pas;y++){
            sommeR = 0;
            sommeG = 0;
            sommeB = 0;
            /*On remplit les matrices de difference d'intensité*/
            for(int k1=-pas;k1<=pas;k1++){
                for(int k2=-pas;k2<=pas;k2++){

                    matR[pas+k1][pas+k2] = abs(qRed(img->pixel(x,y))-qRed(img->pixel(x+k1,y+k2)));
                    if(matR[pas+k1][pas+k2] == 0 ){
                        matR[pas+k1][pas+k2]=5;
                    }

                    matG[pas+k1][pas+k2] = abs(qGreen(img->pixel(x,y)) -qGreen(img->pixel(x+k1,y+k2)));
                    if(matG[pas+k1][pas+k2] == 0 ){
                        matG[pas+k1][pas+k2]=5;
                    }
                    matB[pas+k1][pas+k2] = abs(qBlue(img->pixel(x,y)) -qBlue(img->pixel(x+k1,y+k2)));
                    if(matB[pas+k1][pas+k2] == 0 ){
                        matB[pas+k1][pas+k2]=5;
                    }

                    //On récupère seulement les voisins
                    if((pas+k1) != 0 || (pas+k2) !=0){
                        sommeR += 1/matR[pas+k1][pas+k2];
                        sommeG += 1/matG[pas+k1][pas+k2];
                        sommeB += 1/matB[pas+k1][pas+k2];
                    }

                }
            }
            npixR = 0;
            npixG = 0;
            npixB = 0;
            for(int k1=-pas;k1<=pas;k1++){
                for(int k2=-pas;k2<=pas;k2++){
                    matCR[pas+k1][pas+k2] =( 1/matR[pas+k1][pas+k2] * 1/sommeR) ;
                    matCG[pas+k1][pas+k2] =( 1/matG[pas+k1][pas+k2] * 1/sommeG) ;
                    matCB[pas+k1][pas+k2] =( 1/matB[pas+k1][pas+k2] * 1/sommeB) ;

                    pix = img->pixel(x+k1,y+k2);
                    npixR +=(qRed(pix)* matCR[pas+k1][pas+k2]);
                    npixB +=(qBlue(pix)* matCG[pas+k1][pas+k2]);
                    npixG +=(qGreen(pix)* matCB[pas+k1][pas+k2]);
                }
            }
            copie->setPixel(x,y,qRgb(npixR,npixG,npixB));


        }
    }
    return copie;

}

/*Renvoie valeur mediane des valeurs contenues dans le tableau tab*/
int Filtre::med(int* tab,int k)
{
    sort(tab , tab+(k*k));
    return tab[((k*k)/2)+1];
}

QImage* Filtre::median(QImage* img,int k)
{
   QImage* copie = new QImage(img->width(), img->height(), img->format() );
    int pas = k/2;
   //CAS PARTICULIER

    int* tabR = new int[k*k];
    int* tabB = new int[k*k];
    int* tabG = new int[k*k];
    int i=0;

    for(int x =pas;x<img->width()-pas;x++){
        for(int y =pas; y<img->height()-pas;y++){
            //Pour chaque pixel
            i=0;
            for(int k1=-pas;k1<=pas;k1++){
                for(int k2=-pas;k2<=pas;k2++){
                    tabR[i] = qRed(img->pixel(x+k1,y+k2));
                    tabB[i] = qBlue(img->pixel(x+k1,y+k2));
                    tabG[i] = qGreen(img->pixel(x+k1,y+k2));
                    i++;
                }
            }
            copie->setPixel(x,y,qRgb(med(tabR,k),med(tabG,k),med(tabB,k)));
        }
    }

    return copie;

}

QImage* Filtre::harris(QImage* img,float alpha){

    QImage* H = new QImage(img->width(), img->height(), img->format() );

    QImage* I_x = new QImage(img->width(), img->height(), img->format() );
    I_x = gradientX(img,3);

    QImage* I_y = new QImage(img->width(), img->height(), img->format() );
    I_y = gradientY(img,3);

    /*On applique le gradientY à une image à laquelle on a appliqué le gradientX Ixy*/
    QImage* I_xy = new QImage(img->width(), img->height(), img->format() );
    I_xy = gradientY(I_x,3);

     double rH,gH,bH;
     QRgb pix_x,pix_y,pix_xy;

    /*
     * C = [ (I_x)² (I_xy) ; (I_xy) (I_y)²]
     * H = det C - alpha(trace C)²
     * H = ((I_x)²*(I_y)²)-(I_xy * I_xy)) - alpha*((I_x)²+(I_y)²)²
     */
     for(int x=0;x<img->width();x++){
         for(int y=0;y<img->height();y++){
            pix_x = I_x->pixel(x,y);
            pix_y = I_y->pixel(x,y);
            pix_xy = I_xy->pixel(x,y);

//            rH = pow(pow(qRed(pix_x),2)*pow(qRed(pix_y),2) - pow(qRed(pix_xy),2),2) - alpha*(pow(pow(qRed(pix_x),2)+pow(qRed(pix_y),2),2)) ;
//            gH = pow(pow(qGreen(pix_x),2)*pow(qGreen(pix_y),2) - pow(qGreen(pix_xy),2),2) - alpha*(pow(pow(qGreen(pix_x),2)+pow(qGreen(pix_y),2),2));
//            bH = pow(pow(qBlue(pix_x),2)*pow(qBlue(pix_y),2) - pow(qBlue(pix_xy),2),2) - alpha*(pow(pow(qBlue(pix_x),2)+pow(qBlue(pix_y),2),2));
            rH = pow(qRed(pix_x),2)*pow(qRed(pix_y),2) - pow(qRed(pix_xy),2) - alpha*(pow(pow(qRed(pix_x),2)+pow(qRed(pix_y),2),2)) ;
            gH = pow(qGreen(pix_x),2)*pow(qGreen(pix_y),2) - pow(qGreen(pix_xy),2) - alpha*(pow(pow(qGreen(pix_x),2)+pow(qGreen(pix_y),2),2));
            bH = pow(qBlue(pix_x),2)*pow(qBlue(pix_y),2) - pow(qBlue(pix_xy),2) - alpha*(pow(pow(qBlue(pix_x),2)+pow(qBlue(pix_y),2),2));
            H->setPixel(x,y,qRgb(rH,gH,bH));
         }
     }

    return H;

}

PointH** Filtre::pointsDInteret(QImage* img,QImage* H,int n){
    PointH** tabPH = new PointH*[n];
    int l=0;

    QImage* MaxLocaux = new QImage(img->width(), img->height(), img->format() );
    int pas = 1;
    QRgb pix ,vPix;
    bool maxLoc ;
    int k1,k2;
    for(int x=pas;x<img->width()-1;x++){
        for(int y=pas;y<img->height()-1;y++){
            maxLoc = true;
            pix = H->pixel(x,y);
            if(qRed(pix)>=0 && qGreen(pix)>=0 && qBlue(pix)>=0){
                k1 = -pas;
              while(k1 <= pas && maxLoc){
                  k2 = -pas;
                    while(k2<=pas && maxLoc){
                       vPix = H->pixel(x+k1,y+k2);
                       if((k2 <0 || (k1<0 && k2==0)) && qRed(pix)<=qRed(vPix)){
                           maxLoc = false;
                       }else if(qRed(pix)< qRed(vPix)){
                           maxLoc = false;
                       }
                       k2++;
                    }
                    k1++;
                }

                if(maxLoc){
                    l = inserer(tabPH,new PointH(x,y,pix),l,n);
                    MaxLocaux->setPixel(x,y,pix);
                }else{
                    MaxLocaux->setPixel(x,y,qRgb(0,0,0));
                }
            }else{
                MaxLocaux->setPixel(x,y,qRgb(0,0,0));
            }
        }
    }
    return tabPH;
}

QImage* Filtre::pointsInteret(QImage* img,float alpha,int n){

 QImage* H = harris(img,alpha);
     /*Maximaux locaux positifs*/

     PointH** tabPH = pointsDInteret(img,H,n);

    /*Extraction des N meilleurs points de Harris*/

     QImage* BestHarris = new QImage(img->width(), img->height(), img->format() );
     QImage* copie =new QImage(img->width(), img->height(), img->format() );;

     PointH *p;
     QRgb pix;

     for(int i=0;i<n;i++){
         p = tabPH[i];
         for(int k=-2;k<=2;k++){
           if( p->y+k>=0  && p->y+k<img->height()){
               BestHarris->setPixel(p->x,p->y+k,qRgb(255,0,0));
               copie->setPixel(p->x,p->y+k,qRgb(255,0,0));
           }
           if(p->x+k>=0&& p->x+k<img->width()){
               BestHarris->setPixel(p->x+k,p->y,qRgb(255,0,0));
               copie->setPixel(p->x+k,p->y,qRgb(255,0,0));
           }

         }
     }

     /*Affichage des N meileurs points*/
     for(int x=0;x<img->width();x++){
         for(int y = 0;y<img->height();y++){
             pix = copie->pixel(x,y);
             if(qRed(pix)==255 && qGreen(pix)==0 && qBlue(pix)==0){

             }else{
                 copie->setPixel(x,y,img->pixel(x,y));

             }
         }
     }




    return copie;
}

QImage* Filtre::correspondance(QImage* img1, QImage* img2,int nbPoints){
    QImage* copie = new QImage(img1->width()+50+img2->width(),img1->height(),img1->format());
    QImage* image1 = new QImage(img1->copy(0,0,img1->width(),img1->height()));
    QImage* image2 = new QImage(img2->copy(0,0,img2->width(),img2->height()));
    QImage* Himg1 = harris(img1,0.04);
    QImage* Himg2 = harris(img2,0.04);
    PointH** tabH1 = pointsDInteret(img1,Himg1,nbPoints);
    PointH** tabH2 = pointsDInteret(img2,Himg2,nbPoints);
    img1 = afficherPoints(image1,tabH1,nbPoints);//image1 avec Points d'interet
    img2 = afficherPoints(image2,tabH2,nbPoints);//image2 avec points d'interet

    PointS** tabS1 = similariteSSD( img1,tabH1,nbPoints,img2,tabH2,nbPoints); //points correspondants de l'image2 aux points d'interet de l'image 1
    PointS** tabS2 = similariteSSD( img2,tabH2,nbPoints,img1,tabH1,nbPoints);

   // image1 = img1;
    image2 = afficherPoints(image2,tabS1,nbPoints);

    image1 = afficherPoints(image1,tabS2,nbPoints);
    //image2 = afficherPoints(image1,tabS2,200)

    QPainter* painter = new QPainter(copie);
    painter->drawImage(0,0,*image1);
    painter->drawImage(image1->width()+50,0,*image2);
    painter->end();
    return copie;
}

QImage* Filtre::afficherPoints(QImage* img, PointS** tab,int n){
    QImage* copie = new QImage(img->copy(0,0,img->width(),img->height()));
    PointS* p;
    for(int i=0;i<n;i++){
        p = tab[i];
        for(int k=-2;k<=2;k++){
          if( p->y+k>=0  && p->y+k<img->height()){
              copie->setPixel(p->x,p->y+k,qRgb(255,0,0));
          }
          if(p->x+k>=0&& p->x+k<img->width()){
              copie->setPixel(p->x+k,p->y,qRgb(255,0,0));
          }

        }
    }
    return copie;
}

QImage* Filtre::afficherPoints(QImage* img, PointH** tab,int n){
    QImage* copie = new QImage(img->copy(0,0,img->width(),img->height()));
    PointH* p;
    for(int i=0;i<n;i++){
        p = tab[i];
        for(int k=-2;k<=2;k++){
          if( p->y+k>=0  && p->y+k<img->height()){
              copie->setPixel(p->x,p->y+k,qRgb(255,0,0));
          }
          if(p->x+k>=0&& p->x+k<img->width()){
              copie->setPixel(p->x+k,p->y,qRgb(255,0,0));
          }

        }
    }
    return copie;
}

/*img1 : image de gauche , img2: image de droite*/
PointS** Filtre::similariteSSD(QImage* img1, PointH** tabPH1,int n1,QImage* img2, PointH** tabPH2,int n2){
    PointS** tabPC =new PointS*[n1];
    for(int i=0;i<n1;i++){
        tabPC[i] = calcSimSSD(img1,img2,tabPH1[i],tabPH2,n2);
    }

    return tabPC;
}

PointS* Filtre::calcSimSSD(QImage* img1,QImage* img2,PointH* p,PointH** tabPH2,int n2){
   PointS** tabSSD = new PointS*[n2];
    int pas = 1;
    float somme =0;
    for(int x=0;x<n2;x++){
        for(int i=-pas;i<=pas;i++){
            for(int j=-pas;j<=pas;j++){
                if( (p->x+i>=0) &&( p->x+i < img1->width()) && (tabPH2[x]->x+i >=0) && (tabPH2[x]->x+i < img2->width()) && (p->y+j>=0 && p->y+j < img1->height()) && (tabPH2[x]->y+j >=0) && (tabPH2[x]->y+j < img2->height())){
                    somme += pow(qRed(img1->pixel(p->x+i,p->y+j)) - qRed(img2->pixel(tabPH2[x]->x+i,tabPH2[x]->y+j)),2);
                }
            }
        }
        tabSSD[x]= new PointS(tabPH2[x]->x,tabPH2[x]->y,somme);
        somme=0;
    }
    float max = INT_MIN;
    int i_max;
    for(int i=0;i<n2;i++){
        if(tabSSD[i]->somme > max){
            i_max = i;
            max = tabSSD[i]->somme;
        }
    }
    return tabSSD[i_max];
}


PointS** Filtre::similariteZNCC(QImage* img1,PointH** tabPH1,int n1,QImage* img2,PointH** tabPH2,int n2){
    PointS** tabPS =new PointS*[n1];
    for(int i=0;i<n1;i++){
        tabPS[i] = calcSimZNCC(img1,img2,tabPH1[i],tabPH2,n1,n2);
    }

    return tabPS;
}


PointS* Filtre::calcSimZNCC(QImage* img1,QImage* img2,PointH* p,PointH** tabPH2,int n1,int n2)
{
    PointS** tabZNCC = new PointS*[n2];
    float sigma1 ;
    float sigma2;
     int pas = 1;
     float somme =0;
     float sommeI = 0;
     sigma1 = calcSigma(img1,p,pas);
     for(int x=0;x<n2;x++){
         sigma2 = calcSigma(img2,tabPH2[x],pas);

     }
     for(int x=0;x<n2;x++){
         for(int i=-pas;i<=pas;i++){
             for(int j=-pas;j<=pas;j++){
                somme += pow(qRed(img1->pixel(p->x+i,p->y+j)) - qRed(img2->pixel(tabPH2[x]->x+i,tabPH2[x]->y+j)),2);
             }
         }
         tabZNCC[x]= new PointS(p->x,p->y,somme);
         somme=0;
     }
     float max = INT_MIN;
     int i_max;
     for(int i=0;i<n2;i++){
         if(tabZNCC[i]->somme >max){
             i_max = i;
             max = tabZNCC[i]->somme;
         }
     }
     return tabZNCC[i_max];
 }

float Filtre::calcSigma(QImage* img,PointH* p,int pas){
    float sigma = 0;
    float somme=0;
    float sommeSigma = 0;
    int nb=0;
    /*calcul de l'intensité moyenne*/
    for(int i=-pas;i<=pas;i++){
        for(int j=-pas;j<=pas;j++){
            somme+= qRed(img->pixel(p->x+i,p->y+j));
            nb++;
        }
    }
    /*calcul de sigma*/
    for(int i=-pas;i<=pas;i++){
        for(int j=-pas;j<=pas;j++){
            sommeSigma += pow(qRed(img->pixel(p->x+i,p->y+j))-(somme/nb),2);
        }
    }
    sigma = sqrt(sommeSigma/((2*pas+1)*(2*pas+1)));
    return sigma;
}

/*On suppose que tab est un tableau trié par ordre croissant*/
int Filtre::inserer(PointH** tab,PointH* p,int l,int n){
    PointH* tmp;
    PointH* aPlacer;
    int i;
    if(l<n){
        tab[l]=p;
        int k=l;
        while(k>0 && qRed(p->pix)< qRed(tab[k-1]->pix)){
             tmp = tab[k-1];
             tab[k-1] =tab[k] ;
             tab[k] = tmp;
             k--;
        }
        l++;


    }else{ //si tab est rempli.
        //Si le plus petit element de la table est plus grand que p.pix, on n'ajoute pas p.pix
        if(qRed(tab[0]->pix) >= qRed(p->pix)){


        }else{
            i=l-1;

            while(i>0 && qRed(p->pix) < qRed(tab[i]->pix)){
                i--;
            }
            if(i==0){//si c'est le premier je remplace
                tab[i]=p;
            }else{
                int j=i;
                aPlacer = p;
                while(j>=0){
                    tmp = tab[j];
                    tab[j]=aPlacer;
                    aPlacer = tmp;
                    j--;
                }
            }
        }
    }
    return l;
}


QImage* Filtre::gradient(QImage* img,int k){

    QImage* copie = new QImage(img->width(), img->height(), img->format() );

    QImage* copie1 = new QImage(img->width(), img->height(), img->format() );
    copie1 = gradientX(img,3);

    QImage* copie2 = new QImage(img->width(), img->height(), img->format() );
    copie2 = gradientY(img,3);
    QRgb pix1;
    QRgb pix2;
    double rPix;
    double bPix;
    double gPix;
    for(int x=0;x<img->width();x++){
        for(int y=0;y<img->height();y++){
            pix1 = copie1->pixel(x,y);
            pix2 = copie2->pixel(x,y);

            rPix = sqrt(qRed(pix1)*qRed(pix1)+qRed(pix2)*qRed(pix2));
            gPix = sqrt(qGreen(pix1)*qGreen(pix1)+qGreen(pix2)*qGreen(pix2));
            bPix = sqrt(qBlue(pix1)*qBlue(pix1)+qBlue(pix2)*qBlue(pix2));
            copie->setPixel(x,y,qRgb(rPix,gPix,bPix));
        }
    }

    return copie;
}


QImage* Filtre::gradientX(QImage* img,int k)
{

    float **mat;
    /*allocation du noyau de convolution de taille kxk*/
    mat = new float*[k];
    for(int i=0;i<k;i++){
        mat[i] = new float[k];
    }
    if(k==3){
        mat[0][0] = -1.0;
        mat[0][1] = -2.0;
        mat[0][2] = -1.0;
        mat[1][0] = 0.0;
        mat[1][1] = 0.0;
        mat[1][2] = 0.0;
        mat[2][0] = 1.0;
        mat[2][1] = 2.0;
        mat[2][2] = 1.0;

    }
    return appliquer(mat,img,4,k);

}

QImage* Filtre::gradientY(QImage* img,int k)
{
    float **mat;
    /*allocation du noyau de convolution de taille kxk*/
    mat = new float*[k];
    for(int i=0;i<k;i++){
        mat[i] = new float[k];
    }
    if(k==3){
        mat[0][0] = -1.0;
        mat[0][1] = 0.0;
        mat[0][2] = 1.0;
        mat[1][0] = -2.0;
        mat[1][1] = 0.0;
        mat[1][2] = 2.0;
        mat[2][0] = -1.0;
        mat[2][1] = 0.0;
        mat[2][2] = 1.0;

    }

    return appliquer(mat,img,4,k);

}


/*Realise une convolution */
QImage* Filtre::appliquer(float** noyau, QImage* img,int h,int k)
{

    int pas = k/2;
    int npixR=0;
    int npixB=0;
    int npixG=0;
    QRgb pix;
    QImage *copie = new QImage(img->width(), img->height(), img->format() );

    for(int x=pas;x <( img->width()-pas);x++){
        for(int y=pas;y < (img->height()-pas);y++){
            for(int k1=-pas;k1<=pas;k1++){
                for(int k2=-pas;k2<=pas;k2++){
                    pix = img->pixel(x+k1,y+k2);
                    npixR +=(qRed(pix)* noyau[pas+k1][pas+k2]/h);
                    npixB +=(qBlue(pix)* noyau[pas+k1][pas+k2]/h);
                    npixG +=(qGreen(pix)* noyau[pas+k1][pas+k2]/h);
                }
            }
             npixR = npixR/(k*k);
             npixB = npixB/(k*k);
             npixG = npixG/(k*k);
             pix = qRgb(npixR,npixG,npixB);
             copie->setPixel(x,y,pix);
             npixR = 0;
             npixB = 0;
             npixG = 0;

        }
    }
    return copie;

}

/*Seuille l'image */
QImage* Filtre::seuiller(QImage* img,int seuil)
{
    /*On recupere maxvalue*/
    int maxvalue = 0;
    int pix;
    for(int x=0;x<img->width();x++){
        for(int y=0;y<img->height();y++){
            if(qRed(img->pixel(x,y))>maxvalue){
                maxvalue = qRed(img->pixel(x,y));
            }
        }
    }

    QImage *copie = new QImage(img->width(), img->height(),img->format());
    for(int x=0;x<img->width();x++){
        for(int y=0;y<img->height();y++){
            pix = qRed(img->pixel(x,y));
            if(pix>seuil){
                copie->setPixel(x,y,qRgb(maxvalue,maxvalue,maxvalue));
            }else{
                copie->setPixel(x,y,qRgb(0,0,0));
            }
        }
    }
    return copie;
}

QImage* Filtre::negatif(QImage* img)
{
    /*On recupere maxvalue*/
    int maxvalue = 0;
    int valR,valG,valB;
    for(int x=0;x<img->width();x++){
        for(int y=0;y<img->height();y++){
            if(qRed(img->pixel(x,y))>maxvalue){
                maxvalue = qRed(img->pixel(x,y));
            }
        }
    }

    QImage *copie = new QImage(img->width(), img->height(),img->format());
    for(int x=0;x<img->width();x++){
        for(int y=0;y<img->height();y++){
            valR = abs(maxvalue - qRed(img->pixel(x,y)));
            valG = abs(maxvalue - qGreen(img->pixel(x,y)));
            valB = abs(maxvalue - qBlue(img->pixel(x,y)));
            copie->setPixel(x,y,qRgb(valR,valG,valB));
        }
    }
    return copie;
}


/*binarise une image seuillee*/

QImage* Filtre::binariser(QImage* img){
    *img = img->convertToFormat(QImage::Format_Indexed8);
    return img;
}


QImage* Filtre::kMeansRouge(QImage* img,int K,int n){
    QImage* copie = new QImage(img->copy(0,0,img->width(),img->height()));

    int* centresR = new int[K];

    vector<PointH>* clustersR = new vector<PointH>[K];
    /*INITIALISATION */
    /*Pour choisir les bons centres de cluster, on cherche les K intensités
     * les plus prédominantes dans l'image en utilisant l'histogramme */

    QRgb pix;
    int rouge;
    int minR;
    Histogramme h;

    h.histo(copie);
    centresR = h.intensitePredomR(K);

    /*ALLOCATION*/
    for(int iter=0;iter<n;iter++){
        for(int x=0;x<img->width();x++){
            for(int y=0;y<img->height();y++){
                pix = img->pixel(x,y);
                rouge = qRed(pix);
                //calculer la distance euclidienne et prendre la plus petite !
                minR=0;
                for(int i=0;i<K;i++){
                    if( abs(centresR[i]-rouge) <abs(centresR[minR]-rouge)){
                        minR =i;
                    }
                }
                clustersR[minR].push_back(PointH(x,y,pix));
            }
        }
        /**On recalcule les centres**/
        int* histR;
        int somme=0;

        for(int i=0;i<K;i++){
            histR = new int[clustersR[i].size()];
            /*POUR LES ROUGES */
            somme=0;
            for(int j=0;j<clustersR[i].size();j++){
                /**/
                histR[j] =qRed(clustersR[i][j].pix);
                somme += histR[j];
            }
            centresR[i] = floor(somme/clustersR[i].size());

        }
    }
    /*On recopie dans l'image ! */

    for(int i=0;i<K;i++){
       for(int j=0;j<clustersR[i].size();j++){
           copie->setPixel( clustersR[i][j].x,clustersR[i][j].y,qRgb(centresR[i],0,0));
       }
    }

    return copie;

}
QImage* Filtre::kMeansVert(QImage* img,int K,int n){

    QImage* copie = new QImage(img->copy(0,0,img->width(),img->height()));

    int* centresG = new int[K];

    vector<PointH>* clustersG = new vector<PointH>[K];
    /*INITIALISATION */
    /*Pour choisir les bons centres de cluster, on cherche les K intensités
     * les plus prédominantes dans l'image en utilisant l'histogramme */

    QRgb pix;
    int vert;
    int minG;
    Histogramme h;

    h.histo(copie);
    centresG = h.intensitePredomG(K);

    /*ALLOCATION*/
    for(int iter=0;iter<n;iter++){
        for(int x=0;x<img->width();x++){
            for(int y=0;y<img->height();y++){
                pix = img->pixel(x,y);
                vert = qGreen(pix);
                //calculer la distance euclidienne et prendre la plus petite !
                minG=0;
                for(int i=0;i<K;i++){
                    if( abs(centresG[i]-vert) <abs(centresG[minG]-vert)){
                        minG =i;
                    }
                }
                clustersG[minG].push_back(PointH(x,y,pix));

            }
        }
        /**On recalcule les centres**/
        int* histG;
        int somme=0;

        for(int i=0;i<K;i++){
            histG = new int[clustersG[i].size()];
            /*POUR LES VERTS */
            somme=0;
            for(int j=0;j<clustersG[i].size();j++){
                /**/
                histG[j] =qGreen(clustersG[i][j].pix);
                somme += histG[j];
            }
            centresG[i] = floor(somme/clustersG[i].size());
        }
    }
    /*On recopie dans l'image ! */

    for(int i=0;i<K;i++){
       for(int j=0;j<clustersG[i].size();j++){
           copie->setPixel(clustersG[i][j].x,clustersG[i][j].y,qRgb(0,centresG[i],0));
       }
    }
    return copie;
}
QImage* Filtre::kMeansBleu(QImage* img,int K,int n){
    QImage* copie = new QImage(img->copy(0,0,img->width(),img->height()));

    int* centresB = new int[K];

    vector<PointH>* clustersB = new vector<PointH>[K];
    /*INITIALISATION */
    /*Pour choisir les bons centres de cluster, on cherche les K intensités
     * les plus prédominantes dans l'image en utilisant l'histogramme */

    QRgb pix;
    int bleu;
    int minB;
    Histogramme h;

    h.histo(copie);
    centresB = h.intensitePredomB(K);
    /*ALLOCATION*/
    for(int iter=0;iter<n;iter++){
        for(int x=0;x<img->width();x++){
            for(int y=0;y<img->height();y++){
                pix = img->pixel(x,y);
                bleu = qBlue(pix);
                //calculer la distance euclidienne et prendre la plus petite !
                minB=0;
                for(int i=0;i<K;i++){
                    if( abs(centresB[i]-bleu) <abs(centresB[minB]-bleu)){
                        minB =i;
                    }

                }
                clustersB[minB].push_back(PointH(x,y,pix));

            }
        }
        /**On recalcule les centres**/
        int* histB;
        int somme=0;

        for(int i=0;i<K;i++){
            histB = new int[clustersB[i].size()];

            /*POUR LES BLEUS */
            somme=0;
            for(int j=0;j<clustersB[i].size();j++){
                /**/
                histB[j] =qBlue(clustersB[i][j].pix);
                somme += histB[j];
            }
            centresB[i] = floor(somme/clustersB[i].size());
        }
    }
    /*On recopie dans l'image ! */
    for(int i=0;i<K;i++){
       for(int j=0;j<clustersB[i].size();j++){
           copie->setPixel(clustersB[i][j].x,clustersB[i][j].y,qRgb(0,0,centresB[i]));
       }
    }


    return copie;

}
QImage* Filtre::kMeansRGB(QImage* img,int K,int n){
    QImage* copie = new QImage(img->copy(0,0,img->width(),img->height()));

    QRgb* centresRGB = new QRgb[K];

    vector<PointH>* clustersRGB = new vector<PointH>[K];
    /*INITIALISATION */
    /*Pour choisir les bons centres de cluster, on cherche les K intensités
     * les plus prédominantes dans l'image en utilisant l'histogramme */

    QRgb pix;
    int minRGB;
    int dist1;
    int dist2;
    Histogramme h;

    h.histo(copie);
    centresRGB = h.intensitePredomRGB(K);
    /*ALLOCATION*/
    for(int iter=0;iter<n;iter++){
        for(int x=0;x<img->width();x++){
            for(int y=0;y<img->height();y++){
                pix = img->pixel(x,y);
                //calculer la distance euclidienne et prendre la plus petite !
                minRGB = 0;
                for(int i=0;i<K;i++){
                    dist1 = distEucl(centresRGB[i], pix);
                    dist2 = distEucl(centresRGB[minRGB], pix);
                    if( dist1 < dist2){
                         minRGB = i;
                    }
                }
                clustersRGB[minRGB].push_back(PointH(x,y,pix));

            }
        }
        /**On recalcule les centres**/
        QRgb* histRGB ;
        int sommeR=0;
        int sommeG=0;
        int sommeB=0;
        int taille;


        for(int i=0;i<K;i++){
            histRGB = new QRgb[clustersRGB[i].size()];
            taille = clustersRGB[i].size();
            for(int j=0;j<taille;j++){
                                /**/
                histRGB[j] =clustersRGB[i][j].pix;
                sommeR += qRed(histRGB[j]);
                sommeB += qBlue(histRGB[j]);
                sommeG += qGreen(histRGB[j]);
            }
            if(taille !=0){
                centresRGB[i] = qRgb(floor(sommeR/taille),floor(sommeG/taille),floor(sommeB/taille));
            }

        }
    }
    /*On recopie dans l'image ! */

    for(int i=0;i<K;i++){
        cout<<"centresRGB["<<i<<"] "<<qRed(centresRGB[i])<<" "<<qGreen(centresRGB[i])<<" "<<qBlue(centresRGB[i])<<endl;
       for(int j=0;j<clustersRGB[i].size();j++){
          copie->setPixel(clustersRGB[i][j].x,clustersRGB[i][j].y,centresRGB[i]);
       }
    }


    return copie;
}



QImage* Filtre::kMeans(QImage* img,int K,int n,int coul){
   if(coul ==1){
       return kMeansRouge(img,K,n);
   }
   if(coul == 2){
       return kMeansVert(img,K,n);
   }
   if(coul == 3){
       return kMeansBleu(img,K,n);
   }
   if(coul == 4){
       return kMeansRGB(img,K,n);
   }

    return NULL;
}


int Filtre::distEucl(QRgb pix1,QRgb pix2){
    return pow(qRed(pix1)-qRed(pix2),2) + pow(qGreen(pix1) - qGreen(pix2),2) + pow(qBlue(pix1)-qBlue(pix2),2);
}

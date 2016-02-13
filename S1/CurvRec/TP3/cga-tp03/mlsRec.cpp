#include "mlsRec.h"
#include <cmath>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

QImage MLSRec::apply(const QImage &img,float sigma) {

  // TODO apply weighted least squares on each pixel of the input image
  // example of image manipulation using Qt: 

  // copy input 
  QImage newImg = img; 

  // iterate eover pixels 
  for(int x=0;x<img.width();++x) {
    for(int y=0;y<img.height();++y) {
      // get color at x,y
      Color current = getColor(img,x,y);
      
      // check if this color is missing in the image
      if(colorMissing(current)) {
	// set color as red 
       //current = Color(255.0f,0.0f,0.0f,255.0f);
        // current = estimateColorPlane(img, x, y,sigma);
         sigma = estimateSigma(img,x,y);
         current = estimateColorPlane(img,x,y,sigma);
      }
      // set color in the new image
       setColor(newImg,x,y,current);
    }

    // show progression 
    if(!(x%10))
      cout << "MLS: " << (int)(((float)x/(float)(img.width()-2))*100.0f) << " percent done..." << '\r' << flush;
  }


  // example of matrix manip using Eigen:
  
  // 0 matrix with 4 rows, 3 columns
  MatrixXf M = MatrixXf::Zero(4,3);
  
  // set row 1 and column 3 to 5.0f
  M(0,2) = 5.0f;
  
  // transposed matrix 
  MatrixXf T = M.transpose();
  
  // multiplication 
  MatrixXf MT = M*T;

  // matrix inversion
  MatrixXf MTi = MT.inverse();

  // vector of size 4
  VectorXf v = VectorXf::Zero(4); 

  // matrix vector multiplication
  VectorXf Mv = MTi*v;

  return newImg;
}

MLSRec::Color MLSRec::estimateColorPlane(const QImage &img,int x,int y,float sigma) {
  // estimate (and eval) a,b,c such that 
  // ax + by + c = z
  // z being the value at pixel x,y (One MLS per channel then!)
  //voisinage où on regarde
  float voisinage = 3*sigma ;

  int N = 2*voisinage+1;
  //Taille de  matrice A
  int taille = N*N;
  MatrixXf A = MatrixXf::Zero(taille,3);
  //Br vector B pour couleur rouge
  VectorXf Br = VectorXf::Zero(taille);
  //Bg vector B pour couleur VERTE
  VectorXf Bg = VectorXf::Zero(taille);
  //Bb vector B pour couleur Bleue
  VectorXf Bb = VectorXf::Zero(taille);
  //Ba vector B pour  alpha
  VectorXf Ba = VectorXf::Zero(taille);
  Color c;

  MatrixXf Weight =  MatrixXf::Zero(taille,taille);
 float dist;
 int nb=0;
  for(int i=-voisinage;i<=voisinage;i++){
    if( (x+i>=0) && (x+i < img.width()) ){
        for(int j=-voisinage;j<=voisinage;j++){
            if( (y+j >=0) && (y+j <img.height()) ){
                c = getColor(img,x+i,y+j);
                if(!colorMissing(c)){
                    Br(( (voisinage+i)*N) + (voisinage+j)) = c.red;
                    Bg(( (voisinage+i)*N) + (voisinage+j)) = c.green;
                    Bb(( (voisinage+i)*N) + (voisinage+j)) = c.blue;
                    Ba(( (voisinage+i)*N) + (voisinage+j)) = c.alpha;
                    A(( (voisinage+i)*N) + (voisinage+j),0) = x+i;
                    A(( (voisinage+i)*N) + (voisinage+j),1) = y+j;
                    A(( (voisinage+i)*N) + (voisinage+j),2) = 1;
                    dist = (x-(x+i))*(x-(x+i)) + (y - (y+j))*(y - (y+j));
                    Weight( ( (voisinage+i)*N) + (voisinage+j),( (voisinage+i)*N) + (voisinage+j)) = 1/(sigma*sqrt(2*M_PI)) * exp(-(dist)/(2*sigma*sigma));
                    nb++;
                }
            }
        }
    }
 }

 MatrixXf At = A.transpose();
 MatrixXf AtW = At*(Weight);
 MatrixXf AtWA = (At*(Weight))*A;
 MatrixXf IAtWA = AtWA.inverse();


 MatrixXf Xr = IAtWA*AtW*Br;
 MatrixXf Xg = IAtWA*AtW*Bg;
 MatrixXf Xb = IAtWA*AtW*Bb;
 MatrixXf Xa = IAtWA*AtW*Ba;

 float rouge = x*Xr(0,0)+y*Xr(1,0)+Xr(2,0);
 float vert= x*Xg(0,0)+y*Xg(1,0)+Xg(2,0);
 float bleu= x*Xb(0,0)+y*Xb(1,0)+Xb(2,0);
 float alpha= x*Xa(0,0)+y*Xa(1,0)+Xa(2,0);


if(rouge <0.f)
    rouge = 0.0f;
if(rouge >255.0)
    rouge = 255.0f;

if(vert <0.f)
   vert = 0.0f;
if(vert >255.0f)
    vert = 255.0f;

if(bleu <0.f)
    bleu = 0.0f;
if(bleu >255.0)
    bleu = 255.0f;

if(alpha <0.f)
    alpha = 0.0f;
if(alpha >255.0)
    alpha = 255.0f;

 if(nb == 0){
    return Color(255.0f,255.0f,255.0f,255.0f);
 }

 if(rouge < std::numeric_limits<float>::infinity() && rouge > -std::numeric_limits<float>::infinity() ){
     return Color(rouge,vert,bleu,alpha);
 }
}

MLSRec::Color MLSRec::estimateColorQuadric(const QImage &img,int x,int y,float sigma) {
  // estimate (and eval) a,b,c,d,e,f such that 
  // ax^2 + by^2 + cxy + dx + ey + f = z
  // z being the value at pixel x,y (One MLS per channel then!)

    float voisinage = 3*sigma ;

    int N = 2*voisinage+1;
    //Taille de  matrice A
    int taille = N*N;
    MatrixXf A = MatrixXf::Zero(taille,6);
    //Br vector B pour couleur rouge
    VectorXf Br = VectorXf::Zero(taille);
    //Bg vector B pour couleur VERTE
    VectorXf Bg = VectorXf::Zero(taille);
    //Bb vector B pour couleur Bleue
    VectorXf Bb = VectorXf::Zero(taille);
    //Ba vector B pour  alpha
    VectorXf Ba = VectorXf::Zero(taille);
    Color c;
    //Matrice de poids
    MatrixXf Weight =  MatrixXf::Zero(taille,taille);
   float dist;
   int nb=0;

    for(int i=-voisinage;i<=voisinage;i++){
      if( (x+i>=0) && (x+i < img.width()) ){
          for(int j=-voisinage;j<=voisinage;j++){
              if( (y+j >=0) && (y+j <img.height()) ){
                  c = getColor(img,x+i,y+j);
                  if(!colorMissing(c)){
                      Br(( (voisinage+i)*N) + (voisinage+j)) = c.red;
                      Bg(( (voisinage+i)*N) + (voisinage+j)) = c.green;
                      Bb(( (voisinage+i)*N) + (voisinage+j)) = c.blue;
                      Ba(( (voisinage+i)*N) + (voisinage+j)) = c.alpha;
                      A(( (voisinage+i)*N) + (voisinage+j),0) = pow(x+i,2);//x²
                      A(( (voisinage+i)*N) + (voisinage+j),1) = pow(y+j,2);//y²
                      A(( (voisinage+i)*N) + (voisinage+j),2) = (x+i)*(y+j);//xy
                      A(( (voisinage+i)*N) + (voisinage+j),3) = x+i;//x
                      A(( (voisinage+i)*N) + (voisinage+j),4) = y+j;//y
                      A(( (voisinage+i)*N) + (voisinage+j),5) = 1;//1
                      dist = (x-(x+i))*(x-(x+i)) + (y - (y+j))*(y - (y+j));
                      Weight( ( (voisinage+i)*N) + (voisinage+j),( (voisinage+i)*N) + (voisinage+j)) = 1/(sigma*sqrt(2*M_PI)) * exp(-(dist)/(2*sigma*sigma));
                      nb++;
                  }
              }
          }
      }
   }


   MatrixXf At = A.transpose();
   MatrixXf AtW = At*(Weight);
   MatrixXf AtWA = (At*(Weight))*A;
   MatrixXf IAtWA = AtWA.inverse();

   MatrixXf Xr = AtWA.ldlt().solve(A.transpose()*Weight*Br);
   MatrixXf Xg = AtWA.ldlt().solve(A.transpose()*Weight*Bg);
   MatrixXf Xb = AtWA.ldlt().solve(A.transpose()*Weight*Bb);
   MatrixXf Xa = AtWA.ldlt().solve(A.transpose()*Weight*Ba);

   float rouge = (x*x*Xr(0,0)) + (y*y*Xr(1,0)) + (x*y*Xr(2,0)) + (x*Xr(3,0)) + (y*Xr(4,0)) +Xr(5,0);
   float vert= x*x*Xg(0,0) + y*y*Xg(1,0) + x*y*Xg(2,0) + x*Xg(3,0) + y*Xg(4,0) + Xg(5,0);
   float bleu= x*x*Xb(0,0) + y*y*Xb(1,0) + x*y*Xb(2,0) + x*Xb(3,0) + y*Xb(4,0) + Xb(5,0);
   float alpha= x*x*Xa(0,0)  + y*y*Xa(1,0) + x*y*Xa(2,0) + x*Xa(3,0) + y*Xa(4,0) + Xa(5,0);


  if(rouge < 0.f)
      rouge = 0.0f;
  if(rouge > 255.0)
      rouge = 255.0f;

  if(vert < 0.f)
     vert = 0.0f;
  if(vert > 255.0f)
      vert = 255.0f;

  if(bleu < 0.f)
      bleu = 0.0f;
  if(bleu > 255.0)
      bleu = 255.0f;

  if(alpha < 0.f)
      alpha = 0.0f;
  if(alpha > 255.0)
      alpha = 255.0f;

  if(nb = 0){
      return Color(255.0f,255.0f,255.0f,255.0f);
  }
  if(rouge < std::numeric_limits<float>::infinity() && rouge > -std::numeric_limits<float>::infinity() ){
      return Color(rouge,vert,bleu,alpha);
  }
  // TODO
}


float MLSRec::estimateSigma(const QImage &img,int x,int y) {
    // estimate sigma at x,y depending on the positions of nearest valid neighbors
      float sigma =1.0f;
      int voisinage = 3*sigma;
      Color c ;
      //nb de voisins existants
      int nb_neigh =0;
      int loop = 3;

      /* nombre de voisins parcourus (voisinage+1)*(voisinage+1)*/
      /* On veut avoir au minimum ((voisinage+1)*(voisinage+1))/ 2 voisins valides*/
      /* ce qui correspond à la moitié*/
      while(loop <= 25 && nb_neigh < voisinage/2 ){
          nb_neigh=0;
          //On augmente sigma d'un tiers pour que le voisinage augmente de 1
          sigma = loop*(1.0f/3.0f);
          voisinage = 3*sigma;

          for(int i=-voisinage;i<= voisinage;i++){
              if( (x+i>=0) && (x+i < img.width()) ){
                  for(int j=-voisinage;i<=voisinage;i++){
                      if( (y+j >=0) && (y+j <img.height()) ){
                          c = getColor(img,x+i,y+j);
                            if(!colorMissing(c)){
                                nb_neigh++;
                            }
                            if(nb_neigh >= voisinage/2){
                                return sigma;
                            }
                      }

                  }
              }
          }
          loop++;
      }

    return sigma;
}



float MLSRec::estimateSigma2(const QImage &img,int x,int y) {
  // estimate sigma at x,y depending on the positions of nearest valid neighbors
    float sigma =1.0f;
    int voisinage = 3*sigma;
    Color c ;
    //nb de voisins existants
    int nb_neigh =0;
    int loop = 3;

    /* nombre de voisins parcourus (voisinage+1)*(voisinage+1)*/
    /* On veut avoir au minimum ((voisinage+1)*(voisinage+1))/ 2 voisins valides*/
    /* ce qui correspond à la moitié*/
    while(loop <= 43 && nb_neigh < (voisinage/2) ){
        nb_neigh=0;
        //On augmente sigma d'un tiers pour que le voisinage augmente de 1
        sigma = loop*(1.0f/3.0f);
        voisinage = 3*sigma;

        for(int i=-voisinage;i<= voisinage;i++){
            if( (x+i>=0) && (x+i < img.width()) ){
                for(int j=-voisinage;i<=voisinage;i++){
                    if( (y+j >=0) && (y+j <img.height()) ){
                        c = getColor(img,x+i,y+j);
                          if(!colorMissing(c)){
                              nb_neigh++;
                          }
                          if(nb_neigh >= (voisinage/2)){
                              return sigma;
                          }
                    }

                }
            }
        }
        loop++;
    }

  return sigma;
}

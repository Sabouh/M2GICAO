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
       // if(x>=3*sigma && x<img.width()-3*sigma && y>=3*sigma && y<img.height()-3*sigma ){
            current = estimateColorPlane(img, x, y,sigma);
       // }else{
       //     current = Color(255.0f,0.0f,0.0f,255.0f);
       // }
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
  float voisinage = 3*sigma;
  int taille = (2*voisinage+1) *(2*voisinage+1);
  MatrixXf A = MatrixXf::Zero(taille,3);
  VectorXf Br = VectorXf::Zero(taille);
  VectorXf Bg = VectorXf::Zero(taille);
  VectorXf Bb = VectorXf::Zero(taille);
  VectorXf Ba = VectorXf::Zero(taille);
  Color c;
  for(int i=-voisinage;i<=voisinage;i++){
    if( (x+i>=0) && (x+i < img.width()) ){
        for(int j=-voisinage;j<=voisinage;j++){
            if( (y+j >=0) && (y+i <img.height()) ){
                c = getColor(img,x+i,y+j);
                if(!colorMissing(c)){
                    Br(( (voisinage+i)*(2*voisinage+1)) + (voisinage+j)) = c.red;
                    Bg(( (voisinage+i)*(2*voisinage+1)) + (voisinage+j)) = c.green;
                    Bb(( (voisinage+i)*(2*voisinage+1)) + (voisinage+j)) = c.blue;
                    Ba(( (voisinage+i)*(2*voisinage+1)) + (voisinage+j)) = c.alpha;
                    A(( (voisinage+i)*(2*voisinage+1)) + (voisinage+j),0) = x+i;
                    A(( (voisinage+i)*(2*voisinage+1)) + (voisinage+j),1) = y+j;
                    A(( (voisinage+i)*(2*voisinage+1)) + (voisinage+j),2) = 1;
                }
            }
        }
    }
 }

  MatrixXf Weight =  MatrixXf::Zero(taille,taille);
 float dist;
 for(int i=-voisinage;i<=voisinage;i++){
     if( (x+i>=0) && (x+i < img.width()) ){
        for(int j=-voisinage;j<=voisinage;j++){
            if( (y+j >=0) && (y+i <img.height()) ){
                c = getColor(img,x+i,y+j);
                if(!colorMissing(c)){
                    dist = (x-(x+i))*(x-(x+i)) + (y - (y+j))*(y - (y+j));
                    Weight( ( (voisinage+i)*(2*voisinage+1)) + (voisinage+j),( (voisinage+i)*(2*voisinage+1)) + (voisinage+j)) = 1/(sigma*sqrt(2*M_PI)) * exp(-(dist)/(2*sigma*sigma));
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

//cout <<"YAHOO"<<endl;
if(rouge <0)
    rouge = 0.0f;
if(rouge >255.0)
    rouge = 255.0f;

if(vert <0)
   vert = 0.0f;
if(vert >255.0)
    vert = 255.0f;

if(bleu <0)
    bleu = 0.0f;
if(bleu >255.0)
    bleu = 255.0f;

if(alpha <0)
    alpha = 0.0f;
if(alpha >255.0)
    alpha = 255.0f;
//cout <<"WHAT"<<endl;
  return Color(rouge,vert,bleu,alpha);
}

MLSRec::Color MLSRec::estimateColorQuadric(const QImage &img,int x,int y,float sigma) {
  // estimate (and eval) a,b,c,d,e,f such that 
  // ax^2 + by^2 + cxy + dx + ey + f = z
  // z being the value at pixel x,y (One MLS per channel then!)

  // TODO
  return Color(0.0f,0.0f,0.0f,255.0f);
}


float MLSRec::estimateSigma(const QImage &img,int x,int y) {
  // estimate sigma at x,y depending on the positions of nearest valid neighbors
  
  // TODO 
  return 2.0f;
}

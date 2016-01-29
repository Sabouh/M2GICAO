#ifndef IMG_FILTER
#define IMG_FILTER

#include <QImage>
#include <QColor>

// add noise in the image 
class NoiseFilter {
 public:
  static QImage applyWhiteNoise(const QImage &img, float noise) {
    QImage nimg = img.convertToFormat(QImage::Format_ARGB32);

    for(int i=0;i<nimg.width();++i) {
      for(int j=0;j<nimg.height();++j) {
	float r = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
	if(r<noise) {
	  nimg.setPixel(i,j,qRgba(0,0,0,0));
	}
      }
    }

    return nimg;
  }
};

// draw a square pattern 
class SquareFilter {
 public:
  static QImage applySquare(const QImage &img,int d) {
    QImage nimg = img.convertToFormat(QImage::Format_ARGB32);

    for(int i=0;i<nimg.width();++i) {
      for(int j=0;j<nimg.height();++j) {
	int tx = i%d;
	int ty = j%d;
	int d2 = d/2;
	if((tx<d2 && ty<d2) || (tx>=d2 && ty>=d2)) {
	  nimg.setPixel(i,j,qRgba(0,0,0,0));
	}
      }
    }

    return nimg;
  }
};


#endif // IMG_FILTER

#ifndef MLS_REC
#define MLS_REC

#include <QImage>
#include <iostream>
#include <QColor>

class MLSRec {
 public:
  static QImage apply(const QImage &img,float sigma);

 private:
  // convenient structure for color 
  typedef struct Color {
  Color() : red(0.0f),green(0.0f),blue(0.0f),alpha(0.0f) { }
  Color(float r,float g,float b,float a) : red(r),green(g),blue(b),alpha(a) { }
    
    float red;
    float green;
    float blue;
    float alpha;
  } Color;

  // get color of img at pixel (x,y), take care of boundaries 
  static inline Color getColor(const QImage &img,int x,int y) {
    const QRgb c = img.pixel(std::min(std::max(x,0),img.width()-1),
			     std::min(std::max(y,0),img.height()-1));
    return Color((float)qRed(c),(float)qGreen(c),(float)qBlue(c),(float)qAlpha(c));
  }  

  // set color of img at pixel (x,y), take care of boundaries 
  static inline void setColor(QImage &img,int x,int y,const Color &col) {
    img.setPixel(std::min(std::max(x,0),img.width()-1),
		 std::min(std::max(y,0),img.height()-1),
		 QColor((int)col.red,(int)col.green,(int)col.blue,(int)col.alpha).rgba());
  }
  
  // check if this color is missing (using alpha channel)
  static inline bool colorMissing(const Color &col) {
    return col.alpha<128.0f;
  }

  // reconstruction with MLS 
  static Color estimateColorPlane(const QImage &img,int x,int y,float sigma);
  static Color estimateColorQuadric(const QImage &img,int x,int y,float sigma);
  static float estimateSigma(const QImage &img,int x,int y);
  static float estimateSigma2(const QImage &img,int x,int y);
};

#endif // MLS_REC

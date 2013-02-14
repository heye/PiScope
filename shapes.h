#ifndef SHAPES_H
#define SHAPES_H

#ifdef __cplusplus

extern "C" void Translate(VGfloat, VGfloat);
extern "C" void Rotate(VGfloat);
extern "C" void Shear(VGfloat, VGfloat);
extern "C" void Scale(VGfloat, VGfloat);
extern "C" void Text(VGfloat, VGfloat, char *, Fontinfo, int);
extern "C" void TextMid(VGfloat, VGfloat, char *, Fontinfo, int);
extern "C" void TextEnd(VGfloat, VGfloat, char *, Fontinfo, int);
extern "C" VGfloat TextWidth(char *, Fontinfo, int);
extern "C" void Cbezier(VGfloat, VGfloat, VGfloat, VGfloat, VGfloat, VGfloat, VGfloat, VGfloat);
extern "C" void Qbezier(VGfloat, VGfloat, VGfloat, VGfloat, VGfloat, VGfloat);
extern "C" void Polygon(VGfloat *, VGfloat *, VGint);
extern "C" void Polyline(VGfloat *, VGfloat *, VGint);
extern "C" void Rect(VGfloat, VGfloat, VGfloat, VGfloat);
extern "C" void Line(VGfloat, VGfloat, VGfloat, VGfloat);
extern "C" void Roundrect(VGfloat, VGfloat, VGfloat, VGfloat, VGfloat, VGfloat);
extern "C" void Ellipse(VGfloat, VGfloat, VGfloat, VGfloat);
extern "C" void Circle(VGfloat, VGfloat, VGfloat);
extern "C" void Arc(VGfloat, VGfloat, VGfloat, VGfloat, VGfloat, VGfloat);
extern "C" void Image(VGfloat, VGfloat, int, int, char *);
extern "C" void Start(int, int);
extern "C" void End();
extern "C" void SaveEnd(char *);
extern "C" void Background(unsigned int, unsigned int, unsigned int);
extern "C" void BackgroundRGB(unsigned int, unsigned int, unsigned int, VGfloat);
extern "C" void init(int *, int *);
extern "C" void finish();
extern "C" void setfill(VGfloat[4]);
extern "C" void setstroke(VGfloat[4]);
extern "C" void StrokeWidth(VGfloat);
extern "C" void Stroke(unsigned int, unsigned int, unsigned int, VGfloat);
extern "C" void Fill(unsigned int, unsigned int, unsigned int, VGfloat);
extern "C" void RGBA(unsigned int, unsigned int, unsigned int, VGfloat, VGfloat[4]);
extern "C" void RGB(unsigned int, unsigned int, unsigned int, VGfloat[4]);
extern "C" void FillLinearGradient(VGfloat, VGfloat, VGfloat, VGfloat, VGfloat *, int);
extern "C" void FillRadialGradient(VGfloat, VGfloat, VGfloat, VGfloat, VGfloat, VGfloat *, int);
extern "C" Fontinfo loadfont(const int *, const int *, const unsigned char *, const int *, const int *, const int *, const short *,
			 int);
extern "C" void unloadfont(VGPath *, int);
extern "C" void makeimage(VGfloat, VGfloat, int, int, VGubyte *);
extern "C" void saveterm();
extern "C" void restoreterm();
extern "C" void rawterm();

#endif

#endif
#ifndef BRUSHES_H
#define BRUSHES_H
//#define RESPATH QApplication::applicationDirPath()+"/../../../"
#define RESPATH QApplication::applicationDirPath()

//#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QtGui>
#include <QPainter>
#include <QTabletEvent>
#include <signal.h>
#include "b_smartcolor.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <qbytearray.h>
#include <QByteArray>
#include <QtWidgets/QApplication>
#include <QCoreApplication>
enum csParams{    //Drawing modifiers
    csNone,
    csPressure,
    csVel,
    csDir,
    csRot,
    csTilt,
    csRelang,
    csHtilt,
    csVtilt,

    csLenpx,
    csAcc,

    csXtilt,
    csYtilt,
    csSTOP,
    csERASER,
    // disabled ones go here
        csLen,
    csCrv,
    csIdir,
    csHVdir,
    csHVrot,
    csENDPOINT
};
enum eTools {
    eBrush, eSmudge, eDisp,  eCont, eSTOP,eLine, eEOE
};
enum bmBlends {  //Blending Modes
    bmNormal,
    bmPlus,
    bmDodge,
    bmScreen,
    bmLighten,
    bmBurn,
    bmMult,
    bmDarken,
    bmOvr,
    bmHlight,
    bmSlight,
    bmXor,
    bmDiff,
    bmExc,
    bmSTOP

};
enum eLActions{ //Layer Actions
    laAdd,
    laDel,
    laDup,
    laOp,
    laBm,
    laVis,
    laDrop,
    laDropall,
    laProps,
    laSel,
    laMove,
    laTransform,
    laCropLayer,
    laNewCanvas,
    laResizeCanvas,
    laCropCanvas,
    laSTOP
};

enum eCanvasActions{
 caNew,
 caResize,
 caCrop,
 caSTOP

};

    // pipeline default action sequence:
    /*
      Clamping(for artistic brushes)
      Focality(curvature actually)
      Noise
--
      Rotate //inside application cycle
      Scale-disprop  // --non linear scaling applied.
*/
enum ePipelines{
    // clamp focal noise Scale rotate
    plCFNSR, plRS, plSTOP

};

//csstop needed to end cycle!!
/*
class cSmartColor : public QWidget
{

public:
    QColor UseCol;
    float savehue;
    float savesat;
    float savelit;
private:

public slots:
    void SetHueF(float inp);
    void SetSatF(float inp);
    void SetLitF(float inp);
    void LoadAll();
    void SaveAll();
    void SetCol(QColor col);
protected:
signals:


};
*/
//QList <QString> BMnames;



class EGlobals{
public:
  static  QList <QString> PipeNames;
  static QString str1;
  void init();

};

class cParam {
public:
    float basemax;
    float basemin;
    float clipmax;
    float clipmin;
    float crv;
    float outmax; // calcalated according base;
    float outmin;

    float GetCVal(float inp);
    float GetTVal(float inp);
    void Unclip();
    void Tobase();
};


struct sLayerProps{
  //  quint16 layerID;
    qreal op;
    bool visible;
    int blendmode;
    quint8 presop;
    bool droppedup;
    bool droppeddown;
    bool locked;
    quint8 realidx;
    QString layerName;

    QByteArray Serialize();
    void Serialize(QDataStream *ba);
    void DeSerialize(QByteArray src);
    void DeSerialize(QDataStream *ba);
};

struct g_KeyStates{
// 0 = unpressed, 1=hit, 2=pressed
    int Space;
    int LB;
    int RB;

};
struct d_TriFloat{
    qint16 IntVal;
    quint8 FVal;

    qreal GetVal();
    void SetVal(double val);
};

struct d_PointF{
d_TriFloat xpos;
d_TriFloat ypos;

    qreal x();
    qreal y();
    void SetByQPointF(QPointF src);
    QPointF ToPointF();
};

struct d_Brush;
struct d_RealBrush;
struct d_PackedBrush;

struct d_Stroke// : public QObject
{

public:
    d_PointF packpos1;
    d_PointF packpos2;
    QPointF pos1;
    QPointF pos2;
    QPointF pos3; //used only for internal interpolation.
    QPointF pos4; //--''--

    QByteArray Serialize();
    void Serialize(QDataStream *ba);
    void DeSerialize(QByteArray *src);
    void DeSerialize(QDataStream *ba);


};


struct d_StrokePars
{
public:
// everything is in range 0-1;
    qreal Pars[25];
};

struct d_TParam {
    qreal fmax;
    qreal fmin;
    qreal fjit;
    qint8 tmode;

};

struct d_ToolPreset {
    QString name;
    qint8 toolid;
    bool usecolor;
    qint8 compmode;
    qint8 penmode;
    d_TParam pars[20];

};

struct d_BPSettings {
    float clipmin;
    float clipmax;
    float jitter;
  //this seems outdated
    QPainter::CompositionMode compmode;
    bool usecol;
    int ctlid;
};


struct d_PackedBrush{
public:
    d_TriFloat Prad_in;
    d_TriFloat Prad_out;
    quint8 crv;  // curvature
    quint16 resangle;
    quint8 x2y;  //proportions [0-0.5]=[0-1] by x, [0.5-1]=[1-0] by y;
    quint8 scale; //


    quint8 cop;  // clone opacity
    quint8 pwr;
    quint8 sol;  // solidity // rename to dissolvance
    quint8 sol2op; // solidity2opacity mod // rename to consistency.


    quint16 seed;

    quint16 noisex;  //internal
    quint16 noisey;  // internal


    quint8 NoiseID; // noise id;
    quint8 MaskID;
    quint8 pipeID;


  //  QPainter::CompositionMode compmode;
    quint8 bmidx; //blendmode index;
    quint8 noiseidx;
    quint8 preserveop; //preserve opacity
    QRgb col;

};

struct d_RealBrush{
public:
    float rad_in;
    float rad_out;
    qreal opacity;
    double resangle;

    qreal crv;  // curvature
 //   quint16 resangle;
    qreal x2y;  //proportions [0-0.5]=[0-1] by x, [0.5-1]=[1-0] by y;
    qreal scale; //


    qreal cop;  // clone opacity
    qreal pwr;
    qreal sol;  // solidity // rename to dissolvance
    qreal sol2op; // solidity2opacity mod // rename to consistency.


    quint16 seed;

    quint16 noisex;  //internal
    quint16 noisey;  // internal


    quint8 NoiseID; // noise id;
    quint8 MaskID;
    quint8 pipeID;


  //  QPainter::CompositionMode compmode;
    quint8 bmidx; //blendmode index;
    quint8 noiseidx;
    quint8 preserveop; //preserve opacity
    QRgb col;
};

struct d_Brush{
   // struct d_PackedBrush;
 //   struct d_RealBrush;
public:
    d_PackedBrush Pack;
    d_RealBrush Realb;

    QByteArray Serialize();
    void Serialize(QDataStream *ba);
    void DeSerialize(QByteArray *src);
    void DeSerialize(QDataStream *ba);

    void SelfPack();
    void SelfUnpack();
    //QImage Mask;

};
struct d_Action //: public QObject
{
 //   struct d_Brush;
public:
    quint8 ToolID;
    d_Brush Brush;
    quint8 startseed;
    quint8 Noisemode;
    d_Stroke Stroke;
    quint8 layer; // layer from
    QByteArray Serialize();
    void DeSerialize(QByteArray src);
};

struct actionpair{
    d_Action Act;
    QImage *img;
    bool islocal;
};
struct d_Section
{
    d_Stroke Stroke;
    d_Brush BrushFrom;
    d_Brush Brush;
    quint8 BrushID;
    quint8 NoiseID;
quint8 Noisemode;
    quint8 ToolID;
    quint8 startseed;
    quint8 layer; // layer from

    float spacing; // spacing parameter for sections
  //  float spacing_offset; // offset left for proper drawing;
    quint8 scatter;
    quint8 rRadout;
    quint8 rRadrel;
    quint8 rScale;
    quint8 rScaleRel;
    quint8 rAngle;
    quint8 rSpacing;
    quint8 rSpread;
    quint8 rOp;
    quint8 rSol;
    quint8 rSol2;
    quint8 rCrv;
    quint8 rCop;
    quint8 rPwr;
    quint8 rHue;
    quint8 rSat;
    quint8 rLit;

    QByteArray Serialize();
    void DeSerialize(QByteArray src);

};


struct d_LAction{
  quint8 ActID;
  qint16 layer; // layer from
  qint16 layerto;
  quint8 bm;
  float op;
  bool vis;
  QRect rect; //will be used for layer rescaling transformation and etc;
  QByteArray Serialize();
  void DeSerialize(QByteArray src);
};





static QString PathEXE;
static QString PathAPP;



//global stuff
void InitCore();
void resizeImage(QImage *image, const QSize &newSize);
void rescaleImage(QImage *image, const QSize &newSize);
//void resizeImage(QImage *image, const QSize &newSize,QColor fillcol);
float GenCurveF(float val,float midp); //generates blended curve for 0-1 range


qreal Dist2D(QPointF pos1, QPointF pos2);
float AtanXY(float x, float y);
float rad2deg(float rads);
float Discrete(float inp,float range); //makes range discrete by range,like 10,20,30...
bool isnear(float val,float val2,float range);

static quint32 Rnd_Counter;
float Clamp( float a ,float max);
qreal RngConv(qreal inval, qreal inmin, qreal inmax,qreal outmin, qreal outmax);
void MulPos(QPointF *pos1,float mul);
QPoint WHratio(QPoint wh);
//QByteArray SerializeAction(d_Action act);
QRgb QcToRGB(QColor col);
qreal Blend2(qreal from, qreal to, qreal k);
QRgb BlendRGB(QRgb from, QRgb to,qreal k);
QColor BlendQCOL(QColor from, QColor to, qreal k);
#endif // BRUSHES_H

QPointF CalcLastPos(d_Section sect);
// --------------- functions for random;
qreal SeqRnd(quint16 seed,quint16 N,qreal range);
qreal RawRnd(quint16 seed,qreal range);
quint16 RND (quint16 range);
qreal RNDf (qreal range);
QString DSZstring(QByteArray ba);
QByteArray SZstring(QString st);
d_RealBrush BlendBrushes(d_RealBrush Bfrom, d_RealBrush Bto, float k);

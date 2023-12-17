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
#include "BrushEngine/NetBrushStamp.hpp"
#include "BrushEngine/LegacySharedBrush.hpp"
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <qbytearray.h>
#include <QByteArray>
#include <QtWidgets/QApplication>
#include <QCoreApplication>

enum ParameterControlKind {    //Drawing modifiers
   csNone, PenPressure, Velocity, Direction, csRot, csTilt, csRelang, csHtilt, csVtilt,

   csLenpx, csAcc,

   csXtilt, csYtilt, csSTOP, csERASER, // disabled ones go here
   csLen, csCrv, csIdir, csHVdir, csHVrot, csENDPOINT
};
enum ToolKind {
   eBrush, eSmudge, eDisp, eCont, eSTOP, eLine, eEOE
};
enum BlendingMode {  //Blending Modes
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
   // add fancy sqare root normalizd one
   bmSTOP

};
enum LayerActionKind { //Layer Actions
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

enum CanvasActionKind {
   caNew, caResize, caCrop, caSTOP

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
enum BrushPipelineKind {
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



class EGlobals {
   public:
      static QList<QString> PipeNames;
      static QString str1;
      void init();

};

class BrushDialConfig {
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

struct LayerData {
   //  quint16 layerID;
   qreal Opacity;
   bool IsVisible;
   int blendmode;
   quint8 presop;
   bool droppedup;
   bool droppeddown;
   bool IsLocked;
   quint8 realidx;
   QString layerName;

   QByteArray Serialize();
   void Serialize(QDataStream *ba);
   void DeSerialize(QByteArray src);
   void DeSerialize(QDataStream *ba);
};

struct g_KeyStates {
// 0 = unpressed, 1=hit, 2=pressed
   int Space;
   int LB;
   int RB;

};

struct PackedFloat {
   qint16 IntVal;
   quint8 FVal;

   qreal GetVal();
   void SetVal(double val);
};

struct d_PointF {
   PackedFloat xpos;
   PackedFloat ypos;

   qreal x();
   qreal y();
   void SetByQPointF(QPointF src);
   QPointF ToPointF();
};

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

struct d_StrokePars {
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

struct BrushParameterSettings {
   float clipmin;
   float clipmax;
   float jitter;
   //this seems outdated
   QPainter::CompositionMode compmode;
   bool usecol;
   int ctlid;
};

struct ActionData //: public QObject
{
      //   struct BrushData;
   public:
      quint8 ToolID;
      BrushData Brush;
      quint8 startseed;
      quint8 Noisemode;
      d_Stroke Stroke;
      quint8 layer; // layer from
      QByteArray Serialize();
      void DeSerialize(QByteArray src);
};

struct actionpair {
   ActionData Act;
   QImage *img;
   bool islocal;
};

struct StrokeSection {
   d_Stroke Stroke;
   BrushData BrushFrom;
   BrushData Brush;
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

// layer action
struct LayerAction {
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
float GenCurveF(float val, float midp); //generates blended curve for 0-1 range


qreal Dist2D(QPointF pos1, QPointF pos2);
float AtanXY(float x, float y);
float rad2deg(float rads);
float Discrete(float inp, float range); //makes range discrete by range,like 10,20,30...
bool isnear(float val, float val2, float range);

static quint32 Rnd_Counter;
float Clamp(float a, float max);
qreal RngConv(qreal inval, qreal inmin, qreal inmax, qreal outmin, qreal outmax);
void MulPos(QPointF *pos1, float mul);
QPoint WHratio(QPoint wh);
//QByteArray SerializeAction(ActionData act);
QRgb QcToRGB(QColor col);
qreal Blend2(qreal from, qreal to, qreal k);
QRgb BlendRGB(QRgb from, QRgb to, qreal k);
QColor BlendQCOL(QColor from, QColor to, qreal k);
#endif // BRUSHES_H

QPointF CalcLastPos(StrokeSection sect);
// --------------- functions for random;
qreal SeqRnd(quint16 seed, quint16 N, qreal range);
qreal RawRnd(quint16 seed, qreal range);
quint16 RND(quint16 range);
qreal RNDf(qreal range);
QString DSZstring(QByteArray ba);
QByteArray SZstring(QString st);
ClientBrushStamp BlendBrushes(ClientBrushStamp Bfrom, ClientBrushStamp Bto, float k);

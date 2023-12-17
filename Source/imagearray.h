#ifndef IMAGEARRAY_H
#define IMAGEARRAY_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QtWidgets/QWidget>
#include <QTabletEvent>
#include "Brushes.h"
#include <QMoveEvent>
#include <cstddef>
#include "ctl_bparam.h"
#include "BrushEditorPresenter.h"
#include "c_keylink.h"
#include "ctl_BMselector.h"
#include "geomaster.h"
#include "BrushEngine/ClientBrushStamp.hpp"
//class d_Stroke;

enum NavigationMode{
    mNone,
    mPaint,
    mPan,
    mPick,
    mZoom,
    mRadSize,
    mrad2Size,
    mCrv,
    mSTOP
};

class ImageArray : public QWidget
{
    Q_OBJECT

   private:
      ClientBrushStamp *g_Brush;

   public:
    explicit ImageArray(ClientBrushStamp *pBrush, BrushEditorPresenter *brushEditor, c_KeyLink *keyb);
    //void resizeImage(QImage *image, const QSize &newSize);
 //   void DrawBrush(QPointF pos,int rad,float Opacity);

    d_Stroke pstroke;
    c_KeyLink *GlobalKB;
    qint8 locked;
    ctl_BMselector BMsel;
    QImage *ImgTransp;
    //ActionMaster *ActionExecutor;

    QList <QImage> ViewCanvas;
    QList <QImage> DropLayers;
    QList <QImage> DropDownLayers;
    QList <QImage> DropUpLayers;
    QList <QImage *> LayerDump;
    QImage MergedCanvas;
    QList <LayerData> LayerProps;


    QPointF ScrollPos;
    QPointF PPos;
    QPointF PPos3;
    QPointF PPos4;
    QPointF MPos2;
    QPointF MPos3;
    QPointF BPos;
    QPointF BPos2;
    QPointF Btilt;
    QPointF Btilt2;

    QCursor *curZoom;
    QCursor *curDrag;
    QColor VoidColor;

    QSize MasterSize;

    int idletime;
    int activetime;
    //----- viewport
    float ZoomK;
    int Offx;
    int Offy;
    //----- brush cursor params
    int BSize;
    int Brad;
    int Brad2;
    float xtilt;
    float ytilt;
    float ppres;
    float pxtilt;
    float pytilt;

    QPointF LastMousePos;
    d_StrokePars lastspars;
    d_StrokePars currspars;
    int MMode;
    int TMode;
    bool Drawn;
    bool SpacingEN;


    int maxlen;
    int leftlen;
    int maxrlen;
    int leftrlen;
    int corecount;

    bool repainted;
    bool Brepainted;
    bool wasmerged;
    float maxvel;
    float spacing;
    float spdist; //saving dist for spacing

    int Loupesize;

    QTimer *uptimer;


    bool EnablePoly;

    BrushEditorPresenter *BControls;
    Ctl_BParam *RadCtl;
    Ctl_BParam *RadRelCtl;
    Ctl_BParam *ScaleCtl;
    Ctl_BParam *SpacingCtl;
    Ctl_BParam *SpacingJitCtl;
    QRect RepaintRect;
    QRect RepaintiRect;
    int RepaintLayer;
    int ActiveLayer;

    GeoMaster GM;
    QTimer *PolyTimer;
    int PolyTreshold;


public slots:
    void GetBSize(int sz);


    void SaveImg();
    void SaveImg(QString fname);
    void SaveImg(QIODevice *iodev);
    void WriteToBytes(QByteArray *ba);

    void ExportImg(QString fname);



    void SpacingFilter(d_Stroke *strk);
    //void SpacingFilterT(d_Stroke strk,QTabletEvent *tevent);
    void ResetSpacer(QPointF pos);
    void SetLengths(float nl);

    void ParseMovePosF(QPointF pos);
    void ParseMovePos(QPoint pos);
    void ParseStartPosF(QPointF pos);
    void ParseStartPos(QPoint pos);
    void ParseEndPosF(QPointF pos);
    void ParseEndPos(QPoint pos);
    void EndPainting();


    QPointF CalcPos(QPointF fpos);
    QPoint CalcPosI(QPoint fpos);
    QPointF Interpolate(d_Stroke prev,d_Stroke curr);

    void RepaintImgRect(QRect iRect);
    void RepaintWidgetRect(QRect wRect);
    void RepaintMergedRect(QRect iRect);

    void AddLayer();
    void AddLayerAt(int above);
    void MoveLayer(int from, int above);
    bool DelLayer(int num);
    void DropLayer(int num); // straight drop
    void GenDrops(int num); // generate dropdown for layer num
    void GenThumb(int lnum);
    void GenAllThumbs();
    void SetActiveLayer(int num);
    void SetLOp(int layer, float op);
    void SetLBm(int layer, int bm);
    void SetLvis(int layer,bool vis);
    void DupLayer(int layer);
    void AddZeroLayer(QSize sz,QColor bgcol);
    void ExecLayerAction(LayerAction lact);
  //  void ExecCanvasAction(d_CanvasAction cact);
    //void DupActiveLayer();
    d_StrokePars InterpolateSpars(d_StrokePars current, d_StrokePars last,qreal kblend);

    void MergeAll();
    //void ProjectDown(int downto);
    //void ProjectUp(int downto);

    void FitScreen();
    void TouchScreen();
    void ActualPixels();

    void SetLock (qint8 lk);

    void ResizeScene(QSize size,int stretchp);
    void DrawPoly();

    QImage GetCollapsedRect(QRect wRect);


signals:
    void SendPoly(QPolygonF Poly);
    void SendStroke(d_Stroke STRK);
    void SendTStroke(d_Stroke STRK,d_StrokePars);
    void SendTStroke2(d_Stroke STRK,d_StrokePars spnext,d_StrokePars spprev);
    void SendColor(QColor col);
    void MouseOut();
    void MouseIn();
    void SendNewB(QPoint centralpos, int rad,qreal tiltx, qreal tilty);

    void SendThumb(int layer,QImage img);
    void SendNewLayer(QImage img);
    void SendLayer(int at,QImage img);
    void SendLAction(LayerAction lact);
    void AskSaveLog(QString Fname);
    void AskOpenLog(QString Fname);
    void AskResetPos();

protected:
   void paintEvent(QPaintEvent *event);
   void tabletEvent(QTabletEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseDoubleClickEvent(QMouseEvent *event);
   void moveEvent(QMoveEvent *event);
   void resizeEvent(QResizeEvent *event);
   void enterEvent(QEvent *event);


private:

    QImage RealCanvas;
    QImage RadCanvas;

//    QPainter ViewPainter(&ViewCanvas);
 //   QPainter RealPainter(&RealCanvas);
 //   QPainter RadPainter(&RadCanvas);

};

#endif // IMAGEARRAY_H

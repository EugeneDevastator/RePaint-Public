#ifndef BCTL_DBLSLIDER_H
#define BCTL_DBLSLIDER_H

#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QLinearGradient>
#include <QColor>
#include <QImage>
#include "bctl_imagewidget.h"
#include "brushes.h"


class bctl_DblSlider : public bctl_ImageWidget
{
    Q_OBJECT

public:
    explicit bctl_DblSlider(QWidget *parent = 0);
    /*cParam VisParam;
    cParam Param;*/
    int ActivePick;
    int Range;
    float jitter;
    qreal DsRange;
    QLinearGradient *grad;
    double clipminF;
    double clipmaxF;
    int orient;
    int sliderrad;
    QColor MaxCol;
    QColor MinCol;

    QColor shade;
    QColor hlite;
    QColor midtone;



public slots:
        void Redraw();
        void change();
        void ParsePoint(QPoint pos);
        void GetVals(float maxf,float minf, float jit);
        void DrawFrame(QImage* img,QPainter *pnt, int x, int y, int w, int h,QColor SHD,QColor HL);
        void DrawFrame(QImage* img,QPainter *pnt,QRect rect,QColor SHD,QColor HL);
protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void resizeEvent(QResizeEvent *event);
        void tabletEvent(QTabletEvent *event);
  //      void paintEvent(QPaintEvent *event);
signals:
        void Repainted();
        void ValChange(float maxf);
        void ValConfirm(float maxf);
        void AllValChange(float maxf,float minf, float jit);


};

#endif // BCTL_DBLSLIDER_H

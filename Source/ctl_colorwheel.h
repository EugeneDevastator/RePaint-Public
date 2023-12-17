#ifndef CTL_COLORWHEEL_H
#define CTL_COLORWHEEL_H

#include <QtWidgets/QWidget>
#include <QImage>
#include <QColor>
#include <QPoint>
#include "bctl_imagewidget.h"
#include "Brushes.h"
#include "BrushEngine/LegacySharedBrush.hpp"

class ctl_ColorWheel : public bctl_ImageWidget
{
    Q_OBJECT
public:
    BrushData *g_Brush;
    explicit ctl_ColorWheel();
    //QImage ViewImage;
    b_SmartColor gPaintColor;
    b_SmartColor gEraseColor;
    QColor GetColor();
    QColor LColor;
    QColor RColor;
    float rad;
    int SelMode;
    int KMode;
    bool SwState;
    float savehue;
    float savesat;
    float saveval;

    void DrawSliders(QColor col);
public slots:
    void SetColor(QColor col);


protected:
void paintEvent(QPaintEvent *event);
void Redraw();
   void DrawHUEWheel();
   void DrawLSWheel();
   void DrawVSWheel();
   void DrawSwatch();

   void mouseMoveEvent(QMouseEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);

   void keyPressEvent(QKeyEvent *event);
   void keyReleaseEvent(QKeyEvent *event);

signals:
   void SendColor(QColor col);
public slots:


};

#endif // CTL_COLORWHEEL_H

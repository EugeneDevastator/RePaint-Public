#ifndef BRUSHSIZER_H
#define BRUSHSIZER_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QtWidgets/QWidget>
#include "ArtMaster.h"
#include "Brushes.h"
#include "BrushEngine/ClientBrushStamp.hpp"
#include "BrushEngine/LegacySharedBrush.hpp"

class BrushSizer : public QWidget
{
    Q_OBJECT
public:

    explicit BrushSizer(QWidget *parent = 0);
    int rad_out;
    int rad_in;
    ArtMaster *ARTM;
    float saverel;
    float offang;

    QImage BrushImg;
    ClientBrushStamp *g_Brush;
   void Redraw();
public slots:
//    void GetBrush(BrushData br);
signals:
   void SendBrush(BrushData br);
   void SendSize(int sz);
   void SendRadRel(float rel);

protected:
   void paintEvent(QPaintEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void mousePressEvent(QMouseEvent *event);
private:
    int Mode;


};



#endif // BRUSHSIZER_H

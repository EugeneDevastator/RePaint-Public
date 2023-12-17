#ifndef CTL_FASTBRUSH_H
#define CTL_FASTBRUSH_H

#include <QtWidgets/QWidget>
#include <QImage>
#include "Brushes.h"
#include "ArtMaster.h"
#include "BrushEngine/BrushStamp.hpp"
#include "BrushEngine/LegacySharedBrush.hpp"

class ctl_FastBrush : public QWidget
{
    Q_OBJECT
public:
    explicit ctl_FastBrush(ClientBrushStamp *brush, QWidget *parent = 0);
    QImage *ViewCanvas;
    ClientBrushStamp *g_Brush;
      int midp;
      float ang;
      float *ZoomK;
      float *crvmax;
      float *rrel;
      int brushRegulationMode;
      int lmode;
      float angle;
      float scalerel;
      int anglerad;
      ArtMaster *ARTM;
        BrushData *DBR;
      QImage *Bpreview;
      QImage *BG;
      bool gotbrush;
      bool repainted;


signals:
      void SendRel(float val);
      void SendCrv(float val);
      void AskBrush();
public slots:
      void ParsePos(QPoint pos);
      void ReBrush(ActionData act);
      void ForcePaint();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void tabletEvent(QTabletEvent *event);

};

#endif // CTL_FASTBRUSH_H

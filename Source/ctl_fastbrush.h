#ifndef CTL_FASTBRUSH_H
#define CTL_FASTBRUSH_H

#include <QtWidgets/QWidget>
#include <QImage>
#include "Brushes.h"
#include "ArtMaster.h"

class ctl_FastBrush : public QWidget
{
    Q_OBJECT
public:
    explicit ctl_FastBrush(QWidget *parent = 0);
    QImage *ViewCanvas;
    ClientBrush *g_Brush;
      int midp;
      float ang;
      float *ZoomK;
      float *crvmax;
      float *rrel;
      int mmode;
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

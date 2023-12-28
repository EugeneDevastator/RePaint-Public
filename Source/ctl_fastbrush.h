#ifndef CTL_FASTBRUSH_H
#define CTL_FASTBRUSH_H

#include <QtWidgets/QWidget>
#include <QImage>
#include "Brushes.h"
#include "ArtMaster.h"
#include "BrushEngine/ClientBrushStamp.hpp"
#include "BrushEngine/LegacySharedBrush.hpp"
#include "imagearray.h"

class ctl_FastBrush : public QWidget
{
    Q_OBJECT
   public:
    explicit       ctl_FastBrush(BrushEditorPresenter *brushParams, ImageArray *imageArray, QWidget *parent);
    QImage *ViewCanvas;
      int midpoint;
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
      BrushEditorPresenter *BrushParams;

signals:
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

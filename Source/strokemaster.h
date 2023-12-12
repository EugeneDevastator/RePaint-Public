#ifndef STROKEMASTER_H
#define STROKEMASTER_H

#include <QtWidgets/QWidget>
#include "Brushes.h"
#include "ArtMaster.h"
#include "BrushEditorPresenter.h"
#include "pnl_layers.h"
#include "imagearray.h"
#include "ArtThread.h"




// ------------------- MAIN CLASS -----------------

class StrokeMaster : public QWidget
{
    Q_OBJECT
public:
    explicit StrokeMaster(BrushEditorPresenter *bcont, LayersPanelPresenter *lyrp, ImageArray *iar, QWidget *parent=0);
    

    QSize baseSize;
    QSize dispSize;
    quint8 baselcount; // baselayer count?
    bool isNew;
    bool isLogging;
    bool executing;

    ArtThread *ARTM;
    BrushEditorPresenter *BC;
    LayersPanelPresenter *LP;
    ImageArray *MainImage;

    QPointF lastpos;
    qreal offset;
    BrushData prevBrush;
    QList <StrokeSection> LocalSects;
    QList <StrokeSection> NetSects;
    QTimer *uptimer;

public slots:
   // void callupdate();
  //  void ExecSection(StrokeSection Sect, bool local);
    //void ExecSection(StrokeSection Sect, bool local);
    void ExecSectionNet(StrokeSection Sect);
    void ResetLocalPos();
    void GetRawStroke(d_Stroke Strk,d_StrokePars currpars,d_StrokePars lastpars);

    // logger routines


    //void SaveLog();
signals:
    void SendReadySect(StrokeSection);
    void SendDots(QList <actionpair>,int);// list +
};

#endif // STROKEMASTER_H

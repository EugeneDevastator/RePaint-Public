#ifndef STROKEMASTER_H
#define STROKEMASTER_H

#include <QtWidgets/QWidget>
#include "Brushes.h"
#include "ArtMaster.h"
#include "pnl_bcontorls.h"
#include "pnl_layers.h"
#include "imagearray.h"
#include "ArtThread.h"




// ------------------- MAIN CLASS -----------------

class StrokeMaster : public QWidget
{
    Q_OBJECT
public:
    explicit StrokeMaster(pnl_bcontorls *bcont, pnl_Layers *lyrp, ImageArray *iar, QWidget *parent=0);
    

    QSize baseSize;
    QSize dispSize;
    quint8 baselcount; // baselayer count?
    bool isNew;
    bool isLogging;
    bool executing;

    ArtThread *ARTM;
    pnl_bcontorls *BC;
    pnl_Layers *LP;
    ImageArray *MainImage;

    QPointF lastpos;
    qreal offset;
    d_Brush prevBrush;
    QList <d_Section> LocalSects;
    QList <d_Section> NetSects;
    QTimer *uptimer;

public slots:
   // void callupdate();
  //  void ExecSection(d_Section Sect, bool local);
    //void ExecSection(d_Section Sect, bool local);
    void ExecSectionNet(d_Section Sect);
    void ResetLocalPos();
    void GetRawStroke(d_Stroke Strk,d_StrokePars currpars,d_StrokePars lastpars);

    // logger routines


    //void SaveLog();
signals:
    void SendReadySect(d_Section);
    void SendDots(QList <actionpair>,int);// list +
};

#endif // STROKEMASTER_H

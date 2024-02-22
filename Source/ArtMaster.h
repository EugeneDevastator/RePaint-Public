/*
  Copyright 2012 (C) Yauheni Bolshakov

*/


#ifndef ARTMASTER_H
#define ARTMASTER_H

#include <QtCore>
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QObject>
#include <QThread>
#include "brushes.h"
#include "ctl_BMselector.h"
#include "LayerActionQueue.h"
#include "BrushEngine/LegacySharedBrush.hpp"

class ArtMaster : public QObject
{
    Q_OBJECT
public:
    explicit ArtMaster(QObject *parent = 0);

    //QList <QString> BMnames;
    //QList <QPainter::CompositionMode> BMmodes;
ctl_BMselector BMsel;




     QList <QImage> NoiseProfiles;
     QList <QImage> Masks;
  int aamn;
  int adist;
  bool newac;


public slots:


    void DoAction(QImage *img, ActionData Act, bool local);
    void DoActionList(QImage *img, dotList *actlist, bool local);
    void DrawBrush(QImage *img, d_Stroke STRK, BrushData BRSH);
    void DrawBrushDev(QPaintDevice *img, d_Stroke STRK, BrushData BRSH);
    void DrawDisp(QImage *img, d_Stroke STRK, BrushData BRSH);
    void DrawSmudge(QImage *img, d_Stroke STRK, BrushData BRSH);
    void DrawLine(QImage *img, d_Stroke STRK, BrushData BRSH);
    void LvlBrush(QImage *img, d_Stroke STRK, BrushData BRSH);

    QImage GenBMask(BrushData BRSH, float fx=0, float fy=0);
    QImage* SubPixelate(QImage *Bimg,float fx,float fy);
    void GenSolidity(QImage *img, float sol, float sol2op);
    void GenSolidityP(QImage *img, float sol, float sol2op, qint16 noisex, qint16 noisey);
    void GenFocal(QImage *img, float fop);
    void GenFocalInv(QImage *img, float fop);
    //void GenFocal(QImage *img, float top, float min);
    void GenClamp(QImage *img, float top,float min);

  //  void DrawSection(QImage *img,StrokeSection Act, bool local=true);

    float GenCurveF(float val,float midp);
signals:
     void SendMsg(QString msg);
     void SendDirtyRect(QRect irect);
     void AskRepaint();
     void ConfirmAction(ActionData act);
     void AffirmAction(ActionData aAct);


private:
    int SBMax;
    QList <QImage> DispBrushes;
    QList <QImage> SmallBrushes;
    QImage crPinch;
    QImage crBub;
    QImage crCont;
    QImage crDecont;
    QImage crClamp;
    QImage br512;
    QImage br512e;

};

#endif // ARTMASTER_H

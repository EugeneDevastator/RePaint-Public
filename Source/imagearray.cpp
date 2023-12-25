#include "imagearray.h"
#include "BrushEngine/ClientBrushStamp.hpp"
#include "ReMath/ReMath.hpp"
#include <QtGui>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QFileDialog>
#include <QRect>
#include <QFile>

ImageArray::ImageArray(ClientBrushStamp *pBrush, BrushEditorPresenter *brushEditor, c_KeyLink *keyb) :
        g_Brush(pBrush), GlobalKB(keyb), BControls(brushEditor) {
    SpacingCtl = brushEditor->CtlSpc;
    SpacingJitCtl = brushEditor->CtlSpcJit;
    RadCtl = brushEditor->CtlRad;
    ScaleCtl = brushEditor->CtlScale;
    //MainImage->InternalBrush = Brush;
    //MainImage->SpacingCtl = BControls->CtlSpc;
    //MainImage->SpacingJitCtl = BControls->CtlSpcJit;
    //MainImage->RadCtl = BControls->CtlRad;
    //MainImage->ScaleCtl = BControls->CtlScale;
    this->setCursor(Qt::CrossCursor);
    this->setMouseTracking(true);
//Brush=new BrushData;
    //ViewCanvas = new QImage(120,120,Format_RGB32)
    /*
    QImage BaseImg;
    ViewCanvas.append(BaseImg);
    resizeImage(&ViewCanvas[0], QSize(2400, 2400));

    LayerData lp;
    lp.Opacity=1;
    LayerProps.append(lp);
*/

    Loupesize = 85;
    locked = 0;
    idletime = 90;
    activetime = 25;
    Btilt = QPointF(0, 0);
    Btilt2 = QPointF(0, 0);
    ZoomK = 1;
    Offx = this->geometry().width() * 0.5 + 51;
    Offy = this->geometry().height() * 0.5 + 51;
    MMode = 0;
    TMode = 0;
    spacing = 10;
    spdist = 0;
    maxvel = 20; //in pixels.
    maxlen = 40;//40 brushes, its is relative to spacing;
    maxrlen = 140;//140 pixels
    SpacingEN = false;
    this->setAttribute(Qt::WA_OpaquePaintEvent);
    //this->setAttribute(Qt::WA_NoSystemBackground);
    VoidColor = Qt::gray;
    repainted = true;
    RepaintLayer = 0;
    wasmerged = 0;
    //QBitmap("//res/curZoom.png")
    curZoom = new QCursor(QPixmap(RESPATH + "/res/curZoom.png"));

    Brepainted = true;
    corecount = QThread::idealThreadCount();
    uptimer = new QTimer;
    connect(uptimer, SIGNAL(timeout()), this, SLOT(update()), Qt::QueuedConnection);

//if (corecount==1)
    uptimer->start(idletime);

    QDesktopWidget screen;
    ImgTransp = new QImage(QSize(screen.width(), screen.height()), QImage::Format_ARGB32);
    QPainter painter(ImgTransp);
    QColor whites = QColor::fromRgbF(0.95, 0.95, 0.95);
    QColor blacks = QColor::fromRgbF(0.6, 0.6, 0.6);
    int rsize = 16;
    painter.setPen(Qt::NoPen);
    painter.setBrush(whites);
    painter.drawRect(0, 0, rsize * 2, rsize * 2);
    painter.setBrush(blacks);
    painter.drawRect(0, 0, rsize, rsize);
    painter.drawRect(rsize, rsize, rsize, rsize);
    for (int i = 1; i < screen.width() / (rsize * 2); i++)
        painter.drawImage(QPoint(rsize * i * 2, 0), *ImgTransp);
    for (int i = 1; i < screen.height() / (rsize * 2); i++)
        painter.drawImage(QPoint(0, rsize * i * 2), *ImgTransp);


/*  Initializing poly timer
*/
    EnablePoly = false;
    PolyTreshold = (500);
    PolyTimer = new QTimer;
//PolyTimer->setSingleShot(true);
    connect(PolyTimer, SIGNAL(timeout()), this, SLOT(DrawPoly()), Qt::QueuedConnection);


}

void ImageArray::DrawPoly() {
    PolyTimer->stop();
    QPolygonF poly = GM.IssuePoly();
    emit SendPoly(poly);

}

void ImageArray::SetLengths(float nl) {
    maxlen = nl / 10;
    maxrlen = nl;
}

void ImageArray::RepaintWidgetRect(QRect wRect) {
    int hw = floor(this->geometry().width() * 0.5);
    int hh = floor(this->geometry().height() * 0.5);
/*
    Offx=qMax((int)(-this->geometry().width()*0.5/ZoomK+51),Offx);
    Offx=qMin((int)((ViewCanvas[0].width()+hw/ZoomK))-51,Offx);
    Offy=qMax((int)(-this->geometry().height()*0.5/ZoomK+51),Offy);
    Offy=qMin((int)((ViewCanvas[0].height()+hh/ZoomK))-51,Offy);
*/
    /*
    Offx=(Offx);
    Offy=(Offy);
    wRect.setX((wRect.x()));
    wRect.setY((wRect.y()));
    wRect.setWidth((wRect.width()));
    wRect.setHeight((wRect.width()));
*/

    /*good algo
    QRect iRect=QRect(wRect.x()/ZoomK+Offx-hw/ZoomK,
                         wRect.y()/ZoomK+Offy-hh/ZoomK,
                         (wRect.width()/ZoomK),
                         (wRect.height()/ZoomK)
                         );
*/
    QRect iRect = QRect(floor(wRect.x() / ZoomK + Offx - hw / ZoomK),
                        floor(wRect.y() / ZoomK + Offy - hh / ZoomK),
                        ceil(wRect.width() / ZoomK),
                        ceil(wRect.height() / ZoomK)
    );
    RepaintImgRect(iRect);
    /*
    if (repainted) {
        RepaintRect=wRect;
        RepaintiRect=iRect;
        repainted=false;
    }
    else {
        RepaintRect=wRect|RepaintRect;
        RepaintiRect=iRect|RepaintiRect;
    }
    */

//this->update();
}

void ImageArray::RepaintImgRect(QRect iRect) {
    int hw = floor(this->geometry().width() * 0.5);
    int hh = floor(this->geometry().height() * 0.5);
/*
    Offx=qMax((int)(+hw*ZoomK+51),Offx);
    Offx=qMin((int)((ViewCanvas[0].width()-hw*ZoomK))-51,Offx);
    Offy=qMax((int)(+hh*ZoomK+51),Offy);
    Offy=qMin((int)((ViewCanvas[0].height()-hh*ZoomK))-51,Offy);
*/
    /*
    Offx=floor(Offx);
    Offy=floor(Offy);
*/

    iRect.setX(floor(iRect.x()));
    iRect.setY(floor(iRect.y()));
    iRect.setWidth(ceil(iRect.width()));
    iRect.setHeight(ceil(iRect.height()));

    //QRect dirtyRect(
    QRect wRect(
            (floor((iRect.x() - Offx) * ZoomK) + hw),
            (floor((iRect.y() - Offy) * ZoomK) + hh),
            ceil(iRect.width() * ZoomK),
            ceil(iRect.height() * ZoomK)
    );
    //RepaintWidgetRect(dirtyRect);
    if (repainted) {
        RepaintRect = wRect;
        RepaintiRect = iRect;
        repainted = false;
    } else {
        RepaintRect = wRect | RepaintRect;
        RepaintiRect = iRect | RepaintiRect;
    }

    /*
       if (repainted) {
       RepaintRect=dirtyRect;
       RepaintiRect=iRect;
       repainted=false;
       }
       else{
        RepaintRect=dirtyRect|RepaintRect;
        RepaintiRect=iRect|RepaintiRect;
       }*/

    //   this->update();
}

void ImageArray::RepaintMergedRect(QRect iRect) { // repaints all layers in iRect on Merged canvas
    QPainter painter(&MergedCanvas);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    painter.drawRect(iRect); // clear active rect;

    for (int i = 0; i < ViewCanvas.count(); i++) {
        if (LayerProps[i].IsVisible) {
            painter.setOpacity(LayerProps[i].Opacity);
            painter.setCompositionMode(BMsel.GetCMfromIndex(LayerProps[i].blendmode));
            painter.drawImage(iRect, ViewCanvas[i], iRect);

        }
    }

}

void ImageArray::paintEvent(QPaintEvent *event) {
    if (ViewCanvas.count() > 0) {

//int hw=this->geometry().width()*0.5;
//int hh=this->geometry().height()*0.5;
/*
Offx=qMax((int)(-this->geometry().width()*0.5/ZoomK+51),Offx);
Offx=qMin((int)((ViewCanvas[0].width()+hw/ZoomK))-51,Offx);
Offy=qMax((int)(-this->geometry().height()*0.5/ZoomK+51),Offy);
Offy=qMin((int)((ViewCanvas[0].height()+hh/ZoomK))-51,Offy);
*/

        //bool Brep=Brepainted;

        Offx = floor(Offx);
        Offy = floor(Offy);

        QPainter painter(this);
        //QRect dirtyRect = event->rect();
        //QRect dirtyRect = event->region().boundingRect();
        //painter.drawImage(dirtyRect, ViewCanvas, QRect(dirtyRect.x()+Offx-hw/ZoomK,dirtyRect.y()+Offy-hh/ZoomK,dirtyRect.width()/ZoomK,dirtyRect.height()/ZoomK));



        //  if (!Brep)
        { //erase old brush
            painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
            painter.setOpacity(1);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(Qt::white);
            painter.drawEllipse(BPos2, Brad2, Brad2);
            painter.drawLine(BPos2, QPointF(BPos2.x() + (Btilt2.x() * 2 - 1) * Brad2 * 2,
                                            BPos2.y() + (Btilt2.y() * 2 - 1) * Brad2 * 2));

        }
        // repaint main if needed


        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.setBrush(VoidColor);
        painter.setPen(Qt::NoPen);

        if (locked > 0) {
            //      RepaintWidgetRect(event->rect());
        }

        if (!repainted) {



/*

        int w1=0;
        int h1=0;
        int w2=0;
        int h2=0;
        if (RepaintiRect.x()<0) w1=abs(RepaintiRect.x())*ZoomK;
        if (RepaintiRect.y()<0) h1=abs(RepaintiRect.y())*ZoomK;
        if (RepaintiRect.topRight().x()>ViewCanvas[0].width()) w2=abs(RepaintiRect.topRight().x()-ViewCanvas[0].width())*ZoomK;
        if (RepaintiRect.bottomRight().y()>ViewCanvas[0].height()) h2=abs(RepaintiRect.bottomRight().y()-ViewCanvas[0].height())*ZoomK;

     //rects bounding image may need them later
     // maybe by intersection function can clip them to area
        painter.drawRect(0,0,w1,this->height());
        painter.drawRect(w1,0,this->width(),h1);
        painter.drawRect(this->width()-w2,0,w2,this->height());
        painter.drawRect(0,this->height()-h2,this->width(),h2);
        */

            painter.drawImage(RepaintRect, *ImgTransp, RepaintRect);

            // for (int i=RepaintLayer;i<ViewCanvas.count();i++)                                    // repaint layers to the top;


            // if (!wasmerged) {
            for (int i = ViewCanvas.count() - 1; i >= 0; i--) {

                if (LayerProps[i].IsVisible) {
                    painter.setOpacity(LayerProps[i].Opacity);
                    painter.setCompositionMode(BMsel.GetCMfromIndex(LayerProps[i].blendmode));
                    painter.drawImage(RepaintRect, ViewCanvas[i], RepaintiRect);
                }

            }
            //  }
/*
        else {
            RepaintMergedRect(RepaintiRect);
            painter.setOpacity(1);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.drawImage(RepaintRect, MergedCanvas, RepaintiRect);
            wasmerged=true;
        }*/


        }

        //   if (!Brep)
        { //paint new brush;
            painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
            painter.setOpacity(1);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(Qt::white);
            painter.drawEllipse(BPos, Brad, Brad);
            painter.drawLine(BPos, QPointF(BPos.x() + (Btilt.x() * 2 - 1) * Brad * 2,
                                           BPos.y() + (Btilt.y() * 2 - 1) * Brad * 2));
            Brad2 = Brad;
            BPos2 = BPos;
            Btilt2 = Btilt;

            Brepainted = true;
        }

        float rad = g_Brush->rad_out * ZoomK * RadCtl->GetModValue(lastspars) * ScaleCtl->GetModValue(lastspars);

        if (MMode == mRadSize) rad = g_Brush->rad_out * ZoomK;//*ScaleCtl->Slider->clipmaxF;
        if (rad < 1) rad = g_Brush->rad_out * ZoomK;

        //painter.drawText(25,25,QString::number(dirtyRect.width())+" : "+QString::number(dirtyRect.height()));

        if (MMode == mrad2Size) {
            rad = g_Brush->rad_in * ZoomK * RadCtl->GetModValue(lastspars) * ScaleCtl->GetModValue(lastspars);
            if (rad < 1) rad = g_Brush->rad_out * ZoomK;
            painter.drawEllipse(BPos, rad, rad);
        }
        if (MMode == mCrv) {
            rad = (1 - BControls->CtlCrv->GetMaxValue()) * 0.5 * g_Brush->rad_out * ZoomK *
                  RadCtl->GetModValue(lastspars) * ScaleCtl->GetModValue(lastspars);
            if (rad < 1) rad = g_Brush->rad_out * ZoomK;
            painter.drawEllipse(BPos, rad, rad);
        }
        if (MMode == mPick) {
            //   painter.setRenderHint(QPainter::SmoothPixmapTransform,false);
            //painter.drawImage(BPos.x(),BPos.y(),ViewCanvas,BPos.x()-5,BPos.y()-5,5,5);
            //painter.drawRect(QRect(BPos.x()-91,BPos.y()-91,86,86));


            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            int cellcount = 3;

            QImage Loupe;
            resizeImage(&Loupe, QSize(Loupesize, Loupesize));
            QPainter Lpnt(&Loupe);
            Lpnt.setBrush(QColor::fromRgbF(1, 1, 1, 0));
            Lpnt.setCompositionMode(QPainter::CompositionMode_Source);
            Lpnt.drawRect(0, 0, Loupesize, Loupesize);

            Lpnt.setCompositionMode(QPainter::CompositionMode_SourceOver);
            if (!wasmerged) {

                //Lpnt.setCompositionMode(QPainter::CompositionMode_Source);

                for (int i = ViewCanvas.count() - 1; i >= 0; i--) {

                    if (LayerProps[i].IsVisible) {
                        Lpnt.setOpacity(LayerProps[i].Opacity);
                        Lpnt.setCompositionMode(BMsel.GetCMfromIndex(LayerProps[i].blendmode));
                        Lpnt.drawImage(QRect(0, 0, Loupesize, Loupesize), ViewCanvas[i],
                                       QRect(PPos4.x() - cellcount + 2, PPos4.y() - cellcount + 2, cellcount,
                                             cellcount));
                        //Lpnt.setCompositionMode(QPainter::CompositionMode_SourceOver);
                    }

                }
            }
            QImage ViewLoupe;
            resizeImage(&ViewLoupe, Loupe.size());
            QPainter VLpnt(&ViewLoupe);
            VLpnt.setCompositionMode(QPainter::CompositionMode_Source);
            VLpnt.setPen(Qt::black);
            VLpnt.setBrush(QColor::fromRgbF(1.0, 1.0, 1.0, 1.0));
            VLpnt.drawRect(ViewLoupe.rect());
            VLpnt.setCompositionMode(QPainter::CompositionMode_SourceOver);
            VLpnt.drawImage(0, 0, Loupe);

            //Lpnt.drawImage(QRect(BPos.x()-LoupeSize-5,BPos.y()-LoupeSize-5,LoupeSize,LoupeSize), ViewCanvas[i], QRect(PPos4.x()-cellcount+2,PPos4.y()-cellcount+2,cellcount,cellcount));
            painter.drawImage(BPos.x() - Loupesize - 5, BPos.y() - Loupesize - 5, ViewLoupe);
            painter.setPen(Qt::white);
            //  painter.setPen(QPen(Qt::SolidLine));
            painter.setBrush(Qt::NoBrush);
            painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
            painter.setOpacity(0.3);
            painter.drawRect(BPos.x() - Loupesize - 5 + 1, BPos.y() - Loupesize - 5 + 1, ViewLoupe.width() - 2,
                             ViewLoupe.height() - 2);

            SendColor(QColor::fromRgb(Loupe.pixel(QPoint(Loupesize * 0.5, Loupesize * 0.5))));
            BPos2 = BPos;
            //  painter.drawImage(QRect(BPos.x()-90,BPos.y()-90,85,85),ViewCanvas[0],QRect(PPos4.x()-1,PPos4.y()-1,3,3));
        }
    }
    //  if (MMode==5)  {} //fast brush resizing.



    repainted = true;
}

void ImageArray::ResetSpacer(QPointF pos) {
    PPos = pos;
    PPos3 = pos;
    PPos4 = pos;
    spdist = 0;
}

void ImageArray::SpacingFilter(d_Stroke *strk) {

    float stdist = Dist2D(strk->pos1, strk->pos2);
    float tdist = stdist + spdist;

    //input parameters grabbing::::::::::
    {
        currspars.Pars[Velocity] = RngConv(stdist, 0, maxvel, 0, 1);
        // stpars.Pars[Velocity]=lastspars.Pars[Velocity]*0.5;
        float dir = AtanXY((strk->pos1.x() - strk->pos2.x()), strk->pos1.y() - strk->pos2.y());
//    float dir2=AtanXY((strk.pos2.x()-strk.pos3.x()),strk.pos2.y()-strk.pos3.y());
        QVector2D Hvv(strk->pos1.x() - strk->pos2.x(), strk->pos1.y() - strk->pos2.y());
        QVector2D Hvv2(strk->pos2.x() - strk->pos3.x(), strk->pos2.y() - strk->pos3.y());
        Hvv.normalize();
        Hvv2.normalize();
        float dcos = Hvv.dotProduct(Hvv, Hvv2);
        currspars.Pars[csCrv] = RngConv(dcos, 0.8, 1, 0, 1);
        currspars.Pars[Direction] = RngConv(dir, -M_PI, M_PI, 0, 1);
        float relang = fabs(currspars.Pars[Direction] - currspars.Pars[csRot]);
        if (relang > 0.5) relang = 1 - relang;
        relang = relang *
                 2;                                                                            //  //  //  //  //  //  spacing   f i l t e r
        relang = 1 - fabs(relang - 0.5) * 2;
        currspars.Pars[csRelang] = relang;
        currspars.Pars[csHVdir] = fabs(Hvv.x() / Hvv.length());
        currspars.Pars[csLen] = RngConv(leftlen, 0, maxlen, 0, 1);
        currspars.Pars[csLenpx] = RngConv(leftrlen, 0, maxrlen, 0, 1);
        currspars.Pars[csAcc] = 1 - fabs(currspars.Pars[Velocity] - lastspars.Pars[Velocity]);
        currspars.Pars[csAcc] = RngConv(currspars.Pars[csAcc], 0.7, 1, 0, 1);
        // stpars.Pars[csAcc]=(stpars.Pars[csAcc]+lastspars.Pars[csAcc])*0.5;

        currspars = InterpolateSpars(currspars, lastspars, 0.5);
    }

    QRect r1;
    QRect r2;
    int rad = g_Brush->rad_out * ScaleCtl->GetModValue(currspars) * (RadCtl->GetModValue(currspars));
    r1.setTopLeft(strk->pos1.toPoint() - QPoint(rad, rad));
    r1.setBottomRight(strk->pos1.toPoint() + QPoint(rad, rad));
    r2.setTopLeft(strk->pos2.toPoint() - QPoint(rad, rad));
    r2.setBottomRight(strk->pos2.toPoint() + QPoint(rad, rad));

    spacing = ((SpacingCtl->GetModValue(currspars) * SpacingCtl->GetModValue(currspars)) * g_Brush->rad_out *
               ScaleCtl->GetModValue(currspars) * (RadCtl->GetModValue(currspars)));
    spacing = qMax(spacing, (float) 1);
    if (SpacingEN) {
        if (tdist > spacing) {

            float lft = (spacing - spdist);
            int acnt = floor((stdist - spacing + spdist) / spacing);
            float drange = acnt * spacing;
            float dx = strk->pos1.x() - strk->pos2.x();
            float dy = strk->pos1.y() - strk->pos2.y();
            float x2r = dx / stdist;
            float y2r = dy / stdist;
            float offx = lft * x2r;//dx*offmul;
            float offy = lft * y2r;//dy*offmul;
            QPointF tpos;
            QPointF tpos2;
            tpos = strk->pos2;
            tpos2 = strk->pos1;
            float rnflw;
            float rnside;
            float rrang = SpacingJitCtl->GetModValue(currspars) * g_Brush->rad_out * ScaleCtl->GetModValue(currspars) *
                          RadCtl->GetModValue(currspars);
            d_Stroke outstrk;
            QPointF outPPos = PPos;

            for (int i = 0; i <= acnt; i++) {
                leftlen -= 1;
                leftrlen -= spacing;
                currspars.Pars[csLen] = RngConv(leftlen, 0, maxlen, 0, 1);
                currspars.Pars[csLenpx] = RngConv(leftrlen, 0, maxrlen, 0, 1);

                rnflw = (qrand() / 32767.0 * rrang * 2) - rrang;
                rnside = 0.0;//(qrand()/32767.0*rrang*2)-rrang;

                strk->pos1.setX(tpos.x() + (spacing * i * x2r) + offx);
                strk->pos1.setY(tpos.y() + (spacing * i * y2r) + offy);

                strk->pos2 = PPos;
                outstrk = *strk;

                outstrk.pos1.setX(outstrk.pos1.x() - rnflw * y2r + rnside * x2r);
                outstrk.pos1.setY(outstrk.pos1.y() + rnflw * x2r + rnside * y2r);
                outstrk.pos2 = outPPos;
                emit SendTStroke(outstrk, currspars);

                //  strk->pos3=strk->pos2;
                strk->pos2 = strk->pos1;
                PPos = strk->pos1;
                outPPos = outstrk.pos1;
                //    PPos3=strk->pos2;


                // ---------- MOUSE HERE!

                //strk.pos2=tpos2;
            }
            strk->pos2 = tpos;

            spdist = stdist - drange - lft;//Dist2D(strk.pos1,strk.pos2);

        } else spdist += Dist2D(strk->pos1, strk->pos2);
    } else { // spacing is off
        leftrlen -= Dist2D(strk->pos1, strk->pos2);
        leftlen -= Dist2D(strk->pos1, strk->pos2) / spacing;
        currspars.Pars[csLen] = RngConv(leftlen, 0, maxlen, 0, 1);
        currspars.Pars[csLenpx] = RngConv(leftrlen, 0, maxrlen, 0, 1);
        emit SendTStroke2(*strk, currspars, lastspars);
    }

    lastspars = currspars;

//RepaintImgRect(r1|r2);
//this->update();

}

// ----------------- painting events ----------------

void ImageArray::tabletEvent(QTabletEvent *event) {
    d_Stroke strk;
//d_StrokePars stpars;
// generating input stroke pars
    {
        for (int i = 0; i < csSTOP; i++) currspars.Pars[i] = 1;

        //  currspars.Pars[csERASER] = 0;
        currspars.Pars[PenPressure] = event->pressure();
        float cxtilt = (event->xTilt() + pxtilt) / 2;
        float cytilt = (event->yTilt() + pytilt) / 2;
        currspars.Pars[csXtilt] = ((float) (cxtilt + 60)) / 120;
        currspars.Pars[csYtilt] = ((float) (cytilt + 60)) / 120;
        currspars.Pars[csTilt] = qMax(fabs(cxtilt), fabs(cytilt)) / 60.0;
        float penrot = AtanXY(cxtilt, cytilt);
        currspars.Pars[csRot] = RngConv(penrot, -M_PI, M_PI, 0, 1);
        QVector2D Hvt(cxtilt, cytilt);
        Hvt.normalize();
        currspars.Pars[csHVrot] = Hvt.x() / Hvt.length();
        currspars.Pars[csHtilt] = ((float) fabs(event->xTilt())) / 60;
        currspars.Pars[csVtilt] = ((float) fabs(event->yTilt())) / 60;
    }

    QPointF curpos;
    QPointF offpos = QPointF(event->hiResGlobalX(),
                             event->hiResGlobalY());                                                                 // T A B L E T //
    QPointF apos;
    QPoint localLrPos;
    curpos = mapFromGlobal(cursor().pos());
    if (corecount > 1) {
        localLrPos.setX(floor(offpos.x()));
        localLrPos.setY(floor(offpos.y()));
        qreal fx = offpos.x() - localLrPos.x(); //fractional part of tablet pos
        qreal fy = offpos.y() - localLrPos.y();
        localLrPos = mapFromGlobal(localLrPos);
        apos.setX(localLrPos.x() + fx);
        apos.setY(localLrPos.y() + fy);

        float chkrad = Dist2D(apos, curpos);   //radius check to avoid bug with tablet in mouse mode.
        if (chkrad > 15) {
            apos = curpos;
            apos.setX(apos.x() + fx);
            apos.setY(apos.y() + fy);
        }
    } else { apos = curpos; }





    //strk.pos1=CalcPos(apos);
//apos=CalcPos(apos);
    PPos4 = strk.pos1;

    if (event->pointerType() == QTabletEvent::Eraser)
        currspars.Pars[csERASER] = 1;
    else currspars.Pars[csERASER] = 0;

    if (TMode != 1) TMode = 1;
    if (locked == 0)
        switch (event->type()) {


//this->update();

            case QEvent::TabletPress:
/*
    if (Brepainted) {
        BPos=apos;
        Btilt=QPointF(currspars.Pars[csXtilt],currspars.Pars[csYtilt]);
        Brad=Brush->rad_out*ZoomK*RadCtl->GetModValue(lastspars)*ScaleCtl->GetModValue(lastspars);
        Brepainted=false;
    }
    */
                uptimer->setInterval(activetime);
                if ((MMode == mNone) && (TMode == 1)) {

                    leftlen = maxlen;
                    leftrlen = maxrlen;
                    TMode = 2;

                    if (GlobalKB->KBstate[ekCanvasMove] == true) {
                        //ScrollPos=event->pos();
                        MMode = mPan;
                    } else if (GlobalKB->KBstate[ekCanvasZoomIn] == true) {
                        PPos = event->pos();
                        MMode = mZoom;
                    } else if (GlobalKB->KBstate[ekFastBrush] == true) {
                        PPos = event->pos();
                        MMode = mRadSize;
                    } else if (GlobalKB->KBstate[ekCanvasPick] == true) {
                        MMode = mPick;
                    } else {
                        MMode = mPaint;
                        /*
                        ResetSpacer(strk.pos1);
                        strk.pos2=strk.pos1;
                        strk.pos3=strk.pos1;
                        pstroke=strk;
                        */
                        if (event->pressure() > 0) ppres = currspars.Pars[PenPressure];
                        pxtilt = currspars.Pars[csXtilt];
                        pytilt = currspars.Pars[csYtilt];
                        // ParseStartPosF((apos));
                    }
                    ParseStartPosF((apos));
                }

                break;

            case QEvent::TabletRelease:
                // MergeAll();
                // GenAllThumbs();
                uptimer->setInterval(idletime);
                TMode = 1;
                MMode = mNone;

                break;
            case QEvent::TabletMove:
                if ((MMode == mNone) && (TMode == 1) && event->pressure() > 0) {

                    leftlen = maxlen;
                    leftrlen = maxrlen;
                    TMode = 2;

                    if (GlobalKB->KBstate[ekCanvasMove] == true) {
                        //ScrollPos=event->pos();
                        MMode = mPan;
                    } else if (GlobalKB->KBstate[ekCanvasZoomIn] == true) {
                        PPos = event->pos();
                        MMode = mZoom;
                    } else if (GlobalKB->KBstate[ekFastBrush] == true) {
                        PPos = event->pos();
                        MMode = mRadSize;
                    } else if (GlobalKB->KBstate[ekCanvasPick] == true) {
                        MMode = mPick;
                    } else {
                        MMode = mPaint;
                        /*
                        ResetSpacer(strk.pos1);
                        strk.pos2=strk.pos1;
                        strk.pos3=strk.pos1;
                        pstroke=strk;
                        */
                        if (event->pressure() > 0) ppres = currspars.Pars[PenPressure];
                        pxtilt = currspars.Pars[csXtilt];
                        pytilt = currspars.Pars[csYtilt];
                        // ParseStartPosF((apos));
                    }
                    ParseStartPosF((apos));


                } else if (event->pressure() == 0) {
                    uptimer->setInterval(idletime);
                    TMode = 1;
                    MMode = mNone;
                    ParseMovePosF((apos));
                } else
                    ParseMovePosF((apos));
                break;
            default:
                break;


        }

    int rad = ceil(g_Brush->rad_out * RadCtl->GetModValue(lastspars) * ScaleCtl->GetModValue(lastspars));
    //if  (MMode==50) rad=Brush->rad_out*ZoomK;//if setting brush size
    if (rad < 1) rad = 1;

    // RepaintImgRect(QRect(PPos3.toPoint()-QPoint(rad,rad),PPos3.toPoint()+QPoint(rad,rad)));
    //RepaintImgRect(QRect(CalcPosI(event->pos())-QPoint(rad,rad),CalcPosI(event->pos())+QPoint(rad,rad)));
    PPos3 = CalcPosI(apos.toPoint());
    //emit SendNewB(BPos.toPoint(),(int)rad*ZoomK,(qreal)lastspars.Pars[csXtilt],(qreal)lastspars.Pars[csYtilt]);

    /*
    painter.drawEllipse(BPos,rad,rad);
    painter.drawLine(BPos,QPointF(BPos.x()+(lastspars.Pars[csXtilt]*2-1)*rad,BPos.y()+(lastspars.Pars[csYtilt]*2-1)*rad));
    painter.drawText(25,25,QString::number(dirtyRect.width())+" : "+QString::number(dirtyRect.height()));
    */

    //---- end of tabletevent
    if (corecount > 1) this->update();
}

void ImageArray::ParseStartPos(QPoint pos) {
    ParseStartPosF(QPointF(pos));
}

void ImageArray::ParseEndPos(QPoint pos) {
    ParseEndPosF(QPointF(pos));
}

void ImageArray::ParseMovePos(QPoint pos) {
    ParseMovePosF(QPointF(pos));

}

void ImageArray::EndPainting() {
    ParseEndPosF(LastMousePos);
}

void ImageArray::ParseStartPosF(QPointF pos) {
    LastMousePos = pos;
    if (locked == 0) {

        uptimer->setInterval(activetime);
        Drawn = false;

        leftlen = maxlen;
        leftrlen = maxrlen;

        lastspars = currspars;
        ScrollPos = pos;

        if (MMode == mZoom) { PPos = pos; }

/*        else if (GlobalKB->KBstate[ekFastBrush]==true) {PPos=pos;
            if (event->button()==Qt::LeftButton) MMode=mRadSize;
            if (event->button()==Qt::RightButton) MMode=mrad2Size;
            if (event->button()==Qt::MiddleButton) MMode=mCrv;

        }*/
        else if (MMode == mPick) {
            PPos4 = CalcPos(pos);
        } else if (MMode == mPaint) {

            if (EnablePoly)
                PolyTimer->start(PolyTreshold);

            emit AskResetPos();

            MPos2 = CalcPos(pos);
            MPos3 = MPos2;
            ResetSpacer(MPos2);
            d_Stroke strk;
            strk.pos1 = MPos2;
            strk.pos2 = MPos2;
            strk.pos3 = MPos2;
            pstroke = strk;
            PPos = strk.pos1;
            PPos3 = strk.pos1;

        }
    }
}

void ImageArray::ParseEndPosF(QPointF pos) {

    if (locked == 0) {
        //  MergeAll();
        //  GenAllThumbs();
        uptimer->setInterval(idletime); //must be idletime
        if ((MMode == mRadSize) | (MMode == mrad2Size) | (MMode == mCrv)) cursor().setPos(mapToGlobal(PPos.toPoint()));

        if ((!Drawn) && (MMode == mPaint)) {
            // single click paint
            //currspars.Pars[csERASER] = 0;
            for (int i = 0; i < csSTOP; i++) currspars.Pars[i] = 1;
            currspars.Pars[csHtilt] = 0.5;
            currspars.Pars[csVtilt] = 0.5;
            MPos2 = CalcPos(pos);
            MPos3 = MPos2;
            ResetSpacer(MPos2);
            d_Stroke strk;
            strk.pos1 = MPos2;
            strk.pos2 = MPos2;
            strk.pos3 = MPos2;
            pstroke = strk;
            PPos = strk.pos1;
            PPos3 = strk.pos1;
            emit SendTStroke(strk, currspars);
        }
        if (MMode != mPaint) {
            this->RepaintWidgetRect(
                    QRect(pos.x() - Loupesize - 5, pos.y() - Loupesize - 5, (qreal) Loupesize, (qreal) Loupesize));
            GenAllThumbs();
            MergeAll();
        }

        MMode = mNone;
        GM.NextVec();


    }

}

void ImageArray::ParseMovePosF(QPointF pos) {
    LastMousePos = pos;
    Drawn = true;
    if (GlobalKB->KBstate[ekCanvasMove] == true) this->setCursor(Qt::OpenHandCursor);
    else if (GlobalKB->KBstate[ekCanvasZoomIn] == true) this->setCursor(*curZoom);
    else this->setCursor(Qt::CrossCursor);
//currspars=InterpolateSpars(currspars,lastspars,0.5);

    if (Brepainted) {
        BPos = pos;
        Btilt = QPointF(currspars.Pars[csXtilt], currspars.Pars[csYtilt]);
        Brad = g_Brush->rad_out * ZoomK * RadCtl->GetModValue(lastspars) * ScaleCtl->GetModValue(lastspars);
        Brepainted = false;
    }

    if (MMode == mPan) {
        Offx -= (pos.x() - ScrollPos.x()) / ZoomK;
        Offy -= (pos.y() - ScrollPos.y()) / ZoomK;
        ScrollPos = pos;
        RepaintWidgetRect(this->rect());
    } else if (MMode == mPaint) {
        wasmerged = false;
        d_Stroke strk;
        strk.pos1 = CalcPos(
                pos);                                                                     //  MOVE    POS     PARSER  //
        strk.pos1 = Interpolate(pstroke,
                                strk);                                                        //                          //
        strk.pos2 = pstroke.pos1;
        strk.pos3 = pstroke.pos2;
        MPos2 = strk.pos1;
        MPos3 = strk.pos2;
        pstroke = strk;

        if (EnablePoly) {
            GM.AddPnt(strk.pos1);
            PolyTimer->stop();
            PolyTimer->start(PolyTreshold);
        }

        // -- THIS 2 LINES WERE MAKING FINGER AWESOME!!! DONT DELETE.
        //PPos=strk.pos1;
        //PPos3=strk.pos2;

        SpacingFilter(&strk);
    } else if (MMode == mPick) {
        QPointF pos1 = CalcPos(pos);
        PPos4 = pos1;
        RepaintWidgetRect(QRect(BPos2.x() - Loupesize - 5, BPos2.y() - Loupesize - 5, Loupesize, Loupesize));

    } else if (MMode == mZoom) {
        int delta = ((pos.x() - PPos.x()) + (pos.y() - PPos.y())) / 2;
        ZoomK = ZoomK * RngConv(delta, -5, 5, 0.9, 1.1);
        PPos = pos;
        RepaintWidgetRect(this->rect());
    } else if (MMode == mRadSize) {
        BPos = PPos;
        g_Brush->rad_out = Dist2D(PPos, pos) / ZoomK;
    } else if (MMode == mrad2Size) {
        BPos = PPos;
        g_Brush->rad_in = Dist2D(PPos, pos) / ZoomK;
        g_Brush->rad_in = qMin(g_Brush->rad_in, (g_Brush->rad_out - 1));
        BControls->CtlRadRel->Model->SetMaxCursor(g_Brush->rad_in / g_Brush->rad_out);
        BControls->BrushControl->saverel = (g_Brush->rad_in / g_Brush->rad_out);

    } else if (MMode == mCrv) {
        BPos = PPos;
        float rad_in = Dist2D(PPos, pos) / ZoomK;
        rad_in = qMin(rad_in, (g_Brush->rad_out));
        BControls->CtlCrv->Model->SetMaxCursor(1 - (rad_in / g_Brush->rad_out));

    } else {

        if ((pos.x() > this->geometry().width())
            | (pos.y() > this->geometry().height())
            | (pos.x() < 0)
            | (pos.y() < 0)
                ) emit MouseOut();
        else emit MouseIn();
    }



//    int rad=ceil(Brush->rad_out*ScaleCtl->GetValue()*RadCtl->GetMaxValue());
//    if (rad < 1) rad = 1;

    PPos3 = CalcPos(pos);
    lastspars = currspars;

}

void ImageArray::mousePressEvent(QMouseEvent *event) {
    if (locked == 0) {
        uptimer->setInterval(activetime);

        TMode = 0;

        Drawn = false;
        //parentWidget()->grabKeyboard();

        for (int i = 0; i < csSTOP; i++) currspars.Pars[i] = 1;
        currspars.Pars[csERASER] = 0;
        currspars.Pars[csHtilt] = 0.5;
        currspars.Pars[csVtilt] = 0.5;

        if (GlobalKB->KBstate[ekCanvasZoomIn] == true) { MMode = mZoom; }
        else if (GlobalKB->KBstate[ekFastBrush] == true) {
            PPos = event->pos();
            if (event->button() == Qt::LeftButton) MMode = mRadSize;
            if (event->button() == Qt::RightButton) MMode = mrad2Size;
            if (event->button() == Qt::MiddleButton) MMode = mCrv;
        } else if ((event->button() == Qt::MiddleButton) | (GlobalKB->KBstate[ekCanvasMove] == true)) { MMode = mPan; }
        else if ((event->button() == Qt::RightButton) | (GlobalKB->KBstate[ekCanvasPick] == true)) {
            MMode = mPick;
        } else if (event->button() == Qt::LeftButton) {
            MMode = mPaint;
        }

        ParseStartPosF(event->pos());
    }


}

void ImageArray::mouseMoveEvent(QMouseEvent *event) {
/*
    if ((TMode==0)&(IsLocked==0)) {


        Drawn=true;
        if (GlobalKB->KBstate[ekCanvasMove]==true) this->setCursor(Qt::OpenHandCursor);
        else if (GlobalKB->KBstate[ekCanvasZoomIn]==true) this->setCursor(*curZoom);
        else this->setCursor(Qt::CrossCursor);
        d_StrokePars stpars;
        stpars.Pars[csERASER] = 0;
        for (int i=0;i<csSTOP;i++) stpars.Pars[i]=1;
        stpars.Pars[csHtilt]=0.5;
        stpars.Pars[csVtilt]=0.5;
        //lastspars=stpars;

        if (Brepainted) {
            //Brad2=Brad;
            //BPos2=BPos;
            //Btilt2=Btilt;
            BPos=event->pos();
            Btilt=QPointF(stpars.Pars[csXtilt],stpars.Pars[csYtilt]);
            Brad=Brush->rad_out*ZoomK*RadCtl->GetModValue(lastspars)*ScaleCtl->GetModValue(lastspars);
            Brepainted=false;
        }

    if (MMode==mPan){
            Offx-=(event->pos().x()-ScrollPos.x())/ZoomK;
            Offy-=(event->pos().y()-ScrollPos.y())/ZoomK;
            ScrollPos=event->pos();
        RepaintWidgetRect(this->rect());

    }
    else if (MMode==mPaint) {
      wasmerged=false;

        d_Stroke strk;

        strk.pos1=CalcPos(event->pos());
        strk.pos1=Interpolate(pstroke,strk);
        strk.pos2=pstroke.pos1;
        strk.pos3=pstroke.pos2;
        MPos2=strk.pos1;
        MPos3=strk.pos2;
        pstroke=strk;

       // -- THIS 2 LINES WERE MAKING FINGER AWESOME!!! DONT DELETE.
       // PPos=strk.pos1;
       // PPos3=strk.pos2;

        SpacingFilter(&strk,stpars);
    }
    else if (MMode==mPick){
       QPoint pos1=CalcPosI(event->pos());
       PPos4=pos1;
       RepaintWidgetRect(QRect(BPos2.x()-Loupesize-5,BPos2.y()-Loupesize-5,Loupesize,Loupesize));



    }
    else if (MMode==mZoom){
        int delta=((event->pos().x()-PPos.x())+(event->pos().y()-PPos.y()))/2;
        ZoomK=ZoomK*RngConv(delta,-5,5,0.9,1.1);
        PPos=event->pos();
        RepaintWidgetRect(this->rect());
    }

    else if (MMode==mRadSize){
        BPos=PPos;
        Brush->rad_out=Dist2D(PPos,event->pos())/ZoomK;
    }
    else if (MMode==mrad2Size){
       BPos=PPos;
       Brush->rad_in=Dist2D(PPos,event->pos())/ZoomK;
       Brush->rad_in=qMin(Brush->rad_in,(Brush->rad_out-1));
       BControls->CtlRadRel->SetMaxValF(Brush->rad_in/Brush->rad_out);
       BControls->BrushControl->saverel=(Brush->rad_in/Brush->rad_out);

   }
    else if (MMode==mCrv){
       BPos=PPos;
       float rad_in=Dist2D(PPos,event->pos())/ZoomK;
       rad_in=qMin(rad_in,(Brush->rad_out));
       BControls->CtlCrv->SetMaxCursor(1-(rad_in/Brush->rad_out));

   }
    else {

          if ((event->pos().x()>this->geometry().width())
                  | (event->pos().y()>this->geometry().height())
                  | (event->pos().x() < 0)
                  | (event->pos().y() < 0)
                  ) emit MouseOut();
          else emit MouseIn();
        }



//    int rad=ceil(Brush->rad_out*ScaleCtl->GetValue()*RadCtl->GetMaxValue());
//    if (rad < 1) rad = 1;

    PPos3=CalcPosI(event->pos());


    }
//this->update();
    //if (Brepainted==false) { BPos2=BPos;
    //}

    //this->update();
    */
    if ((TMode == 0) & (locked == 0)) {
        ParseMovePos(event->pos());
    }
    if (corecount > 1) this->update();
}

void ImageArray::mouseDoubleClickEvent(QMouseEvent *event) {
    MMode = mNone;

}

void ImageArray::mouseReleaseEvent(QMouseEvent *event) {
/*if (IsLocked==0) {
    MergeAll();
    GenAllThumbs();
    uptimer->setInterval(idletime);
    if ((MMode==mRadSize)|(MMode==mrad2Size)|(MMode==mCrv)) cursor().setPos(mapToGlobal(PPos.toPoint()));

if ((event->button()==Qt::LeftButton)&&(!Drawn)&&(MMode==mPaint)){
    d_StrokePars stpars;
    stpars.Pars[csERASER] = 0;
    for (int i=0;i<csSTOP;i++) stpars.Pars[i]=1;
    stpars.Pars[csHtilt]=0.5;
    stpars.Pars[csVtilt]=0.5;
    MPos2=CalcPos(event->pos());
    MPos3=MPos2;
    ResetSpacer(MPos2);
    d_Stroke strk;
    strk.pos1=MPos2;
    strk.pos2=MPos2;
    strk.pos3=MPos2;
    pstroke=strk;
    PPos=strk.pos1;
    PPos3=strk.pos1;
    emit SendTStroke(strk,stpars);
}
        MMode=mNone;
 // RepaintWidgetRect(this->rect());
}
*/
    ParseEndPosF(QPointF(event->pos()));
}

void ImageArray::moveEvent(QMoveEvent *event) {
    float rad = g_Brush->rad_out * ZoomK * RadCtl->GetModValue(lastspars) * ScaleCtl->GetModValue(lastspars);
    if (rad < 1) rad = g_Brush->rad_out * ZoomK;
    //  RepaintImgRect(QRect(event->pos()-QPoint(rad,rad),MPos2.toPoint()+QPoint(rad,rad)));
//emit SendNewB(BPos.toPoint(),(int)rad,(qreal)lastspars.Pars[csXtilt],(qreal)lastspars.Pars[csYtilt]);
//this->update();

}

void ImageArray::SaveImg() {
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image"),
                                                    "",
                                                    tr("eyeris layer image ELI PNG Files (*.ELI);;Portable Network Graphic (*.PNG);;JPG Files (*.JPG)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty()) {
        SaveImg(fileName);
    }

}

void ImageArray::ExportImg(QString fname) {

    if (!fname.isEmpty()) {
        QImage saveimg;
        resizeImage(&saveimg, ViewCanvas[0].size());
        QPainter pnt(&saveimg);
        pnt.setCompositionMode(QPainter::CompositionMode_Source);
        pnt.setBrush(QColor::fromRgbF(0, 0, 0, 0));
        pnt.setPen(Qt::NoPen);
        pnt.drawRect(QRect(0, 0, ViewCanvas[0].width(), ViewCanvas[0].height()));
        pnt.setCompositionMode(QPainter::CompositionMode_SourceOver);
        //  if (!wasmerged) {
        for (int i = ViewCanvas.count() - 1; i >= 0; i--) {

            if (LayerProps[i].IsVisible) {
                pnt.setOpacity(LayerProps[i].Opacity);
                pnt.setCompositionMode(BMsel.GetCMfromIndex(LayerProps[i].blendmode));
                pnt.drawImage(0, 0, ViewCanvas[i]);
            }

        }
        // }
        saveimg.save(fname);
    }

}

void ImageArray::SaveImg(QString fname) {
    /*
//original save procedure, will use it for export later;
    QFileDialog::Options options;
//    if (!native->isChecked())
  //      options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                tr("QFileDialog::getOpenFileName()"),
                                                    "",
                                tr("PNG Files (*.png)"),
                                &selectedFilter,
                                options);
    if (!fileName.isEmpty()){
        ViewCanvas[0].save(fileName);
        }
    */


    if (!(fname.right(3).contains("ELI"))) {
        ExportImg(fname);
        return;
    }

    QString cname = fname;
    cname.chop(4);
    emit AskSaveLog(cname);

    QFile savefile(fname);
    savefile.open(QIODevice::ReadWrite);
    SaveImg(&savefile);
    savefile.close();
}

void ImageArray::SaveImg(QIODevice *iodev) {
    QDataStream dstream;
    dstream.setDevice(iodev);
    qint32 lcount = ViewCanvas.count();
    dstream << lcount;
    qint32 width = ViewCanvas[0].width();
    qint32 height = ViewCanvas[0].height();
    dstream << width;
    dstream << height;
    for (int i = 0; i < ViewCanvas.count(); i++) {

        QByteArray ba;
        QBuffer buf(&ba);
        buf.open(QIODevice::WriteOnly | QIODevice::Truncate);
        ViewCanvas[i].save(&buf, "PNG");
        qint32 lsz = buf.size();
        dstream << lsz;
        dstream << buf.data();
        LayerProps[i].Serialize(&dstream);
        //    QByteArray LPBuf((const char *)&LayerProps[i], sizeof(LayerData));
        //   dstream << LPBuf;

    }
}

void ImageArray::WriteToBytes(QByteArray *ba) {
    QBuffer *buf = new QBuffer(ba);
    buf->open(QBuffer::ReadWrite);
    SaveImg(buf);
    delete buf;
}

void ImageArray::GetBSize(int sz) {
    BSize = sz;
}

QPointF ImageArray::CalcPos(QPointF fpos) {
    int hw = this->geometry().width() * 0.5;
    int hh = this->geometry().height() * 0.5;
    fpos.setX(Offx + ((fpos.x() - hw) / ZoomK));
    fpos.setY(Offy + ((fpos.y() - hh) / ZoomK));
    return fpos;
}

QPoint ImageArray::CalcPosI(QPoint fpos) {
    int hw = ceil(this->geometry().width() * 0.5);
    int hh = ceil(this->geometry().height() * 0.5);
    fpos.setX(Offx + floor((fpos.x() - hw) / ZoomK));
    fpos.setY(Offy + floor((fpos.y() - hh) / ZoomK));
    return fpos;
}

QPointF ImageArray::Interpolate(d_Stroke prev, d_Stroke curr) {
    QPointF pnt;

    float dx1 = prev.pos1.x() - prev.pos2.x();
    float dx2 = curr.pos1.x() - prev.pos1.x();
    float dy1 = prev.pos1.y() - prev.pos2.y();
    float dy2 = curr.pos1.y() - prev.pos1.y();

    pnt.setX(prev.pos1.x() + (dx1 + dx2) / 2);
    pnt.setY(prev.pos1.y() + (dy1 + dy2) / 2);

    return pnt;
}

void ImageArray::resizeEvent(QResizeEvent *event) {
    RepaintWidgetRect(this->rect());
    this->update();
}

//----------------- L A Y E R   O P E R A T I O N S ----------------


void ImageArray::AddLayer() {
    LayerData lp;

    lp.Opacity = 1;
    lp.IsVisible = true;
    lp.blendmode = 0;
    LayerProps.append(lp);

    QImage NewImg(ViewCanvas[0].size(), QImage::Format_ARGB32_Premultiplied);
    QImage DropImg(ViewCanvas[0].size(), QImage::Format_ARGB32_Premultiplied);
    ViewCanvas.append(NewImg);
    DropLayers.append(DropImg);

    resizeImage(&ViewCanvas[ViewCanvas.count() - 1], ViewCanvas[0].size());
    resizeImage(&DropLayers[DropLayers.count() - 1], ViewCanvas[0].size());
    emit SendNewLayer(NewImg);
}

void ImageArray::AddLayerAt(int above) {
    LayerData lp;

    lp.Opacity = 1;
    lp.IsVisible = true;
    lp.blendmode = 0;
    LayerProps.insert(above, lp);
    qDebug() << ("lp added");
    //  QImage NewImg;
    // QImage DropImg;
    ViewCanvas.insert(above, QImage(ViewCanvas[0].size(), QImage::Format_ARGB32_Premultiplied));
    DropLayers.insert(above, QImage(ViewCanvas[0].size(), QImage::Format_ARGB32_Premultiplied));

    resizeImage(&ViewCanvas[above], MasterSize);
    resizeImage(&DropLayers[above], MasterSize);
    qDebug() << ("layer added");
    //emit SendNewLayer(NewImg);
}

void ImageArray::AddZeroLayer(QSize sz, QColor bgcol) {

// clear all layers;

    LayerData lp;

    lp.Opacity = 1;
    lp.blendmode = 0;
    lp.IsVisible = true;
    LayerProps.append(lp);

    QImage NewImg;
    QImage DropImg;
    ViewCanvas.append(NewImg);
    DropLayers.append(DropImg);

    qDebug() << ("IaR zero layer added");

    resizeImage(&ViewCanvas[ViewCanvas.count() - 1], sz);
    resizeImage(&DropLayers[DropLayers.count() - 1], sz);
    resizeImage(&MergedCanvas, sz);
    qDebug() << ("IaR zero layer reszd");
    QPainter painter(&ViewCanvas[0]);
    painter.setBrush(QColor::fromRgbF(0.0, 0.0, 0.0, 0.0));
    painter.setPen(Qt::NoPen);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawRect(0, 0, sz.width(), sz.height());
    qDebug() << ("IaR zero layer filled");

//LayerAction lact;
//lact.ActID=laAdd;
//lact.layer=0;
//emit SendLAction(lact);


};

bool ImageArray::DelLayer(int num) {
    qDebug() << ("LYR delete at" + QString::number(num));
    if (ViewCanvas.count() > 1) {
        QImage *Dump = &ViewCanvas[num];
        LayerDump.append(Dump);

        ViewCanvas.removeAt(num);
        DropLayers.removeAt(num);
        LayerProps.removeAt(num);
        GenAllThumbs();
        return true;
    }
    return false;
}

void ImageArray::MoveLayer(int from, int above) {
    qDebug() << ("LYR move from" + QString::number(from) + "above" + QString::number(above));
    ViewCanvas.move(from, above);
    DropLayers.move(from, above);
    LayerProps.move(from, above);
    RepaintWidgetRect(this->rect());
    GenAllThumbs();
}

void ImageArray::DropLayer(int num) {
    qDebug() << ("LYR drop at" + QString::number(num));
    if (num < ViewCanvas.count() - 1) {
        QPainter painter(&ViewCanvas[num + 1]);
        painter.setPen(Qt::NoPen);
        painter.setOpacity(LayerProps[num].Opacity / LayerProps[num + 1].Opacity);
        painter.setCompositionMode(BMsel.GetCMfromIndex(LayerProps[num].blendmode));
        painter.drawImage(0, 0, ViewCanvas[num]);
        DelLayer(num);
    }

}

void ImageArray::GenDrops(int num) {

}

void ImageArray::GenThumb(int lnum) {
    QImage *thumb = new QImage;
    float factor = 120.0 / ViewCanvas[0].width();
    resizeImage(thumb, ViewCanvas[0].size() * factor);

    QPainter painter(thumb);

    painter.setPen(Qt::NoPen);

    QColor whites = QColor::fromRgbF(0.95, 0.95, 0.95);
    QColor blacks = QColor::fromRgbF(0.6, 0.6, 0.6);
    int rsize = 8;
    painter.setPen(Qt::NoPen);
    painter.setBrush(whites);
    painter.drawRect(0, 0, rsize * 2, rsize * 2);
    painter.setBrush(blacks);
    painter.drawRect(0, 0, rsize, rsize);
    painter.drawRect(rsize, rsize, rsize, rsize);
    for (int i = 1; i < thumb->width() / (rsize * 2); i++)
        painter.drawImage(QPoint(rsize * i * 2, 0), *thumb);
    for (int i = 1; i < thumb->height() / (rsize * 2); i++)
        painter.drawImage(QPoint(0, rsize * i * 2), *thumb);
    painter.drawImage(thumb->rect(), ViewCanvas[lnum], ViewCanvas[lnum].rect());
    emit SendThumb(lnum, *thumb);
    //delete thumb;
}

void ImageArray::GenAllThumbs() {
    RepaintWidgetRect(this->rect());
    for (int i = 0; i < (ViewCanvas.count()); i++)
        GenThumb(i);
    this->update();

    return;
}

void ImageArray::SetActiveLayer(int num) {
    ActiveLayer = num;
}

void ImageArray::SetLOp(int layer, float op) {
    LayerProps[layer].Opacity = op;
    //please add collapsed repaint here;
    RepaintWidgetRect(this->rect());
    wasmerged = false;
}

void ImageArray::SetLBm(int layer, int bm) {
    LayerProps[layer].blendmode = bm;
    //please add collapsed repaint here;
    RepaintWidgetRect(this->rect());
    wasmerged = false;
}

void ImageArray::SetLvis(int layer, bool vis) {
    LayerProps[layer].IsVisible = vis;
    RepaintWidgetRect(this->rect());
}

void ImageArray::ExecLayerAction(LayerAction lact) {
/*
    if (lact.ActID==laBm){
        SetLBm(lact.layer,lact.bm);
    }
    else if (lact.ActID==laOp){
        SetLOp(lact.layer,lact.Opacity);
    }
    else if (lact.ActID==laDup){
        DupLayer(lact.layer);
    }
    else if (lact.ActID==laDel){
        DelLayer(lact.layer);
    }
    else if (lact.ActID==laAdd){
        AddLayerAt(lact.layer);
    }
    else if (lact.ActID==laDrop){
        DropLayer(lact.layer);
    }
    else if (lact.ActID==laMove){
        MoveLayer(lact.layer,lact.layerto);
    }
    else if (lact.ActID==laNewCanvas){
        this->NewImg(lact.rect.size(),lact.layer);
    }
    else if (lact.ActID==laResizeCanvas){
        this->ResizeScene(lact.rect.size(),1);
    }
    else if (lact.ActID==laCropCanvas){
        this->ResizeScene(lact.rect.size(),0);
    }
*/
}

void ImageArray::DupLayer(int layer) {
    LayerData lp;

    lp.Opacity = 1;
    lp.IsVisible = true;
    lp.blendmode = 0;
    //LayerProps.append(lp);
    LayerProps.insert(layer, LayerProps.at(layer));
    ViewCanvas.insert(layer, ViewCanvas.at(layer));
    DropLayers.insert(layer, DropLayers.at(layer));

    //resizeImage(&ViewCanvas[ViewCanvas.count()-1], ViewCanvas[0].size());
    //resizeImage(&DropLayers[DropLayers.count()-1], ViewCanvas[0].size());
    //emit SendNewLayer(NewImg);
    GenAllThumbs();
}

/*
void ImageArray::DupActiveLayer(){
    //LayerData lp;


    LayerProps.insert(ActiveLayer+1,LayerProps.at(ActiveLayer));

    ViewCanvas.insert(ActiveLayer+1,ViewCanvas.at(ActiveLayer));
    DropLayers.insert(ActiveLayer+1,DropLayers.at(ActiveLayer));

    //resizeImage(&ViewCanvas[ViewCanvas.count()-1], ViewCanvas[0].size());
    //resizeImage(&DropLayers[DropLayers.count()-1], ViewCanvas[0].size());
    //emit SendNewLayer(NewImg);
emit SendLayer(ActiveLayer,ViewCanvas.at(ActiveLayer));
}
*/

void ImageArray::ResizeScene(QSize size, int stretchp) {

    for (int k = 0; k < ViewCanvas.count(); k++) {
        rescaleImage(&ViewCanvas[k], size);
        rescaleImage(&DropLayers[k], size);
    }
    RepaintWidgetRect(this->rect());
    this->update();


};

void ImageArray::MergeAll() {
/*

    if (!wasmerged) {


    QPainter painter(&MergedCanvas);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(MergedCanvas.rect());

    for (int i=0;i<ViewCanvas.count();i++){
        if (LayerProps[i].visible) {
            painter.setOpacity(LayerProps[i].Opacity);
            painter.setCompositionMode(BMsel.GetCMfromIndex(LayerProps[i].blendmode));
            painter.drawImage(0,0,ViewCanvas[i]);

        }
    }

    wasmerged=true;
    }
*/
}




// --------------- Zooming operations -----------------

void ImageArray::FitScreen() {

    if (ViewCanvas[0].width() > ViewCanvas[0].height())
        ZoomK = (float) this->rect().width() / (float) ViewCanvas[0].width();
    else
        ZoomK = (float) this->rect().width() / (float) ViewCanvas[0].height();
//int a=0;
    Offx = ViewCanvas[0].width() * 0.5;
    Offy = ViewCanvas[0].height() * 0.5;
    RepaintWidgetRect(this->rect());

}

void ImageArray::TouchScreen() {
    if (ViewCanvas[0].width() < ViewCanvas[0].height())
        ZoomK = (float) this->rect().width() / (float) ViewCanvas[0].width();
    else
        ZoomK = (float) this->rect().width() / (float) ViewCanvas[0].height();
//int a=0;
    Offx = ViewCanvas[0].width() * 0.5;
    Offy = ViewCanvas[0].height() * 0.5;
    RepaintWidgetRect(this->rect());

}

void ImageArray::ActualPixels() {
    ZoomK = 1;
//Offx=;
//Offy=0;
    RepaintWidgetRect(this->rect());
}

void ImageArray::SetLock(qint8 lk) {
    locked = locked + lk;
    if (locked > 0) {
        //this->setEnabled(false);
    } else {
        this->setEnabled(true);
        locked = 0;
    }
}

d_StrokePars ImageArray::InterpolateSpars(d_StrokePars current, d_StrokePars last, qreal kblend) {
    d_StrokePars newspars;
    //float dirblend=0.4;
    for (int i = 0; i < csSTOP; i++) {
        //if (i!=csERASER)
        if (i == Direction)
            newspars.Pars[i] = current.Pars[i];//(current.Pars[i]*dirblend)+(last.Pars[i]*(1.0-dirblend));
        else
            newspars.Pars[i] = current.Pars[i] * kblend + last.Pars[i] * (1.0 - kblend);
    }
    for (int i = csSTOP; i < csENDPOINT; i++) {
        newspars.Pars[i] = current.Pars[i];
    }
    return newspars;
}

void ImageArray::enterEvent(QEvent *event) {
    this->activateWindow();

}

QImage ImageArray::GetCollapsedRect(QRect wRect) {
    int hw = floor(this->geometry().width() * 0.5);
    int hh = floor(this->geometry().height() * 0.5);

    QRect iRect = QRect(floor(wRect.x() / ZoomK + Offx - hw / ZoomK),
                        floor(wRect.y() / ZoomK + Offy - hh / ZoomK),
                        ceil(wRect.width() / ZoomK),
                        ceil(wRect.height() / ZoomK)
    );

    iRect.setX(floor(iRect.x()));
    iRect.setY(floor(iRect.y()));
    iRect.setWidth(ceil(iRect.width()));
    iRect.setHeight(ceil(iRect.height()));

    //QRect dirtyRect(
    QRect w2Rect(
            (floor((iRect.x() - Offx) * ZoomK) + hw),
            (floor((iRect.y() - Offy) * ZoomK) + hh),
            ceil(iRect.width() * ZoomK),
            ceil(iRect.height() * ZoomK)
    );

    QImage resimg(wRect.size(), QImage::Format_ARGB32);
    QPainter painter(&resimg);

    painter.drawImage(resimg.rect(), *ImgTransp, w2Rect);

// for (int i=RepaintLayer;i<ViewCanvas.count();i++)                                    // repaint layers to the top;


// if (!wasmerged) {
    for (int i = ViewCanvas.count() - 1; i >= 0; i--) {

        if (LayerProps[i].IsVisible) {
            painter.setOpacity(LayerProps[i].Opacity);
            painter.setCompositionMode(BMsel.GetCMfromIndex(LayerProps[i].blendmode));
            painter.drawImage(resimg.rect(), ViewCanvas[i], iRect);
        }

    }
    return resimg;
}

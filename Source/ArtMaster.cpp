/*
    RePaint, the network painting tool
    Copyright (C) 2013-2023 Yauheni Bolshakov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
  */

#include "ArtMaster.h"
#include "Brushes.h"
#include <QRect>
#include <QLinearGradient>
#include <QPainter>
#include <QImage>
#include <QRgb>
#include "layerstack.h"
#include "BrushEngine/LegacySharedBrush.hpp"

/*ArtMaster::ArtMaster(QObject *parent) :
    QObject(parent)*/
ArtMaster::ArtMaster(QObject *parent)
{

    aamn = 0; // action amount
    adist = 0;

    //
    QString AppPath = RESPATH;

    //-------------------------- LOADING Small Brushes
    SBMax = 4;
    /*
    for (int i=0;i<=SBMax;i++){
        SmallBrushes.append(QImage(AppPath+"/Brushes/"+QString::number(i)+".png"));
    }
    */

    NoiseProfiles.append(QImage(RESPATH + "/Noise/clouds.png"));

    //      Masks.append(QImage(RESPATH+"/Brushes/cloud.png"));
    //      Masks.append(QImage(RESPATH+"/Brushes/flow.png"));

    //-------------------------- Generating displace differmasks
    //   QImage *qi;

    DispBrushes.append(QImage(RESPATH + "/DBrushes/Waves.png"));

    //        qi = &DispBrushes.last();//new QImage(RESPATH+"/DBrushes/Waves.png");

    QRgb *dpx;
    int wd = DispBrushes.last().width();
    int i = 1;
    quint8 dx;
    quint8 dy;

    int ucdx;
    int ucdy;

    for (int i = 0; i < wd; i++)
    {
        int rad;
        dpx = (QRgb *)DispBrushes.last().constScanLine(i);
        for (int j = 0; j < wd; j++)
        {
            rad = Dist2D(QPoint(127, 127), QPoint(j, i));

            ucdx = (qRed(dpx[j]) - j);
            ucdy = (qGreen(dpx[j]) - i);
            ucdx = qMin(rad, ucdx);
            ucdy = qMin(rad, ucdy);
            dx = ucdx + 127;
            dy = ucdy + 127;

            dpx[j] = qRgb(dx, dy, 127);
        }
    }

    qDebug() << ("ARTM:disp brushes loaded");

    //-------------------------------

    // resizeImage(&crPinch,QSize(256,2));
    // resizeImage(&crBub,QSize(256,2));
    // resizeImage(&crCont,QSize(256,256));
    // resizeImage(&crDecont,QSize(256,2));
    // resizeImage(&crClamp,QSize(256*3,256));
    crPinch = QImage(256, 1, QImage::Format_ARGB32); // [0-255] cnt=256
    crBub = QImage(256, 1, QImage::Format_ARGB32);
    crCont = QImage(256, 256, QImage::Format_ARGB32);
    //  crDecont=QImage(257,2,QImage::Format_ARGB32);
    crClamp = QImage(256 * 3, 256, QImage::Format_ARGB32);

    //    br512 = QImage(QSize(511,511),QImage::Format_ARGB32);
    //   br512e = QImage(QSize(512,512),QImage::Format_ARGB32);
    qDebug() << ("ARTM:FadeCurveExp init");

    // float rad;
    // int iv;

    //  if (!QFile(RESPATH+"/res/br512.png").exists())
    /*
    {
        for (int i=0;i<=511;i++){
            px=(QRgb*)br512.scanLine(i);
            for (int j=0;j<=511;j++){
                 rad=Dist2D(QPointF(i,j),QPointF(255,255));
                 if (rad>255) rad=255;
                 iv=(255-round(rad));
                 px[j]=qRgba(0,0,0,iv);
            }
        }
        br512.save(RESPATH+"/res/br512.png");
    }
    */

    //   if (!QFile(RESPATH+"/res/br512e.png").exists())
    /*
      {
          for (int i=0;i<=512;i++){
              px=(QRgb*)br512e.scanLine(i);
              for (int j=0;j<=512;j++){
                   rad=Dist2D(QPointF(i,j),QPointF(256.5,256.5));
                   if (rad>255) rad=255;
                   iv=(255-round(rad));
                   px[j]=qRgba(0,0,0,iv);
              }
          }
          br512.save(RESPATH+"/res/br512e.png");
      }
  */

    wd = crPinch.width();
    i = 1;
    {
        float sal;
        QRgb *px;
        px = (QRgb *)crPinch.constScanLine(0);
        for (int j = 0; j <= 255; j++)
        {

            sal = (float)j / 255.0;
            sal = sal * sal * sal;
            sal = sal * 255;
            /*
            ((char *)&px[j])[0]=sal;
            ((char *)&px[j])[1]=sal;
            ((char *)&px[j])[2]=sal;
            ((char *)&px[j])[3]=sal;*/
            px[j] = qRgba(sal, sal, sal, sal);
        }
        //((char *)&px[256])[0]=1;
        // px[j]=qRgba(sal,sal,sal,sal);
        qDebug() << ("ARTM:pinch done");
    }
    {
        float sal;
        QRgb *px;
        px = (QRgb *)crBub.constScanLine(0);
        // bubble curve using x^3
        for (int j = 0; j <= 255; j++)
        {
            sal = (float)j / 255.0;
            sal = 1 + (sal - 1) * (sal - 1) * (sal - 1);
            sal = sal * 255;
            /*
            ((char *)&px[j])[0]=sal;
            ((char *)&px[j])[1]=sal;
            ((char *)&px[j])[2]=sal;
            ((char *)&px[j])[3]=sal;*/
            px[j] = qRgba(sal, sal, sal, sal);
        }

        qDebug() << ("ARTM:bubble done");
    }

    // contrast curve
    {
        float sal;
        QRgb *px;
        for (int m = 0; m < 256; m++)
        {
            px = (QRgb *)crCont.constScanLine(m);
            for (int j = 0; j <= m; j++)
            { // m=127
                sal = (float)j / (float)m;
                sal = sal * sal * sal;
                sal = sal * m;
                /*
                ((char *)&px[j])[0]=sal;
                ((char *)&px[j])[1]=sal;
                ((char *)&px[j])[2]=sal;
                ((char *)&px[j])[3]=sal;*/
                px[j] = qRgba(sal, sal, sal, sal);
            }
            for (int j = m + 1; j < 256; j++)
            {
                sal = (float)(j - (m + 1)) / (float)(255 - m);
                sal = 1 + (sal - 1) * (sal - 1) * (sal - 1);
                sal = sal * (255 - m) + m + 1;
                /*
                ((char *)&px[j])[0]=sal;
                ((char *)&px[j])[1]=sal;
                ((char *)&px[j])[2]=sal;
                ((char *)&px[j])[3]=sal;*/
                px[j] = qRgba(sal, sal, sal, sal);
            }
        }
    }
    qDebug() << ("ARTM:contrast done");

    // px=(QRgb*)crClamp.scanLine(1);

    QPainter pClamp(&crClamp);
    qDebug() << ("ARTM:clamp pnt init");
    QLinearGradient grad;
    grad = QLinearGradient(257, 0, 512, 0);

    quint16 ep = 512;
    qDebug() << ("ARTM:clamp grad init");

    grad.setColorAt(0, QColor::fromRgbF(0.0, 0.0, 0.0, 1.0));
    grad.setColorAt(1, QColor::fromRgbF(1.0, 1.0, 1.0, 1.0));
    pClamp.setPen(Qt::NoPen);

    for (int i = 0; i < 255; i++)
    {

        grad.setFinalStop(ep, 0);
        pClamp.setBrush(grad);
        pClamp.drawRect(0, i, crClamp.width(), i + 1); //.fillRect(0,(const int)i,256*3-1,(const int)i);
        ep -= 1;
    }
    pClamp.setBrush(QColor::fromRgbF(0.0, 0.0, 0.0, 1.0));
    pClamp.drawRect(0, 256, 256, 256);
    pClamp.setBrush(QColor::fromRgbF(1.0, 1.0, 1.0, 1.0));
    pClamp.drawRect(257, 255, crClamp.width() - 256, 256);
    qDebug() << ("ARTM:clamp done");
    /*
    crPinch.save("D:\\pinch.png");
    crBub.save("D:\\crBub.png");
    crCont.save("D:\\crCont.png");

    crClamp.save("D:\\crClamp.png");
       */
    // ----------------- END OF ARTMASTER INITIALIZATION ----------------
}

void ArtMaster::GenSolidity(QImage *img, float sol, float sol2op)
{
    int sal;
    QRgb *px;
    // Solidity=Solidity/255.0;
    // SolidityOfOpacity=SolidityOfOpacity/255.0;
    int wd = img->width();
    for (int i = 0; i < wd; i++)
    {
        px = (QRgb *)img->constScanLine(i);
        for (int j = 0; j < wd; j++)
        {
            sal = qAlpha(px[j]) * (((qrand() / 32768.0) < sol) ? 1 : 0); // solidity filter
            sal = ((abs(qrand() / 128) < sal) ? 255 * sol2op + sal * (1 - sol2op) : sal * (1 - sol2op));
            // qAlpha(px[j])
            px[j] = qRgba(qRed(px[j]), qGreen(px[j]), qBlue(px[j]), sal);
            //((char *)&px[j])[3]=sal;
        }
    }
}

void ArtMaster::GenSolidityP(QImage *img, float sol, float sol2op, qint16 noisex, qint16 noisey)
{
    int sal;

    QRgb *px;
    QRgb *npx;

    qint16 offx = noisex;
    qint16 offy = noisey;
    // float fsol=Solidity;
    float fsol2op = sol2op;
    sol = sol * 255;
    sol2op = sol2op * 255;
    int wd = img->width();
    for (int i = 0; i < wd; i++)
    {
        px = (QRgb *)img->scanLine(i);
        npx = (QRgb *)NoiseProfiles.at(0).scanLine(i + offy);
        for (int j = 0; j < wd; j++)
        {
            sal = qAlpha(px[j]) * (((qRed(npx[j + offx])) < sol) ? 1 : 0); // solidity filter
            sal = ((qRed(npx[j + offx]) < sal) ? 255 * fsol2op + sal * (1 - fsol2op) : sal * (1 - fsol2op));
            px[j] = qRgba(qRed(px[j]), qGreen(px[j]), qBlue(px[j]), sal);
            // ((char *)&px[j])[3]=sal;
        }
    }
}

void ArtMaster::GenFocal(QImage *img, float fop)
{
    int sal;
    QRgb *px;
    QRgb *cpx;

    int wd = img->width();
    int bpn;
    cpx = (QRgb *)crPinch.constScanLine(0);
    for (int i = 0; i < wd; i++)
    {
        px = (QRgb *)img->constScanLine(i);

        for (int j = 0; j < wd; j++)
        {
            sal = qAlpha(px[j]);
            bpn = qRed(cpx[sal]);
            sal = ((bpn - sal) * fop) + sal;
            //((char *)&px[j])[3]=sal;
            px[j] = qRgba(qRed(px[j]), qGreen(px[j]), qBlue(px[j]), sal);
        }
    }
}
void ArtMaster::GenFocalInv(QImage *img, float fop)
{
    int sal;
    QRgb *px;
    QRgb *cpx;
    int bpn;
    int wd = img->width();
    cpx = (QRgb *)crBub.constScanLine(0);
    for (int i = 0; i < wd; i++)
    {
        px = (QRgb *)img->constScanLine(i);

        for (int j = 0; j < wd; j++)
        {
            sal = qAlpha(px[j]);
            bpn = qRed(cpx[sal]);
            sal = ((bpn - sal) * fop) + sal;
            //((char *)&px[j])[3]=sal;
            px[j] = qRgba(qRed(px[j]), qGreen(px[j]), qBlue(px[j]), sal);
        }
    }
}
float ArtMaster::GenCurveF(float val, float midp)
{
    if (midp < 0)
    {
        float mid = 1 - midp;
        float fpos = 1 + pow(val - 1, 3);
        return (fpos - val) * mid + val;
    }
    else if (midp > 0)
        return (val * val * val - val) * midp + val;
    else
        return val;
}
void ArtMaster::GenClamp(QImage *img, float top, float min)
{
    int sal;
    // signed int tsal;
    if (min >= top)
        min = top - 0.1;
    // float mul=1/(top-min);
    // if (min>=top) forceclamp=1;
    int imin = 255;
    QRgb *px;
    QRgb *cpx;
    cpx = (QRgb *)crClamp.constScanLine(round((1 - top) * 255));
    int wd = img->width();
    for (int i = 0; i < wd; i++)
    {
        px = (QRgb *)img->constScanLine(i);
        for (int j = 0; j < wd; j++)
        {
            sal = qAlpha(px[j]);
            // tsal=(sal-min*255)*mul; // offsetted funcion is multiplied - scaled down vertically;
            // tsal=qMax(tsal,0);
            // sal=qMin(tsal,255);
            sal = qRed(cpx[imin + sal]);
            //((char *)&px[j])[3]=sal;
            px[j] = qRgba(qRed(px[j]), qGreen(px[j]), qBlue(px[j]), sal);
        }
    }
}
// add doactionlist

void ArtMaster::DoActionList(QImage *img, dotList *actlist, bool local = true)
{
    ActionData *Act;
    for (int k = 0; k < actlist->count(); k++)
    {
        Act = &actlist->operator[](k);

        if ((Act->ToolID == eSmudge) | (Act->ToolID == eDisp) | (Act->ToolID == eCont))
        {
            // //clamp position please move outside
            int twd = Act->Brush.ClientStamp.rad_out * Act->Brush.ClientStamp.scale;
            float cx = qMin(Act->Stroke.pos2.x(), (qreal)img->width() - twd);
            cx = qMax(cx, (float)twd);
            float cy = qMin(Act->Stroke.pos2.y(), (qreal)img->height() - twd);
            cy = qMax(cy, (float)twd);
            Act->Stroke.pos2.setX(cx);
            Act->Stroke.pos2.setY(cy);
        }
        if (Act->ToolID == eBrush)
            this->DrawBrush(img, Act->Stroke, Act->Brush);
        else if (Act->ToolID == eSmudge)
            DrawSmudge(img, Act->Stroke, Act->Brush);
        else if (Act->ToolID == eDisp)
            DrawDisp(img, Act->Stroke, Act->Brush);
        else if (Act->ToolID == eCont)
            LvlBrush(img, Act->Stroke, Act->Brush);
        else if (Act->ToolID == eLine)
            DrawLine(img, Act->Stroke, Act->Brush);
        //     case 2: DrawSmudge(img,Act.Stroke,Act.Brush);
        int realrad = Act->Brush.ClientStamp.rad_out * Act->Brush.ClientStamp.scale;
        // ActionData ac2;
        // ac2=Act;
        // emit ConfirmAction(ac2);
        // if (local) emit AffirmAction(Act);
        // if (!singlecore)
        emit SendDirtyRect(QRect(Act->Stroke.pos1.toPoint() - QPoint(realrad, realrad), Act->Stroke.pos1.toPoint() + QPoint(realrad, realrad)));
    }
    delete actlist;
}

void ArtMaster::DoAction(QImage *img, ActionData Act, bool local = true)
{
    if ((Act.ToolID == eSmudge) | (Act.ToolID == eDisp) | (Act.ToolID == eCont))
    {
        // //clamp position please move outside
        int twd = Act.Brush.ClientStamp.rad_out * Act.Brush.ClientStamp.scale;
        float cx = qMin(Act.Stroke.pos2.x(), (qreal)img->width() - twd);
        cx = qMax(cx, (float)twd);
        float cy = qMin(Act.Stroke.pos2.y(), (qreal)img->height() - twd);
        cy = qMax(cy, (float)twd);
        Act.Stroke.pos2.setX(cx);
        Act.Stroke.pos2.setY(cy);
    }
    if (Act.ToolID == eBrush)
        this->DrawBrush(img, Act.Stroke, Act.Brush);
    else if (Act.ToolID == eSmudge)
        DrawSmudge(img, Act.Stroke, Act.Brush);
    else if (Act.ToolID == eDisp)
        DrawDisp(img, Act.Stroke, Act.Brush);
    else if (Act.ToolID == eCont)
        LvlBrush(img, Act.Stroke, Act.Brush);
    else if (Act.ToolID == eLine)
        DrawLine(img, Act.Stroke, Act.Brush);
    //     case 2: DrawSmudge(img,Act.Stroke,Act.Brush);
    int realrad = Act.Brush.ClientStamp.rad_out * Act.Brush.ClientStamp.scale;
    // ActionData ac2;
    // ac2=Act;
    // emit ConfirmAction(ac2);
    // if (local) emit AffirmAction(Act);
    // if (!singlecore)
    emit SendDirtyRect(QRect(Act.Stroke.pos1.toPoint() - QPoint(realrad, realrad), Act.Stroke.pos1.toPoint() + QPoint(realrad, realrad)));
}

//---------------------------- L I N E ---------------------------------

void ArtMaster::DrawLine(QImage *img, d_Stroke STRK, BrushData BRSH)
{

    float wd = BRSH.ClientStamp.rad_out * 0.5;
    float bigrad = Dist2D(STRK.pos2, STRK.pos3);
    int xoff = wd;
    int yoff = wd;
    int dx23 = STRK.pos2.x() - STRK.pos3.x();
    int dy23 = STRK.pos2.y() - STRK.pos3.y();
    int dx12 = STRK.pos1.x() - STRK.pos2.x();
    int dy12 = STRK.pos1.y() - STRK.pos2.y();

    if (dx23 < 0)
        xoff = abs(dx23) + wd;
    if (dy23 < 0)
        yoff = abs(dy23) + wd;

    QPainterPath PST;
    PST.moveTo(QPoint(xoff, yoff));
    PST.lineTo(QPoint(xoff + dx23, yoff + dy23));
    QPainterPath PST2;
    PST2.moveTo(QPoint(xoff + dx23, yoff + dy23));
    PST2.lineTo(QPoint(xoff + dx23 + dx12, yoff + dy23 + dy12));

    // float bigrad=Dist2D(STRK.pos1,STRK.pos2);
    float mul = BRSH.ClientStamp.rad_out / bigrad * 0.5;

    QLinearGradient Grad;
    Grad.setSpread(QGradient::ReflectSpread);
    QPointF Gpos3;
    QPointF Gpos2;
    Gpos3.setX(xoff);
    Gpos3.setY(yoff);
    Gpos2.setX(xoff + (dx23));
    Gpos2.setY(yoff + (dy23));
    Grad.setStart(Gpos3);
    Grad.setFinalStop((Gpos2.y() - Gpos3.y()) * mul + Gpos3.x(), (-Gpos2.x() + Gpos3.x()) * mul + Gpos3.y());
    // Grad.setStart(STRK.pos2.x(),STRK.pos2.y());
    // Grad.setFinalStop((STRK.pos1.y()-STRK.pos2.y())*mul+STRK.pos2.x(),(-STRK.pos1.x()+STRK.pos2.x())*mul+STRK.pos2.y());

    QColor col1 = BRSH.ClientStamp.col;
    col1.setAlpha(0.0);
    Grad.setColorAt(0, BRSH.ClientStamp.col);
    Grad.setColorAt(1, col1);
    Grad.setColorAt((float)BRSH.ClientStamp.rad_in / BRSH.ClientStamp.rad_out, BRSH.ClientStamp.col);

    QImage Bimg(QSize(BRSH.ClientStamp.rad_out + abs(dx23), BRSH.ClientStamp.rad_out + abs(dy23)), QImage::Format_ARGB32);
    Bimg.fill(qRgba(0, 0, 0, 0));
    QPainter Bpainter(&Bimg);
    Bpainter.setCompositionMode(QPainter::CompositionMode_Source);
    // first stroke:
    QPen np;
    np.color() = Qt::black;
    np.setWidth(BRSH.ClientStamp.rad_out);
    np.setCapStyle(Qt::RoundCap);
    np.setJoinStyle(Qt::RoundJoin);
    np.setBrush(QBrush(Grad));
    Bpainter.setOpacity(BRSH.ClientStamp.opacity);
    Bpainter.strokePath(PST, np);
    // antifilling:
    np.setBrush(Qt::red);
    np.setCapStyle(Qt::FlatCap);
    Bpainter.setOpacity(0);
    Bpainter.strokePath(PST2, np);

    QPainter painter(img);
    painter.drawImage(STRK.pos3.x() - xoff, STRK.pos3.y() - yoff, Bimg);
}

// --------------------------- D R A W     S M U D G E ------------------------

void ArtMaster::DrawSmudge(QImage *img, d_Stroke STRK, BrushData BRSH)
{

    QPointF fpos = STRK.pos1;
    fpos.setX(fpos.x() - floor(fpos.x()));
    fpos.setY(fpos.y() - floor(fpos.y()));

    int wd = BRSH.ClientStamp.rad_out * 2;
    float tscale = BRSH.ClientStamp.scale;
    int twd = BRSH.ClientStamp.rad_out * tscale;
    QColor col1;
    // col1.setRgba(qRed(BRSH.col),qBlue(BRSH.col),qGreen(BRSH.col),1);
    col1 = BRSH.ClientStamp.col;

    // QRect Trect= QRect(STRK.pos2.x()-BRSH.rad_out,STRK.pos2.y()-BRSH.rad_out,STRK.pos2.x()+BRSH.rad_out,STRK.pos2.y()+BRSH.rad_out);
    QRect Trect = QRect(STRK.pos2.x() - twd, STRK.pos2.y() - twd, STRK.pos2.x() + twd, STRK.pos2.y() + twd); // scaled rect for correct mask application

    QImage Cimg(QSize(twd * 2 + 2, twd * 2 + 2), QImage::Format_ARGB32_Premultiplied);
    QPainter Cpainter(&Cimg);
    Cpainter.setCompositionMode(QPainter::CompositionMode_Source);

    Cpainter.drawImage(QPoint(0, 0), *img, Trect); // filling with image contents;
                                                   //    QImage SavedImg(Cimg);

    // -- SMUDGE APPLICATION MASK
    QImage Bimg(GenBMask(BRSH, fpos.x(), fpos.y()));
    //---------------------------------
    // now just rescaling noised image
    QImage Bimg2(QSize(twd * 2 + 2, twd * 2 + 2), QImage::Format_ARGB32_Premultiplied);
    Bimg2.fill(qRgba(0, 0, 0, 0));
    QPainter B2painter(&Bimg2);
    B2painter.setPen(Qt::NoPen);
    //-- opacity mask transformations
    float x2y = BRSH.ClientStamp.x2y;
    float y2x = 1 - (x2y - 0.5) * 2;
    y2x = qMin(y2x, (float)1);
    x2y = qMin(x2y * 2, (float)1);

    B2painter.translate(twd, twd);
    B2painter.rotate(BRSH.ClientStamp.resangle);
    B2painter.scale(tscale * x2y, tscale * y2x);
    B2painter.translate(-twd, -twd);
    B2painter.setOpacity(1);
    B2painter.setCompositionMode(QPainter::CompositionMode_Source);
    B2painter.drawImage(twd - wd * 0.5, twd - wd * 0.5, Bimg);
    /* old routine
        Cpainter.setCompositionMode(QPainter::CompositionMode_Source); //applying pregenerated mask
        Cpainter.setPen(Qt::NoPen);
        Cpainter.setOpacity(1);
        Cpainter.drawImage(0,0,Bimg2);

        Cpainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
         Cpainter.setOpacity(BRSH.CloneOpacity);
         Cpainter.drawImage(QPoint(0,0),*img,Trect);
    */

    QImage Acut(Cimg); // and taking for alpha cutting;
    QPainter ACpaint(&Acut);
    ACpaint.fillRect(Acut.rect(), col1);
    ACpaint.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    ACpaint.drawImage(0, 0, Bimg2);
    ACpaint.setCompositionMode(QPainter::CompositionMode_SourceOver);
    ACpaint.drawImage(0, 0, Cimg);

    // resaturating with brush color, EXCLUDING transparent pixels!;
    Cpainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    Cpainter.setOpacity(1.0 - BRSH.ClientStamp.cop);
    Cpainter.fillRect(Cimg.rect(), col1);

    Cpainter.setCompositionMode(QPainter::CompositionMode_DestinationIn); // applying pregenerated mask
    Cpainter.setPen(Qt::NoPen);
    Cpainter.setOpacity(1.0);
    Cpainter.drawImage(0, 0, Bimg2); // by now we have alphed cloned stamp;

    QPainter painter(img);
    painter.setPen(Qt::NoPen);

    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.setOpacity(BRSH.ClientStamp.opacity);
    painter.drawImage(STRK.pos1.x() - twd, STRK.pos1.y() - twd, Acut);

    painter.setCompositionMode(BMsel.BMmodes.at(BRSH.ClientStamp.bmidx)); // QPainter::CompositionMode_Darken);
    painter.setOpacity(BRSH.ClientStamp.opacity);
    painter.drawImage(STRK.pos1.x() - twd, STRK.pos1.y() - twd, Cimg);

    /*
    if (BRSH.preserveop==1){
        QPainter Spainter(&SavedImg);

        Spainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        Spainter.setOpacity(1);
        Spainter.drawImage(QPoint(0,0),*img,Trect);
        painter.setOpacity(1);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.drawImage(STRK.pos1.x()-twd,STRK.pos1.y()-twd,SavedImg);
    }
    */

    // delete Cimg;
    // delete Bimg;
    // delete Bimg2;
}

// --------------------------- D R A W   B R U S H ----------------------

void ArtMaster::DrawBrush(QImage *img, d_Stroke STRK, BrushData BRSH)
{
    QPointF fpos = STRK.pos1;
    fpos.setX(fpos.x() - floor(fpos.x()));
    fpos.setY(fpos.y() - floor(fpos.y()));

    int wd = BRSH.ClientStamp.rad_out * 2;
    float tscale = BRSH.ClientStamp.scale;
    int twd = BRSH.ClientStamp.rad_out * tscale;

    QRect Trect = QRect(STRK.pos1.x() - twd, STRK.pos1.y() - twd, STRK.pos1.x() + twd, STRK.pos1.y() + twd); // scaled rect for correct mask application

    QImage Cimg(QSize(twd * 2, twd * 2), QImage::Format_ARGB32_Premultiplied);
    QPainter Cpainter(&Cimg);
    Cpainter.setCompositionMode(QPainter::CompositionMode_Source);
    Cpainter.drawImage(QPoint(0, 0), *img, Trect); // filling with image contents;

    // -- Simple brush mask.
    QImage Bimg(GenBMask(BRSH, fpos.x(), fpos.y())); // unscaled brush
    //---------------------------------
    // now just rescaling noised image
    QImage Bimg2(QSize(twd * 2 + 2, twd * 2 + 2), QImage::Format_ARGB32_Premultiplied); // image with scaled brush;
    Bimg2.fill(qRgba(0, 0, 0, 0));

    QPainter B2painter(&Bimg2);
    B2painter.setPen(Qt::NoPen);
    //-- opacity mask transformations
    float x2y = BRSH.ClientStamp.x2y;
    float y2x = 1 - (x2y - 0.5) * 2;
    y2x = qMin(y2x, (float)1);
    x2y = qMin(x2y * 2, (float)1);

    B2painter.translate(twd, twd);
    B2painter.rotate(BRSH.ClientStamp.resangle);
    B2painter.scale(tscale * x2y, tscale * y2x);
    B2painter.translate(-twd, -twd);
    B2painter.setOpacity(1);
    B2painter.setCompositionMode(QPainter::CompositionMode_Source);
    B2painter.drawImage(twd - wd * 0.5, twd - wd * 0.5, Bimg);
    // if (BRSH.preserveop==1){
    //            B2painter.resetTransform();
    //        B2painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);

    // B2painter.resetMatrix();
    //        B2painter.drawImage(0,0,Cimg);
    //}

    // resaturating with brush color, EXCLUDING transparent pixels!;
    // Cpainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    // Cpainter.setOpacity(1-BRSH.CloneOpacity);
    // Cpainter.fillRect(Cimg.rect(),col1);

    //    Cpainter.setCompositionMode(QPainter::CompositionMode_DestinationIn); //applying pregenerated mask
    //    Cpainter.setPen(Qt::NoPen);
    //    Cpainter.setOpacity(1);
    //    Cpainter.drawImage(0,0,Bimg2);  // by now we have alphed cloned stamp;

    //  Cpainter.drawImage(0,0,Cimg);  // by now we have alphed cloned stamp;

    //---------

    QPainter painter(img);
    painter.setPen(Qt::NoPen);

    painter.setCompositionMode(BMsel.BMmodes.at(BRSH.ClientStamp.bmidx)); // QPainter::CompositionMode_Darken);
    painter.setOpacity(BRSH.ClientStamp.opacity);
    painter.drawImage(STRK.pos1.x() - twd, STRK.pos1.y() - twd, Bimg2); // was bimg2

    if (BRSH.ClientStamp.preserveop == 1)
    {
        Cpainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        Cpainter.setOpacity(1);
        Cpainter.drawImage(QPoint(0, 0), *img, Trect);
        painter.setOpacity(1);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.drawImage(STRK.pos1.x() - twd, STRK.pos1.y() - twd, Cimg); // was cimg
    }
    // painter.setOpacity(1);
    // painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    // painter.drawImage(STRK.pos1.x()-twd,STRK.pos1.y()-twd,Cimg);
    // delete Cimg;
    // delete Bimg;
    // delete Bimg2;
}

void ArtMaster::DrawBrushDev(QPaintDevice *img, d_Stroke STRK, BrushData BRSH)
{
    // int hwd=floor(BRSH.rad_out);
    // int wd=hwd*2;

    QPainter painter(img);

    // painter.setRenderHint(QPainter::Antialiasing,true);
    // painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    // painter.setCompositionMode(BRSH.compmode);//(QPainter::CompositionMode_SourceOver);
    painter.setCompositionMode(BMsel.BMmodes.at(BRSH.ClientStamp.bmidx));
    painter.setOpacity(BRSH.ClientStamp.opacity);

    // painter.drawImage(STRK.pos1,Bimg);
    // QMatrix m;
    //  ======= Rotation and scale

    QPointF fpos = STRK.pos1;
    fpos.setX(fpos.x() - floor(fpos.x()));
    fpos.setY(fpos.y() - floor(fpos.y()));

    QImage Bimg2(GenBMask(BRSH, fpos.x(), fpos.y()));

    float x2y = BRSH.ClientStamp.x2y;
    float y2x = 1 - (x2y - 0.5) * 2;
    y2x = qMin(y2x, (float)1);
    x2y = qMin(x2y * 2, (float)1);
    // painter.setRenderHints(QPainter::SmoothPixmapTransform);
    painter.translate(STRK.pos1.x(), STRK.pos1.y());
    painter.rotate(BRSH.ClientStamp.resangle);
    painter.scale(x2y * ((BRSH.ClientStamp.scale)), y2x * ((BRSH.ClientStamp.scale)));
    painter.translate(-STRK.pos1.x(), -STRK.pos1.y());
    // painter.translate(fpos);

    int decrad = ceil(BRSH.ClientStamp.rad_out);
    // if (decrad<2) decrad=1;
    int drwx = floor(STRK.pos1.x()) - decrad;
    int drwy = floor(STRK.pos1.y()) - decrad;
    painter.drawImage(drwx, drwy + 0.5, Bimg2);
    // delete Bimg2;
}

// --------------------------  L E V E L    B R U S H  ---------------------------------
void ArtMaster::LvlBrush(QImage *img, d_Stroke STRK, BrushData BRSH)
{
    int wd = BRSH.ClientStamp.rad_out * 2;
    float tscale = BRSH.ClientStamp.scale;
    int twd = BRSH.ClientStamp.rad_out * tscale;

    QPointF fpos = STRK.pos1;
    fpos.setX(fpos.x() - floor(fpos.x()));
    fpos.setY(fpos.y() - floor(fpos.y()));
    // clip position ---- Transfer this outside thanx
    /*
    float cx=qMin(STRK.pos1.x(),(qreal)img->width()-twd);
    cx=qMax(cx,(float)twd);
    float cy=qMin(STRK.pos1.y(),(qreal)img->height()-twd);
    cy=qMax(cy,(float)twd);
    STRK.pos1.setX(cx);
    STRK.pos1.setY(cy);
    */
    //-----------

    // QRect Trect= QRect(STRK.pos2.x()-BRSH.rad_out,STRK.pos2.y()-BRSH.rad_out,STRK.pos2.x()+BRSH.rad_out,STRK.pos2.y()+BRSH.rad_out);
    QRect Trect = QRect(STRK.pos1.x() - twd, STRK.pos1.y() - twd, STRK.pos1.x() + twd, STRK.pos1.y() + twd);

    QImage Cimg(QSize(twd * 2, twd * 2), QImage::Format_ARGB32);
    QPainter Cpainter(&Cimg);
    QColor col1 = BRSH.ClientStamp.col;
    Cpainter.fillRect(Cimg.rect(), col1);
    if (BRSH.ClientStamp.cop > 0)
    {
        Cpainter.setOpacity(BRSH.ClientStamp.cop);
        Cpainter.drawImage(QPoint(0, 0), *img, Trect);
    }
    int salr;
    int salg;
    int salb;
    int bpr;
    int bpg;
    int bpb;

    QRgb *px;
    QRgb *cpx;

    int wdt = Cimg.width();

    // now creating ordinary mask

    QRadialGradient BGrad(QPoint(BRSH.ClientStamp.rad_out, BRSH.ClientStamp.rad_out), BRSH.ClientStamp.rad_out);
    col1.setAlpha(0.0);
    BGrad.setColorAt(0, BRSH.ClientStamp.col);
    BGrad.setColorAt(1, col1);
    BGrad.setColorAt((float)BRSH.ClientStamp.rad_in / BRSH.ClientStamp.rad_out, BRSH.ClientStamp.col);

    QImage Bimg(GenBMask(BRSH, fpos.x(), fpos.y()));
    /*
       QImage Bimg(QSize(wd,wd), QImage::Format_ARGB32);
    Bimg.fill(qRgba(255,1,1,0));
    QPainter Bpainter(&Bimg);
    Bpainter.setPen(Qt::NoPen);
    Bpainter.setOpacity(1);
    Bpainter.setCompositionMode(QPainter::CompositionMode_Source);
    Bpainter.setBrush(QBrush(BGrad));
    Bpainter.drawEllipse(0,0,wd,wd);

    float fop=(BRSH.FadeCurveExp/255.0)*2-1.0;
    GenClamp(&Bimg,1-(BRSH.rad_in/BRSH.rad_out),0);
    if (fop<0) GenFocalInv(&Bimg,abs(fop));
        else GenFocal(&Bimg,fop);
    GenSolidity(&Bimg,BRSH.Solidity,1-BRSH.SolidityOfOpacity);
*/
    //---------------------------------
    // now just rescaling noised image
    QImage Bimg2(QSize(twd * 2, twd * 2), QImage::Format_ARGB32);
    Bimg2.fill(qRgba(255, 1, 1, 0));
    QPainter B2painter(&Bimg2);
    //-- opacity mask transformations
    float x2y = BRSH.ClientStamp.x2y;
    float y2x = 1 - (x2y - 0.5) * 2;
    y2x = qMin(y2x, (float)1);
    x2y = qMin(x2y * 2, (float)1);

    B2painter.translate(twd, twd);
    B2painter.rotate((BRSH.ClientStamp.resangle));
    B2painter.scale(tscale * x2y, tscale * y2x);
    B2painter.translate(-twd, -twd);
    //-- end opacity transforms
    B2painter.setOpacity(1);
    B2painter.setCompositionMode(QPainter::CompositionMode_Source);
    B2painter.drawImage(twd - wd * 0.5, twd - wd * 0.5, Bimg);

    // QRgb *px;
    // QRgb *cpx;
    for (int i = 0; i < twd * 2; i++)
    {
        px = (QRgb *)Bimg2.constScanLine(i);
        cpx = (QRgb *)Cimg.constScanLine(i);
        for (int j = 0; j < twd * 2; j++)
        {
            // sal=qAlpha(px[j])*(((qrand()/32768.0)<Solidity) ? 1 : 0); //solidity filter
            // sal=(((qrand()/128)<sal) ? 255*SolidityOfOpacity+sal*(1-SolidityOfOpacity) : sal*(1-SolidityOfOpacity));
            // ((char *)&cpx[j])[3]=qAlpha(px[j]);
            cpx[j] = qRgba(qRed(cpx[j]), qGreen(cpx[j]), qBlue(cpx[j]), qAlpha(px[j]));
        }
    }

    int midp = 128;
    int maxm = 0;
    int minm = 255;
    int rng = 20;
    float cv;
    for (int i = 0; i < wdt; i = i + 5)
    {
        cpx = (QRgb *)Cimg.constScanLine(i);
        for (int j = 0; j < wdt; j = j + 5)
        {
            if (qAlpha(cpx[j]) > 0)
            {
                cv = qRed(cpx[j]) + qGreen(cpx[j]) + qBlue(cpx[j]);
                cv = cv / 3;
                if (cv > maxm - rng)
                    maxm = (maxm + cv) / 2;
                if (cv < minm + rng)
                    minm = (minm + cv) / 2;
                //       if (cv>maxm) maxm=cv;
                //       if (cv>minm) minm=cv;
            }
        }
    }
    midp = ((maxm - minm) / 2) + minm;

    cpx = (QRgb *)crCont.constScanLine(midp);
    for (int i = 0; i < wdt; i++)
    {
        px = (QRgb *)Cimg.constScanLine(i);

        for (int j = 0; j < wdt; j++)
        {
            salr = qRed(px[j]);
            salg = qGreen(px[j]);
            salb = qBlue(px[j]);
            bpr = (cpx[qRed(px[j])]);
            bpg = (cpx[qGreen(px[j])]);
            bpb = (cpx[qBlue(px[j])]);
            px[j] = qRgba(bpr, bpg, bpb, qAlpha(px[j]));
            //((char *)&px[j])[3]=sal;
        }
    }

    QPainter painter(img);
    // painter.setCompositionMode(BRSH.compmode);//QPainter::CompositionMode_Darken);
    painter.setCompositionMode(BMsel.BMmodes.at(BRSH.ClientStamp.bmidx));
    painter.setOpacity(BRSH.ClientStamp.opacity);
    // painter.drawImage(STRK.pos1,Bimg);

    //    painter.translate(STRK.pos1.x(),STRK.pos1.y());
    //    painter.rotate(30.0);
    //   painter.translate(-STRK.pos1.x(),-STRK.pos1.y());

    painter.drawImage(STRK.pos1.x() - twd, STRK.pos1.y() - twd, Cimg);
}
// -------------------------- D I S P L A C E R    B R U S H ---------------------------

void ArtMaster::DrawDisp(QImage *img, d_Stroke STRK, BrushData BRSH)
{

    qreal pwr = BRSH.ClientStamp.pwr;
    pwr = pwr * 2;
    int wd = BRSH.ClientStamp.rad_out * 2;
    float tscale = BRSH.ClientStamp.scale;

    int twd = BRSH.ClientStamp.rad_out * tscale;

    float x2y;
    float y2x;
    QPointF fpos = STRK.pos1;
    fpos.setX(fpos.x() - floor(fpos.x()));
    fpos.setY(fpos.y() - floor(fpos.y()));
    // QRect Trect= QRect(STRK.pos2.x()-BRSH.rad_out,STRK.pos2.y()-BRSH.rad_out,STRK.pos2.x()+BRSH.rad_out,STRK.pos2.y()+BRSH.rad_out);
    QRect Trect = QRect(STRK.pos1.x() - twd, STRK.pos1.y() - twd, STRK.pos1.x() + twd, STRK.pos1.y() + twd); // rescaled rect, we dont need rescaled source image.

    QImage Cimg(QSize(twd * 2, twd * 2), QImage::Format_ARGB32);
    QPainter Cpainter(&Cimg);
    QColor col1 = BRSH.ClientStamp.col;
    Cpainter.fillRect(Cimg.rect(), col1);
    if (BRSH.ClientStamp.cop > 0)
    {
        Cpainter.setOpacity(BRSH.ClientStamp.cop);
        Cpainter.drawImage(QPoint(0, 0), *img, Trect);
    }

    QImage *DSMask;
    DSMask = &DispBrushes[0]; // new QImage(RESPATH+"/DBrushes/dblank.png");
    float dratio = wd * tscale / DSMask->width();

    QImage *DMask = new QImage(QSize(twd * 2, twd * 2), QImage::Format_ARGB32);
    QPainter DMpainter(DMask);
    DMpainter.fillRect(DMask->rect(), QColor::fromRgbF(0.5, 0.5, 0.5));
    x2y = BRSH.ClientStamp.x2y;
    y2x = 1 - (x2y - 0.5) * 2;
    y2x = qMin(y2x, (float)1);
    x2y = qMin(x2y * 2, (float)1);

    // DMpainter.translate(-twd,-twd);

    // DMpainter.translate(twd,twd);

    DMpainter.scale(dratio, dratio);
    DMpainter.translate(DSMask->width() / 2, DSMask->width() / 2);
    DMpainter.rotate(BRSH.ClientStamp.resangle);
    DMpainter.translate(-DSMask->width() / 2, -DSMask->width() / 2);
    // DMpainter.translate(-twd,-twd);

    DMpainter.drawImage(0, 0, *DSMask);
    DMpainter.end();
    // DMask=(DispBrushes.at(0));

    QImage Dimg(QSize(twd * 2, twd * 2), QImage::Format_ARGB32_Premultiplied); // diaplCED IMAGE WHICH WILL BE APPLIED.

    QRgb *px;
    QRgb *dpx;
    QRgb *cpx;
    // qreal st=pwr;

    //----------working cycle with main image
    qint16 yline;
    for (qint16 i = 0; i < twd * 2; i++)
    {
        px = (QRgb *)Dimg.constScanLine(i);
        dpx = (QRgb *)DMask->constScanLine(i);
        for (qint16 j = 0; j < twd * 2; j++)
        {

            //     cpx=(QRgb*)Cimg.scanLine(i+floor(qGreen(dpx[j]))-127);
            //   int xd=j+floor(qRed(dpx[j])-127);
            yline = (qint16)((i + floor((qGreen(dpx[j])) - 127) * pwr * dratio) + STRK.pos1.y() - twd);
            yline = qMax(yline, qint16(0));
            yline = qMin(yline, (qint16)(img->height() - 1));
            cpx = (QRgb *)img->constScanLine(yline);
            //      cpx=(QRgb*)img->constScanLine(
            //                (i+floor((qGreen(dpx[j]))-127)*st*dratio) + STRK.pos1.y()-twd
            //                 );
            qint16 xd = j + floor((qRed(dpx[j]) - 127) * pwr * dratio) + STRK.pos1.x() - twd;

            xd = qMax(xd, qint16(0));
            xd = qMin(xd, (qint16)(img->width() - 1));

            px[j] = qRgba(qRed(cpx[xd]), qGreen(cpx[xd]), qBlue(cpx[xd]), qAlpha(cpx[xd])); // cpx[xd];
        }
    }

    //----- end working cycle

    //--------------- working cycle with local brush -----------
    /*
    for (int i=0;i<twd*2;i++){
        px=(QRgb*)Dimg.scanLine(i);
        dpx=(QRgb*)DMask->scanLine(i);
        for (int j=0;j<twd*2;j++){
            cpx=(QRgb*)Cimg.scanLine(i+floor((qGreen(dpx[j]))-127)*st*dratio);
            int xd=j+floor((qRed(dpx[j])-127)*st*dratio);

            px[j]=cpx[xd];
       }
    }
    */
    //-------------- end working cycle with local --------------

    // QPainter Dpainter(&Dimg);
    // Cpainter.fillRect(Cimg.rect(),col1);

    // -- Disp APPLICATION MASK

    QRadialGradient BGrad(QPoint(BRSH.ClientStamp.rad_out, BRSH.ClientStamp.rad_out), BRSH.ClientStamp.rad_out);
    col1.setAlpha(0.0);
    BGrad.setColorAt(0, BRSH.ClientStamp.col);
    BGrad.setColorAt(1, col1);
    BGrad.setColorAt((float)BRSH.ClientStamp.rad_in / BRSH.ClientStamp.rad_out, BRSH.ClientStamp.col);

    QImage Bimg(GenBMask(BRSH, fpos.x(), fpos.y()));

    //    QImage Bimg(QSize(wd,wd), QImage::Format_ARGB32_Premultiplied); // bimg = opacity mask

    /*Bimg.fill(qRgba(255,1,1,0));
   QPainter Bpainter(&Bimg);
   Bpainter.setPen(Qt::NoPen);
   Bpainter.setOpacity(1);
   Bpainter.setCompositionMode(QPainter::CompositionMode_Source);
   Bpainter.setBrush(QBrush(BGrad));
   Bpainter.drawEllipse(0,0,wd,wd);
Bpainter.end();
   float fop=(BRSH.FadeCurveExp/255.0)*2-1.0;
   GenClamp(&Bimg,1-(BRSH.rad_in/BRSH.rad_out),0);
   if (fop<0) GenFocalInv(&Bimg,abs(fop));
       else GenFocal(&Bimg,fop);
   GenSolidity(&Bimg,BRSH.Solidity,1-BRSH.SolidityOfOpacity);
*/
    //---------------------------------
    // now just rescaling noised image
    QImage Bimg2(QSize(twd * 2, twd * 2), QImage::Format_ARGB32);
    Bimg2.fill(qRgba(255, 1, 1, 0));
    QPainter B2painter(&Bimg2);
    //-- opacity mask transformations
    x2y = BRSH.ClientStamp.x2y;
    y2x = 1 - (x2y - 0.5) * 2;
    y2x = qMin(y2x, (float)1);
    x2y = qMin(x2y * 2, (float)1);
    B2painter.translate(twd, twd);
    B2painter.rotate(BRSH.ClientStamp.resangle);
    B2painter.scale(tscale * x2y, tscale * y2x);
    B2painter.translate(-twd, -twd);
    //-- end opacity transforms
    B2painter.setOpacity(1);
    B2painter.setCompositionMode(QPainter::CompositionMode_Source);
    B2painter.drawImage(twd - wd * 0.5, twd - wd * 0.5, Bimg);

    // applying opacity mask,
    B2painter.end();

    QImage Acut(Dimg); // and taking for alpha cutting;
    QPainter ACpaint(&Acut);
    ACpaint.fillRect(Acut.rect(), BRSH.ClientStamp.col);
    ACpaint.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    ACpaint.drawImage(0, 0, Bimg2);
    ACpaint.setCompositionMode(QPainter::CompositionMode_SourceOver);
    ACpaint.drawImage(0, 0, Dimg);

    // applying mask to displaced image;
    QPainter dpainter(&Dimg);
    dpainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    dpainter.drawImage(0, 0, Bimg2);
    // dpainter.setCompositionMode(QPainter::CompositionMode_DestinationAtop);
    // dpainter.setOpacity(BRSH.CloneOpacity);
    // dpainter.drawImage(0,0,Bimg2);

    QPainter painter(img);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.setOpacity(BRSH.ClientStamp.opacity);
    painter.drawImage(STRK.pos1.x() - twd, STRK.pos1.y() - twd, Acut);

    painter.setCompositionMode(BMsel.BMmodes.at(BRSH.ClientStamp.bmidx)); // QPainter::CompositionMode_Darken);
    painter.setOpacity(BRSH.ClientStamp.opacity);
    painter.drawImage(STRK.pos1.x() - twd, STRK.pos1.y() - twd, Dimg); // Dimg is right one
    //    painter.drawImage(STRK.pos1.x()-twd,STRK.pos1.y()-twd,Acut);

    delete DMask;
}

QImage *ArtMaster::SubPixelate(QImage *Bimg, float fx, float fy)
{
    int wd = Bimg->width();
    QImage *NBimg = new QImage(QSize(wd + 1, wd + 1), QImage::Format_ARGB32);
    NBimg->fill(qRgba(0, 0, 0, 0));
    QRgb *sbpx;
    QRgb *px;
    QRgb *px1;
    // int resa;
    int fa; //,fr,fg,fb;
    float fmul;
    /*
        for (int i=0;i<wd;i++){
                    sbpx=(QRgb*)Bimg->scanLine(i);
                    px=(QRgb*)NBimg->scanLine(i);
                    px1=(QRgb*)NBimg->scanLine(i+1);

                    for (int j=0;j<wd;j++){


                        fmul=(1-fx)*(1-fy);

                        for (int k=0;k<4;k++){
                            fa=floor(((unsigned char *)&sbpx[j])[k]*fmul);
                            ((char *)&px[j])[k]+=fa;
                        }
                        fmul=(1-fx)*(fy);
                        for (int k=0;k<4;k++){
                            fa=floor(((unsigned char *)&sbpx[j])[k]*fmul);
                            ((char *)&px1[j])[k]+=fa;
                        }
                        fmul=(fx)*(1-fy);
                        for (int k=0;k<4;k++){
                            fa=floor(((unsigned char *)&sbpx[j])[k]*fmul);
                            ((char *)&px[j+1])[k]+=fa;
                        }
                        fmul=(fx)*(fy);

                        for (int k=0;k<4;k++){
                            fa=floor(((unsigned char *)&sbpx[j])[k]*fmul);
                            ((char *)&px1[j+1])[k]+=fa;

                        }

                   }
                }
    */
    return NBimg;
}
QImage ArtMaster::GenBMask(BrushData BRSH, float fx, float fy)
{

    bool sub3 = false;
    int wd = ceil(BRSH.ClientStamp.rad_out * 2) + 2; // big dia +1 pixel;
    qreal cposx = ceil(BRSH.ClientStamp.rad_out) + fx;
    qreal cposy = ceil(BRSH.ClientStamp.rad_out) + fy;
    qreal crad = BRSH.ClientStamp.rad_out; // radius
    if (wd < 5)
        sub3 = true;

    if (wd < 0)
    {
        wd = 4;
        sub3 = true;
        cposx = 1.5 + fx;
        cposy = 1.5 + fy;
        crad = 1.5;
    }

    QImage Bimg(QSize(wd, wd), QImage::Format_ARGB32_Premultiplied);

    // Bimg->fill(qRgba(BRSH.col.red(),BRSH.col.green(),BRSH.col.blue(),1));
    QPainter Bpainter(&Bimg);
    QPen pn;
    pn.setStyle(Qt::NoPen);
    pn.setWidth(0);
    Bpainter.setPen(pn);

    QColor col0 = BRSH.ClientStamp.col;
    if (wd >= 0)
    {
        // int drawrad=ceil(BRSH.rad_out);
        // float rmul=RngConv(drawrad-(BRSH.rad_out),1,0,1,(float)drawrad/floor(BRSH.rad_out));
        // float rmul = BRSH.rad_out/255;
        QRadialGradient BGrad(QPointF(cposx, cposy), crad); // QPoint(drawrad,drawrad),drawrad);
        QColor col1 = BRSH.ClientStamp.col;

        col0.toRgb().setAlpha(1);

        //  col0=QColor::fromRgba(qRgba(255,255,255,255));
        col1.setAlpha(0.0);
        // col1=QColor::fromRgba(qRgba(255,255,255,0));

        BGrad.setColorAt(0, col0);
        if (BRSH.ClientStamp.pipeID == plRS)
        {
            QColor midcol = col0;
            midcol.setAlpha(0.5);
            BGrad.setColorAt((BRSH.ClientStamp.rad_in / BRSH.ClientStamp.rad_out), col0);
        }
        BGrad.setColorAt(1, col1);
        // Bpainter.setRenderHints(QPainter::SmoothPixmapTransform);
        Bpainter.setPen(Qt::NoPen);
        Bpainter.setBrush(QBrush(BGrad)); // working line
        Bpainter.setCompositionMode(QPainter::CompositionMode_Source);
        // if (sub3) Bpainter.setOpacity((BRSH.rad_out*2)/6);
        Bpainter.drawRect(Bimg.rect());

        // ---new image code
    }

    //  if (wd<20) Bimg= SubPixelate(Bimg,fx,fy);

    /*
    QImage nbi=QImage(RESPATH+"/D:/brush1.png");
    Bpainter.drawImage(Bimg.rect(),nbi,nbi.rect());
    Bpainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    Bpainter.setBrush(BRSH.col);
    Bpainter.drawRect(Bimg.rect());
*/
    // -- EOB new image code
    // ------------------- noise cycle -------------

    float sol = BRSH.ClientStamp.sol;           // 0=full dissolve 1= opaque.
    float sol2op = 1 - BRSH.ClientStamp.sol2op; // 0=opacity is the same, 1 = full dissolvance
    float fop = BRSH.ClientStamp.crv;

    if (BRSH.ClientStamp.pipeID == plCFNSR)
    {
        GenClamp(&Bimg, 1 - (BRSH.ClientStamp.rad_in / BRSH.ClientStamp.rad_out), 0);
        if (fop <= 0)
            this->GenFocalInv(&Bimg, fabs(fop));
        else if (fop <= 1)
            this->GenFocal(&Bimg, fop);
        if (!((sol == 1) & (sol2op == 0)))
            GenSolidityP(&Bimg, sol, sol2op, BRSH.ClientStamp.noisex, BRSH.ClientStamp.noisey);
    }

    Bpainter.setBrush(col0);
    Bpainter.setOpacity(1);
    Bpainter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    Bpainter.drawRect(0, 0, wd, wd);

    return Bimg;
}

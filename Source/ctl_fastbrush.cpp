#include "ctl_fastbrush.h"
#include "BrushEngine/ClientBrushStamp.hpp"
#include "BrushEngine/LegacySharedBrush.hpp"
#include "ReMath/ReMath.hpp"
#include <QtGui>

ctl_FastBrush::ctl_FastBrush(ClientBrushStamp *brush, QWidget *parent) :
QWidget(parent), g_Brush(brush)
{
    // Brush =new BrushData;
    ZoomK = new float;

    this->setFixedSize(512, 512);
    BG = new QImage(this->size(), QImage::Format_ARGB32);
    // resizeImage(ViewCanvas,QSize(512,512));
    // this->setAttribute(Qt::WA_NoSystemBackground);
    // this->setAttribute(Qt::WA_OpaquePaintEvent);
    // this->setAttribute(Qt::WA_ContentsPropagated);
    midp = 256;
    anglerad = 128;
    ARTM = new ArtMaster();
    //  ARTM->corecount=1;
    //  ARTM->terminate();
    DBR = new BrushData;
    gotbrush = false;
}
void ctl_FastBrush::ForcePaint()
{
}

void ctl_FastBrush::paintEvent(QPaintEvent *event)
{
    // if (!repainted )
    {
        /*QImage tmpbr(QSize(128,128),QImage::Format_ARGB32_Premultiplied);
        QPainter ip(&tmpbr);
        ip.setCompositionMode(QPainter::CompositionMode_Source);
        ip.setBrush(QColor::fromRgbF(0,0,0,0));
        ip.drawRect(tmpbr.rect());*/

        // tmpbr.fill(QColor::fromRgbF(0,0,0,0));
        d_Stroke strk;
        strk.pos1 = QPointF(midp, midp);

        // painter.eraseRect(this->rect());

        // if ((gotbrush)&(!this->isHidden())) {
        QPainter painter2(this);
        painter2.drawImage(0, 0, *BG);
        painter2.end();

        ARTM->DrawBrushDev(this, strk, *DBR);
        QPainter painter(this);

        gotbrush = false;
        //}

        // painter.drawImage(0,0,tmpbr);
        // painter.setBrush(QColor::fromRgbF(0,0,0,1));
        // painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
        // painter.drawRect(this->rect());
        //    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        QPen np;
        np.setColor(Qt::black);
        np.setWidth(2);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(np);
        float drawradout = g_Brush->rad_out * (*ZoomK);
        float drawradin = g_Brush->rad_in * (*ZoomK);
        float rr = (qreal)g_Brush->rad_out * (qreal)(g_Brush->crv);
        qreal drawrelrad = drawradout * (g_Brush->crv);
        // painter.setCompositionMode(QPainter::CompositionMode_Xor);
        // painter.setPen(Qt::white);

        // painter.drawEllipse(QPointF(midp,midp),drawradout,drawradout);
        // painter.drawPie(QRect(midp-drawradin,midp-drawradin,drawradin*2,drawradin*2),16*30+16*120,16*120);

        float d30 = M_PI * 30 / 180;
        /*
        painter.drawLine(QPoint(midp,midp),QPoint(midp,midp)+QPoint(512*cos(-d30),512*sin(-d30)));
        painter.drawLine(QPoint(midp,midp),QPoint(midp,midp)+QPoint(512*cos(-d30*5),512*sin(-d30*5)));
        painter.drawLine(QPoint(midp,midp),QPoint(midp,midp)+QPoint(512*cos(-d30*9),512*sin(-d30*9)));
        painter.drawEllipse(QPointF(midp,midp),drawradout,drawradout);
        painter.drawPie(QRect(midp-drawradin,midp-drawradin,drawradin*2,drawradin*2),16*30+16*120,16*120);
        painter.drawPie(QRect(midp-drawrelrad,midp-drawrelrad,drawrelrad*2,drawrelrad*2),16*30+16*120+16*120,16*120);
        */
        np.setWidth(1);
        np.setColor(Qt::white);
        painter.setPen(np);
        painter.setCompositionMode(QPainter::CompositionMode_Exclusion);

        painter.drawLine(QPoint(midp, midp), QPoint(0, 1) + QPoint(midp, midp) + QPoint(512 * cos(-d30), 512 * sin(-d30)));
        painter.drawLine(QPoint(midp, midp), QPoint(0, 1) + QPoint(midp, midp) + QPoint(512 * cos(-d30 * 5), 512 * sin(-d30 * 5)));
        painter.drawLine(QPoint(midp, midp), QPoint(1, 0) + QPoint(midp, midp) + QPoint(512 * cos(-d30 * 9), 512 * sin(-d30 * 9)));

        painter.drawEllipse(QPointF(midp, midp), drawradout, drawradout);
        painter.drawArc(QRect(midp - drawradin, midp - drawradin, drawradin * 2, drawradin * 2), 16 * 30 + 16 * 120, 16 * 120);
        painter.drawArc(QRect(midp - drawrelrad, midp - drawrelrad, drawrelrad * 2, drawrelrad * 2), 16 * 30 + 16 * 120 + 16 * 120, 16 * 120);

        // painter.drawText(127,0,100,23,0,QString::number(ang));

        // drawing angle arrow
        float rang = (g_Brush->resangle * M_PI * 2 / 360);
        QPoint head(anglerad * cos(rang), anglerad * sin(rang));
        np.setColor(Qt::red);
        np.setWidth(2);
        painter.setPen(np);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawLine(QPoint(midp, midp), QPoint(midp, midp) + head);
        painter.drawLine(QPoint(midp, midp) + head, QPoint(midp, midp) + head + QPoint(10 * cos(rang - M_PI + M_PI * 0.2), 10 * sin(rang - M_PI + M_PI * 0.2)));
        painter.drawLine(QPoint(midp, midp) + head, QPoint(midp, midp) + head + QPoint(10 * cos(rang - M_PI - M_PI * 0.2), 10 * sin(rang - M_PI - M_PI * 0.2)));
        repainted = true;
    }
}

void ctl_FastBrush::ReBrush(ActionData act)
{
    *DBR = act.Brush;
    DBR->ClientStamp.scale = DBR->ClientStamp.scale * (*ZoomK);
    gotbrush = true;
    if (repainted == true)
    {
        repainted = false;
        this->update();
    }
}

void ctl_FastBrush::mousePressEvent(QMouseEvent *event)
{
    float rad = Dist2D(event->pos(), QPoint(midp, midp));

    rad = rad / (*ZoomK);

    ang = AtanXY(event->pos().x() - midp, event->pos().y() - midp);
    float d30 = M_PI * 30 / 180;
    if (fabs(((ang + M_PI) * 180 / M_PI) - g_Brush->resangle) < 5)
        brushRegulationMode = 4;
    else if ((ang > d30) & (ang < d30 * 5))
        brushRegulationMode = 1;
    else if ((ang < d30) & (ang > (-M_PI * 0.5)))
        brushRegulationMode = 2;
    else if ((ang > d30 * 5) | (ang < (-M_PI * 0.5)))
        brushRegulationMode = 3;
}

void ctl_FastBrush::tabletEvent(QTabletEvent *event)
{
    float rad = Dist2D(event->pos(), QPoint(midp, midp));

    rad = rad / (*ZoomK);

    ang = AtanXY(event->pos().x() - midp, event->pos().y() - midp);
    float d30 = M_PI * 30 / 180;

    if (event->type() == QEvent::TabletPress)
    {
        if (fabs(((ang + M_PI) * 180 / M_PI) - g_Brush->resangle) < 5)
            brushRegulationMode = 4;
        else if ((ang > d30) & (ang < d30 * 5))
            brushRegulationMode = 1;
        else if ((ang < d30) & (ang > (-M_PI * 0.5)))
            brushRegulationMode = 2;
        else if ((ang > d30 * 5) | (ang < (-M_PI * 0.5)))
            brushRegulationMode = 3;
    }
    if (event->type() == QEvent::TabletMove)
    {
        if (brushRegulationMode > 0)
        {
            ParsePos(event->pos());
        }
    }
    if (event->type() == QEvent::TabletRelease)
    {
        brushRegulationMode = 0;
    }
}
void ctl_FastBrush::ParsePos(QPoint pos)
{

    //    float radrel=Brush->rad_out/Brush->rad_in;
    int mode;
    ang = AtanXY(pos.x() - midp, pos.y() - midp);
    if ((pos.x() - midp) == 0)
    {

        ang = 0;
        if (pos.y() - midp > 0)
            ang = -M_PI * 0.5;
    }
    float rad = Dist2D(pos, QPoint(midp, midp));

    float absrad = rad;
    rad = rad / (*ZoomK);
    rad = qMin((float)256, rad);
    float d30 = M_PI * 30 / 180;

    if ((ang > d30) & (ang < d30 * 5))
        mode = 1;
    if ((ang < d30) & (ang > (-M_PI * 0.5)))
        mode = 2;
    if ((ang > d30 * 5) | (ang < (-M_PI * 0.5)))
        mode = 3;

    if (brushRegulationMode == 1)
    { // radius
        float rel = g_Brush->rad_in / g_Brush->rad_out;
        if (!((ang > d30) & (ang < d30 * 5)))
            (rad = Discrete(rad, 10));
        // if (mode !=1) rad=0;
        g_Brush->rad_out = rad;
        g_Brush->rad_in = g_Brush->rad_out * rel;
    }
    if (brushRegulationMode == 2)
    { // rad rel
        g_Brush->rad_in = qMin(rad, g_Brush->rad_out);
        if (mode != 2)
            g_Brush->rad_in = 0;

        if (g_Brush->rad_in < 7)
            g_Brush->rad_in = 0;
        float rel = g_Brush->rad_in / g_Brush->rad_out;
        if (rel > 0.99)
            rel = 0.98;

        emit SendRel(rel);
    }
    if (brushRegulationMode == 3)
    { // curve
        qreal ir = qMin(rad, g_Brush->rad_out);
        if (mode != 3)
            ir = 0;
        qreal tcrv = (qreal)ir / (qreal)(g_Brush->rad_out);
        g_Brush->crv = tcrv;
        emit SendCrv(1 - tcrv);
    }
    if (brushRegulationMode == 4)
    {
        ang = (ang + M_PI) * 180 / M_PI;
        if ((absrad > anglerad + 32) | (rad < 20))
            ang = Discrete(ang, 45 * 0.5);
        g_Brush->resangle = ang;
    }

    emit AskBrush();
    // ForcePaint();
    this->update();
    QApplication::processEvents(QEventLoop::AllEvents, 10);
}

void ctl_FastBrush::mouseMoveEvent(QMouseEvent *event)
{
    ParsePos(event->pos());
}

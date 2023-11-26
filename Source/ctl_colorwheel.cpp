#include "ctl_colorwheel.h"
// #include <QConicalGradient>
#include <QGradient>
#include <QPainter>
#include <QtGui>
#include <math.h>
ctl_ColorWheel::ctl_ColorWheel()

{
    LColor = LColor.toHsl();
    float baseh = 512;
    float wdmul = 1.362;
    int wd = baseh * wdmul;
    int hg = baseh;
    SelMode = 0;
    KMode = 0;
    resizeImage(&ViewImage, QSize(wd, hg));
    rad = (float)wd * 0.5;

    this->setFixedWidth(wd);
    this->setFixedHeight(hg);

    DrawHUEWheel();
    // DrawVSWheel();
}
void ctl_ColorWheel::DrawHUEWheel()
{

    int pos = ViewImage.height() / 2;
    QConicalGradient grad(pos, pos, 0);
    float step = RngConv(M_PI / 6, 0, M_PI, 0, 1);
    grad.setColorAt(0, Qt::red);
    grad.setColorAt(0 + step, Qt::yellow);
    grad.setColorAt(0 + step * 2, Qt::green);
    grad.setColorAt(0 + step * 3, Qt::cyan);
    grad.setColorAt(0 + step * 4, Qt::blue);
    grad.setColorAt(0 + step * 5, QColor::fromRgbF(1, 0, 1, 1));
    grad.setColorAt(1, Qt::red);
    QPainter painter(&ViewImage);
    // painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(this->rect());

    painter.setBrush(QBrush(grad));
    painter.drawEllipse(QPoint(pos, pos), pos - 16, pos - 16);

    float offp = 0.05;
    QRadialGradient grad2(pos, pos, pos);
    grad2.setColorAt(0, Qt::white);
    grad2.setColorAt(0 + offp, Qt::white);
    grad2.setColorAt(0.5 - offp, QColor::fromRgbF(1.0, 1.0, 1.0, 0.0));

    painter.setCompositionMode(QPainter::CompositionMode_Screen);
    painter.setBrush(QBrush(grad2));
    painter.drawEllipse(QPoint(pos, pos), pos, pos);

    grad2.setColorAt(0, QColor::fromRgbF(1.0, 1.0, 1.0, 0.0));
    grad2.setColorAt(0.5 + offp, QColor::fromRgbF(0.0, 0.0, 0.0, 0.0));
    grad2.setColorAt(1 - offp, QColor::fromRgbF(0.0, 0.0, 0.0, 1));
    grad2.setColorAt(1, Qt::black);
    painter.setBrush(QBrush(grad2));
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    painter.drawEllipse(QPoint(pos, pos), pos, pos);
}
void ctl_ColorWheel::DrawSliders(QColor col)
{
    int brd = 1;
    float step = (this->width() - this->height() - brd * 4) / 3;

    QPainter painter(&ViewImage);
    painter.setBrush(Qt::black);
    painter.drawRect(this->height(), 0, this->width(), this->height());
    painter.setBrush(Qt::white);

    QLinearGradient grad(0, 0, 0, this->height());
    float gstep = RngConv(M_PI / 6, 0, M_PI, 0, 1);
    grad.setColorAt(0, QColor::fromHslF(0, LColor.hslSaturationF(), LColor.lightnessF()));
    grad.setColorAt(0 + gstep, QColor::fromHslF(gstep, LColor.hslSaturationF(), LColor.lightnessF()));
    grad.setColorAt(0 + gstep * 2, QColor::fromHslF(gstep * 2, LColor.hslSaturationF(), LColor.lightnessF()));
    grad.setColorAt(0 + gstep * 3, QColor::fromHslF(gstep * 3, LColor.hslSaturationF(), LColor.lightnessF()));
    grad.setColorAt(0 + gstep * 4, QColor::fromHslF(gstep * 4, LColor.hslSaturationF(), LColor.lightnessF()));
    grad.setColorAt(0 + gstep * 5, QColor::fromHslF(gstep * 5, LColor.hslSaturationF(), LColor.lightnessF()));
    grad.setColorAt(1, QColor::fromHslF(0, LColor.hslSaturationF(), LColor.lightnessF()));
    painter.setBrush(grad);
    painter.drawRect(this->height() + brd - 1, brd - 1, this->height() + brd + step, this->height() - brd);

    QLinearGradient satgrad(0, 0, 0, this->height());
    satgrad.setColorAt(0, QColor::fromHslF(LColor.hueF(), 1, LColor.lightnessF()));
    satgrad.setColorAt(1, QColor::fromHslF(LColor.hueF(), 0, LColor.lightnessF()));
    painter.setBrush(satgrad);
    painter.drawRect(this->height() + brd * 2 + step - 1, brd - 1, this->height() + brd * 2 + step * 2 - 1, this->height() - brd - 1);

    satgrad.setColorAt(0, QColor::fromHslF(LColor.hueF(), LColor.hslSaturationF(), 1));
    satgrad.setColorAt(0.5, QColor::fromHslF(LColor.hueF(), LColor.hslSaturationF(), 0.5));
    satgrad.setColorAt(1, QColor::fromHslF(LColor.hueF(), LColor.hslSaturationF(), 0));
    painter.setBrush(satgrad);

    painter.drawRect(this->height() + brd * 3 + step * 2 - 1, brd - 1, this->height() + brd * 3 + step * 3, this->height() - brd - 1);
}

void ctl_ColorWheel::DrawLSWheel()
{
    // int pos=ViewImage.width()/2;
    QColor col = QColor::fromHslF(savehue, savesat, saveval, 1.0);
    QColor cols = QColor::fromHslF(savehue, 0, saveval, 1.0);
    QLinearGradient grad(0, 0, 256, 0);
    grad.setColorAt(0, LColor);
    grad.setColorAt(0.1, LColor);
    grad.setColorAt(0.9, cols);
    grad.setColorAt(1, cols);
    QPainter painter(&ViewImage);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(grad));
    painter.drawRect(ViewImage.rect());

    QLinearGradient grad2(0, 0, 0, 256);
    grad2.setColorAt(0, Qt::white);
    grad2.setColorAt(0.1, Qt::white);
    grad2.setColorAt(0.9, Qt::black);
    grad2.setColorAt(1, Qt::black);
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    painter.setBrush(QBrush(grad2));
    painter.drawRect(ViewImage.rect());
}
void ctl_ColorWheel::DrawVSWheel()
{
    // int pos=ViewImage.width()/2;
    QColor col = QColor::fromHslF(savehue, savesat, saveval, 1.0);
    QColor colg = QColor::fromHslF(savehue, 0, saveval, 1.0);
    QColor cols = QColor::fromHslF(savehue, 1, saveval, 1.0);

    // QColor cols=QColor::fromHslF(savehue,0,saveval,1.0);

    QLinearGradient grad(0, 0, (this->width()), 0);
    float sp = 0.05;

    grad.setColorAt(0, colg);
    grad.setColorAt(0 + sp, colg);
    grad.setColorAt(0.5 - sp, col);
    grad.setColorAt(0.5 + sp, col);
    grad.setColorAt(1 - sp, cols);
    grad.setColorAt(1, cols);

    QPainter painter(&ViewImage);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(grad));
    painter.drawRect(ViewImage.rect());

    QLinearGradient grad2(0, 0, 0, 256);
    grad2.setColorAt(0, Qt::white);
    grad2.setColorAt(0.5 - sp, QColor::fromRgbF(1.0, 1.0, 1.0, 0.0));
    grad2.setColorAt(0.5 + sp, QColor::fromRgbF(0.0, 0.0, 0.0, 0.0));
    // grad2.setColorAt(0.5+sp+0.25+sp/2,QColor::fromRgbF(0.0,0.0,0.0,0.3));
    grad2.setColorAt(1 - sp, Qt::black);
    grad2.setColorAt(1, Qt::black);
    // painter.setCompositionMode(QPainter::CompositionMode_);
    painter.setBrush(QBrush(grad2));
    painter.drawRect(ViewImage.rect());
}

void ctl_ColorWheel::DrawSwatch()
{
    float diag = this->height() * sqrt(2.0) * 0.5;
    float rad = (diag - this->height() * 0.5) * cos(M_PI_4);
    QPainter painter(&ViewImage);
    painter.setBrush(LColor);
    // painter.setPen(Qt::NoPen);
    // painter.drawRect(240,240,255,255);
    painter.drawEllipse(0, 0, rad, rad);
}

void ctl_ColorWheel::paintEvent(QPaintEvent *event)
{
    bctl_ImageWidget::paintEvent(event);
    QPainter painter(this);
    float ang = (1 - LColor.hueF()) * M_PI * 2;
    // ang=Discrete(ang,M_PI/6);
    float rad = (LColor.hslSaturationF()) * 255;
    float x = rad * cos(ang) + 255;
    float y = rad * sin(ang) + 255;
    painter.drawEllipse(x, y, 10, 10);
    // draw sliders boxes;

    float step = (this->width() - this->height()) / 3;
    int rh = 13;

    float chue = LColor.hueF();
    float csat = LColor.hslSaturationF();
    float cval = LColor.lightnessF();

    painter.drawRect(this->height(), chue * this->height() - rh, step, rh + rh + 1);
    painter.drawRect(this->height() + step, (1 - csat) * this->height() - rh, step, rh + rh + 1);
    painter.drawRect(this->height() + step * 2, (1 - cval) * this->height() - rh, step, rh + rh + 1);

    painter.setBrush(LColor);
    painter.setPen(Qt::NoPen);

    float coff = step / 1.618;
    float cw = step - coff;

    painter.drawRect(this->height() + 1 + coff, chue * this->height() - rh + 1, cw, rh + rh);
    painter.drawRect(this->height() + step + 1 + coff, (1 - csat) * this->height() - rh + 1, cw, rh + rh);
    painter.drawRect(this->height() + step * 2 + 1 + coff, (1 - cval) * this->height() - rh + 1, cw, rh + rh);
}

void ctl_ColorWheel::keyPressEvent(QKeyEvent *event)
{
    // if (event->modifiers()==Qt::AltModifier)
    KMode = 0;
}
void ctl_ColorWheel::keyReleaseEvent(QKeyEvent *event)
{
    KMode = 1;
}
void ctl_ColorWheel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        SelMode = 2;
    }
    else
        SelMode = 1;

    SwState = true;

    if (event->pos().x() > this->height())
    {
        float relpos = RngConv(event->pos().x() - this->height(), 0, this->width() - this->height(), 0, 3);
        int r = floor(relpos); // Discrete(event->pos().x-this->height(),this->width()-this->height()/3);
        SelMode = 10 + r;
    }

    else
    {

        QPoint ClamPos;
        ClamPos = event->pos();
        ClamPos.setX(Clamp(ClamPos.x(), this->width() - 1));
        ClamPos.setY(Clamp(ClamPos.y(), this->height() - 1));

        LColor = QColor::fromRgb(ViewImage.pixel(ClamPos));
        savehue = LColor.hueF();
        savesat = LColor.hslSaturationF();
        saveval = LColor.lightnessF();

        emit SendColor(LColor);
    }
}

void ctl_ColorWheel::mouseMoveEvent(QMouseEvent *event)
{

    QPoint ClamPos;
    ClamPos = event->pos();
    ClamPos.setX(Clamp(ClamPos.x(), this->width() - 1));
    ClamPos.setY(Clamp(ClamPos.y(), this->height() - 1));

    if (SwState)
    {
        //       DrawHUEWheel();
        SwState = false;
    }
    if (SelMode == 1)
    {
        LColor = QColor::fromRgb(ViewImage.pixel(ClamPos));
        savehue = LColor.hueF();
        savesat = LColor.hslSaturationF();
        saveval = LColor.lightnessF();
        DrawSliders(LColor);
        g_Brush->col = qRgba(LColor.red(), LColor.green(), LColor.blue(), qAlpha(g_Brush->col));
    }

    else
    { // click in sliders area
        int r = SelMode - 10;
        float step = (this->width() - this->height()) / 3;
        if ((event->pos().x() < this->height() - 20 - step * r) | (event->pos().x() > this->height() + 20 + step * (r + 1)))
        {
            ClamPos.setY(Discrete(ClamPos.y(), this->height() / 12));
        }

        if (SelMode == 10)
        {
            savehue = ((float)ClamPos.y() / this->height());
            LColor = QColor::fromHslF(savehue, savesat, saveval);
        }
        else if (SelMode == 11)
        {
            savesat = 1 - ((float)ClamPos.y() / this->height());
            LColor = QColor::fromHslF(savehue, savesat, saveval);
        }
        else if (SelMode == 12)
        {
            saveval = 1 - ((float)ClamPos.y() / this->height());
            LColor = QColor::fromHslF(savehue, savesat, saveval);
        }
    }
    DrawSliders(LColor);

    g_Brush->col = qRgba(LColor.red(), LColor.green(), LColor.blue(), qAlpha(g_Brush->col));
    DrawSwatch();
    update();

    // d_Stroke strk;
    // strk.pos1=event->pos();
    // emit SendStroke(strk);
}
void ctl_ColorWheel::mouseReleaseEvent(QMouseEvent *event)
{
    // DrawLSWheel();
    // DrawHUEWheel();
    DrawSliders(LColor);
    emit SendColor(LColor);
    this->update();
    SelMode = 0;
}

void ctl_ColorWheel::SetColor(QColor col)
{
    LColor = col;
    savehue = LColor.hueF();
    savesat = LColor.hslSaturationF();
    saveval = LColor.lightnessF();
    g_Brush->col = qRgba(LColor.red(), LColor.green(), LColor.blue(), qAlpha(g_Brush->col));
}

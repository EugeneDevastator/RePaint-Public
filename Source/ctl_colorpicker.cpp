#include "ctl_colorpicker.h"
#include <QPainter>

ctl_ColorPicker::ctl_ColorPicker(QWidget *parent) : QWidget(parent)
{
    SquareArea = new QImage(this->size(), QImage::Format_ARGB32);
    RecalcMainGrad();
}
void ctl_ColorPicker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, *SquareArea, dirtyRect);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::NoBrush);
    painter.setCompositionMode(QPainter::CompositionMode_Difference);
    painter.drawEllipse(QPoint(Lit * this->width(), Sat * this->height()), 5, 5);
}
void ctl_ColorPicker::RecalcMainGrad()
{
    QPainter SQpainter(SquareArea);
    QLinearGradient grad = QLinearGradient(2, 0, 2, this->height());
    QLinearGradient litgrad = QLinearGradient(0, 0, this->width(), 0);

    /*
                float step=RngConv(M_PI/6,0,M_PI,0,1);
                grad.setColorAt(0,Qt::red);
                grad.setColorAt(0+step,Qt::yellow);
                grad.setColorAt(0+step*2,Qt::green);
                grad.setColorAt(0+step*3,Qt::cyan);
                grad.setColorAt(0+step*4,Qt::blue);
                grad.setColorAt(0+step*5,QColor::fromRgbF(1,0,1,1));
                grad.setColorAt(1,Qt::red);
    */
    grad.setColorAt(0, QColor::fromHslF(0, 0, 0.5, 0));
    grad.setColorAt(1, QColor::fromHslF(0, 0, 0.5, 1));
    // CtlSat->Gslider->grad->setColorAt(0,QColor::fromHslF(1,0,0.5));
    // CtlSat->Gslider->grad->setColorAt(1,QColor::fromHslF(1,1,0.5));

    litgrad.setColorAt(0, QColor::fromHslF(0, 0, 0, 1));
    litgrad.setColorAt(0.4999, QColor::fromHslF(0, 0, 0, 0));
    litgrad.setColorAt(0.5, QColor::fromHslF(0, 0, 1, 0));
    litgrad.setColorAt(1, QColor::fromHslF(0, 0, 1, 1));
    SQpainter.setBrush(Qt::green);
    SQpainter.drawRect(SquareArea->rect());
    SQpainter.setBrush(grad);
    SQpainter.drawRect(SquareArea->rect());
    SQpainter.setBrush(litgrad);
    SQpainter.drawRect(SquareArea->rect());
    // SQpainter.setCompositionMode(QPainter::CompositionMode_Multiply);
    // SQpainter.drawRect(0,0,SquareArea->width()*0.5,SquareArea->height());
    // SQpainter.setCompositionMode(QPainter::CompositionMode_Screen);
    // SQpainter.drawRect(SquareArea->width()*0.5,0,SquareArea->width(),SquareArea->height());
}

void ctl_ColorPicker::resizeEvent(QResizeEvent *event)
{
    resizeImage(SquareArea, event->size());
    RecalcMainGrad();
}
void ctl_ColorPicker::mousePressEvent(QMouseEvent *event)
{
    Lit = ((qreal)event->pos().x() / this->width());
    Sat = ((qreal)event->pos().y() / this->height());
    if (Lit < 0)
    {
        Lit = 0;
        Sat = Discrete(Sat, 0.25);
    }
    if (Sat < 0)
    {
        Sat = 0;
        Lit = Discrete(Lit, 0.25);
    }
    this->update();
}

void ctl_ColorPicker::mouseMoveEvent(QMouseEvent *event)
{
    Lit = ((qreal)event->pos().x() / this->width());
    Sat = ((qreal)event->pos().y() / this->height());
    if (Lit < 0)
    {
        Lit = 0;
        Sat = Discrete(Sat, 0.25 * 0.5);
    }
    if (Lit > 1)
    {
        Lit = 1;
        Sat = Discrete(Sat, 0.25 * 0.5);
    }
    if (Sat < 0)
    {
        Sat = 0;
        Lit = Discrete(Lit, 0.25 * 0.5);
    }
    if (Sat > 1)
    {
        Sat = 1;
        Lit = Discrete(Lit, 0.25 * 0.5);
    }
    this->update();
}

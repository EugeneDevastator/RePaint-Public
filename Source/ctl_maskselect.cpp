#include "ctl_maskselect.h"

ctl_MaskSelect::ctl_MaskSelect(QWidget *parent) : QWidget(parent)
{
    cellsize = 128;
    MaskList = new QList<QImage *>;
    this->setFixedWidth(768);
    this->setFixedHeight(512);
    ViewCanvas = new QImage(QSize(this->width(), this->height()), QImage::Format_ARGB32);
    // ViewCanvas->fill(Qt::white);
    QPainter pnt(ViewCanvas);
    pnt.setBrush(Qt::white);
    pnt.drawRect(this->rect());
}
void ctl_MaskSelect::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    QRect dirtyRect = event->rect();
    //    painter.drawImage(dirtyRect, ViewCanvas, dirtyRect);
    //  if (ZoomK<1) painter.setRenderHints(QPainter::SmoothPixmapTransform);
    // painter.drawImage(dirtyRect, ViewCanvas, QRect(dirtyRect.x()+Offx/ZoomK,dirtyRect.y()+Offy/ZoomK,dirtyRect.width()/ZoomK,dirtyRect.height()/ZoomK));
    painter.drawImage(dirtyRect, *ViewCanvas, dirtyRect);
}

void ctl_MaskSelect::mouseMoveEvent(QMouseEvent *event)
{
    QPoint dpos;
    dpos = event->pos() - ppos;
    scrollpos += dpos;

    ppos = event->pos();
    int crx = floor(event->pos().x() / cellsize);
    int cry = floor((event->pos().y() - scrollpos.y()) / cellsize);

    int total = MaskList->count();
    int hc = floor(this->width() / cellsize);

    int i = 0;
    int j = 0;
    // int offj=floor(scrollpos.x()/cellsize);
    // int offi=floor(scrollpos.y()/cellsize);

    QPainter pnt(ViewCanvas);
    pnt.setBrush(Qt::white);
    pnt.drawRect(this->rect());
    for (int c = 1; c <= total; c++)
    {
        i = floor(c / hc);
        j = c - hc * i;
        pnt.drawImage(QRect(j * cellsize, i * cellsize + scrollpos.y(), cellsize, cellsize), *MaskList->at(c - 1), MaskList->at(c - 1)->rect());
    }
    pnt.drawText(QRect(0, cellsize * 0.5 - 15 + scrollpos.y(), cellsize, cellsize), "Innate Brush");
    pnt.setCompositionMode(QPainter::CompositionMode_Difference);
    pnt.setBrush(Qt::white);
    pnt.drawRect(QRect(crx * cellsize, cry * cellsize + scrollpos.y(), cellsize, cellsize));

    this->update();
}
void ctl_MaskSelect::mousePressEvent(QMouseEvent *event)
{
    ppos = event->pos();
}

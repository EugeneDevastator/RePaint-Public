#include "bctl_imagewidget.h"
#include <QtGui>

bctl_ImageWidget::bctl_ImageWidget(QWidget *parent) : QWidget(parent)
{
}
void bctl_ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, ViewImage, dirtyRect);
}

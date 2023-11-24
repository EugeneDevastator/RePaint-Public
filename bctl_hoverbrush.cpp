#include "bctl_hoverbrush.h"


bctl_HoverBrush::bctl_HoverBrush(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(64,64);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_ContentsPropagated);
}
void bctl_HoverBrush::GetNewB(QPoint centralpos, int rad, qreal tiltx, qreal tilty){
    setFixedSize(rad*2,rad*2);
    this->move(centralpos-QPoint(rad,rad));
    this->update();

}

void bctl_HoverBrush::paintEvent(QPaintEvent *event){
    QPainter painter(this);
int hw = this->width()*0.5;
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(QPoint(hw,hw),hw-1,hw-1);
}
void bctl_HoverBrush::SetBSize(QSize sz){

}

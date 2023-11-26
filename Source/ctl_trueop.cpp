#include "ctl_trueop.h"
#include <QtGui>
#include "brushes.h"
ctl_trueop::ctl_trueop(QWidget *parent) : QWidget(parent)
{
    col = QColor::fromRgbF(0.5, 1, 0, 1);
    // op=0.2;
    //  spc=0.5;
    op = 1;
    spc = 0.3;
    // this->setAttribute(Qt::WA_NoBackground);
    hue = new double;
    sat = new double;
    lit = new double;
    repainted = false;
    crv = new double;
    rrel = new double;
    this->setFixedWidth(64);
    this->setFixedHeight(32);
    BG = new QImage(this->geometry().size(), QImage::Format_ARGB32);
    QPainter pnt(BG);
    pnt.drawRect(BG->rect());
    QPainter painter(this);
    // painter.eraseRect(this->rect());
    painter.drawImage(0, 0, *BG);
    this->update();
}
void ctl_trueop::paintEvent(QPaintEvent *event)
{
    //    if (!repainted)
    {

        float rrmax = 1.7; // originally was 1.7
        float rrmin = 1.5;
        float crvmax = (rrmax / rrmin) - 0.1;

        float trueop = 0.7 * (op / pow(spc, 0.5)) * RngConv(*rrel, 0, 1, rrmin, rrmax) * RngConv(*crv, -1, 1, RngConv(*rrel, 0, 1, crvmax, 1), 1);
        QPainter painter(this);
        // painter.eraseRect(this->rect());
        painter.drawImage(0, 0, *BG);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor::fromHslF(*hue, *sat, *lit, 1));
        painter.setOpacity(trueop);
        painter.drawRect(this->rect());
        repainted = true;
    }
}
void ctl_trueop::setop(float iop)
{
    op = iop;
    repainted = false;
    this->update();
}
void ctl_trueop::setspc(float ispc)
{
    spc = ispc; // note that inside spacing is treated as square.
    repainted = false;
    this->update();
}

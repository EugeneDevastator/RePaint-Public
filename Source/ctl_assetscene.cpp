#include "ctl_assetscene.h"
#include <QDir>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QMouseEvent>
#include <qmath.h>

ctl_AssetScene::ctl_AssetScene(QWidget *parent) : QGraphicsScene(parent)
{
    //  GSC = new QGraphicsScene;
    //  this->setScene(GSC);

    this->clear();
    SideLen = 92;

    /*  QPixmap *framepic=new QPixmap(itemsize,itemsize);
      QPainter painter(framepic);
      painter.setBrush(QColor::fromRgbF(0,0,0,0));
      painter.setCompositionMode(QPainter::CompositionMode_Source);
      painter.drawRect(framepic->rect());
      painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painter.setBrush(Qt::red);
      painter.drawRect(5,5,30,30);
      */
    QPen *rpen = new QPen;
    rpen->setWidth(3);
    rpen->setColor(Qt::red);
    QBrush *rb = new QBrush;
    rb->setColor(QColor::fromRgbF(0, 0, 0, 0));
    frameitm = (QGraphicsItem *)this->addRect(0, 0, SideLen, SideLen, *rpen, *rb);
}
void ctl_AssetScene::InitByPath(QString itemspath)
{
    QDir LookDir;

    LookDir.setFilter(QDir::Files);
    LookDir.setSorting(QDir::Name);
    LookDir.setNameFilters(QStringList("*.png"));
    LookDir.setPath(itemspath);

    int i = 0;
    foreach (QString fname, LookDir.entryList())
    {
        QGraphicsPixmapItem *nitm = this->addPixmap(QPixmap(itemspath + fname));

        qreal maxdim = qMax(nitm->pixmap().width(), nitm->pixmap().height());
        qreal scale = SideLen / maxdim;
        nitm->setScale(scale);
        // nitm->setGraphicsEffect();
        nitm->setPos(0, i * SideLen);

        ItemList << nitm;

        i++;
        // this->setWindowTitle(itemspath+LookDir.entryList().at(1));
    }
}

void ctl_AssetScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    morigin = event->screenPos();
}

void ctl_AssetScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (Dist2D(event->screenPos(), morigin) < SideLen / 10)
    {
        int idx = floor(event->scenePos().y() / SideLen);
        ItemList[idx]->setSelected(true);
        frameitm->setPos(ItemList.at(idx)->pos());
        frameitm->setZValue(999);
    }
}
/*
void c_AssetItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
     QGraphicsPixmapItem::paint(painter,option,widget);
     painter->setBrush(Qt::red);
     painter->drawRect(widget->rect());
}
*/

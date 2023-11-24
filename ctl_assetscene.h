#ifndef CTL_ASSETSCENE_H
#define CTL_ASSETSCENE_H

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QDir>
#include <QMouseEvent>
#include "brushes.h"
//class c_AssetItem;

class ctl_AssetScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ctl_AssetScene(QWidget *parent = 0);
  //  QGraphicsScene *GSC;
    QList <QGraphicsItem *> ItemList;
    qint16 SideLen;
    QGraphicsItem * frameitm;
    QPoint morigin;
signals:
    
public slots:
    void InitByPath(QString itemspath);
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};
/*
class c_AssetItem: public QGraphicsPixmapItem
{
    Q_OBJECT
   public:
  c_AssetItem (QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    signals:
    public slots:
protected:
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );

}
*/
#endif // CTL_ASSETSCENE_H

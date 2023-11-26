#include "ctl_assetview.h"

ctl_assetview::ctl_assetview (QGraphicsScene * scene, QWidget * parent)
{
   // QGraphicsView::QGraphicsView(scene,parent);
   // GSC = new ctl_AssetScene;
this->setScene(scene);
   // this->scrollBarWidgets()
    this->setDragMode(QGraphicsView::ScrollHandDrag);
}
/*
void ctl_assetview::mouseMoveEvent(QMouseEvent *event){
  //  this->scrollContentsBy(90,90);
 //   this->sceneRect().setTop(this->sceneRect().top()+2);
    //(-origin.x()+event->pos().x(),-origin.y()+event->pos().y());
    origin=event->pos();
}
void ctl_assetview::mousePressEvent(QMouseEvent *event){
    origin=event->pos();
}*/

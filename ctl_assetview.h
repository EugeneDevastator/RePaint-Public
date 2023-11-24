#ifndef CTL_ASSETVIEW_H
#define CTL_ASSETVIEW_H

#include <QtWidgets/QGraphicsView>
#include <QMouseEvent>
//#include <ctl_assetscene.h>

class ctl_assetview : public QGraphicsView
{
    Q_OBJECT
public:

    explicit ctl_assetview (QGraphicsScene * scene, QWidget * parent = 0 );
 QPoint origin;
signals:
    
public slots:
protected:

   // void mouseMoveEvent(QMouseEvent *event);
  //  void mousePressEvent(QMouseEvent *event);

};

#endif // CTL_ASSETVIEW_H

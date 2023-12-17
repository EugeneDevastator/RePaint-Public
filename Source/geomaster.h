#ifndef GEOMASTER_H
#define GEOMASTER_H

#include <QtWidgets/QWidget>
#include "Brushes.h"
#include "BrushEngine/LegacySharedBrush.hpp"

struct fatpoint{
   BrushData props;
   QPointF pos;
};
/*
struct fatvector{

};
*/


class GeoMaster : public QWidget
{
    Q_OBJECT
public:
    explicit GeoMaster(QWidget *parent = 0);
   QList <fatpoint> lstFatPoints;
   QList < QVector<QPointF> > lstVects;

   QVector<QPointF> BufVec;

   QPolygonF IssuePoly();
   void AddVec(QVector<QPointF> vec);
   void AddPnt(QPointF pos);
   void NextVec();

signals:

    
public slots:
    
};

#endif // GEOMASTER_H

#include "geomaster.h"
#include "Brushes.h"

GeoMaster::GeoMaster(QWidget *parent) : QWidget(parent)
{
    lstVects.clear();
    lstFatPoints.clear();
    BufVec.clear();
}

void GeoMaster::AddVec(QVector<QPointF> vec)
{
    if (vec.count() > 1)
        lstVects.append(vec);
}

void GeoMaster::AddPnt(QPointF pos)
{
    BufVec.append(pos);
}

void GeoMaster::NextVec()
{
    if (lstVects.count() > 1)
    {
        lstVects.append(BufVec);
    }
    else
    {
        lstVects.insert(1, BufVec);
    }
    BufVec.clear();
}

QPolygonF GeoMaster::IssuePoly()
{

    for (int i = lstVects.count() - 1; i >= 0; i--)
    {
        if (lstVects[i].count() < 2)
            lstVects.removeAt(i);
    }

    QVector<QPointF> polyvec;
    int i = 0;
    polyvec << lstVects[i].first();
    QPointF lft = lstVects[i].last();
    lstVects[i].clear();
    lstVects[i].append(lft);

    for (i = 0; i < lstVects.count() - 1; i++)
    {
        // standard situation
        if (Dist2D(polyvec.last(), lstVects[i + 1].first()) < Dist2D(polyvec.last(), lstVects[i + 1].last()))
        {
            polyvec << lstVects[i + 1].first();
            QPointF lft = lstVects[i + 1].last();
            lstVects[i + 1].clear();
            lstVects[i + 1].append(lft);
        }
        else
        {
            polyvec << lstVects[i + 1].last();
            QPointF lft = lstVects[i + 1].first();
            lstVects[i + 1].clear();
            lstVects[i + 1].append(lft);
        }
    }

    // add last vector point;
    /*
        if (Dist2D(polyvec.last(),lstVects[i].first()) < Dist2D(polyvec.last(),lstVects[i].last())){
                polyvec << lstVects[i].last();
                    QPointF lft=lstVects[i].first();
                    lstVects[i].clear();
                    lstVects[i].append(lft);
                }
            else {
                polyvec << lstVects[i].first();
                    QPointF lft=lstVects[i].last();
                    lstVects[i].clear();
                    lstVects[i].append(lft);
                }*/
    //    polyvec << lstVects[i].first();
    for (i = lstVects.count() - 1; i >= 0; i--)
    {
        polyvec << lstVects[i].first();
    }
    /*
    // backward adding
    for(i=lstVects.count();i>=1;i--){
        if (Dist2D(polyvec.last(),lstVects[i-1].first()) > Dist2D(polyvec.last(),lstVects[i-1].last())){
            polyvec << lstVects[i-1].first();
        }
        else {
           polyvec << lstVects[i-1].last();
        }
    }

        //adding last polygon point
        if (Dist2D(polyvec.last(),lstVects[i].first()) < Dist2D(polyvec.last(),lstVects[i].last())){
            polyvec << lstVects[i].last();
            }
        else {
            polyvec << lstVects[i].first();
            }
*/

    BufVec.clear();
    lstVects.clear();
    QPolygonF poly(polyvec);
    return poly;
}

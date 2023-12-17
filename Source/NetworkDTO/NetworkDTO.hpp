//
// Created by Eugene on 12/17/2023.
//
#ifndef NETWORKDTO_HPP
#define NETWORKDTO_HPP
// Contents of NetworkDTO.hpp

struct PackedFloat {
   qint16 IntVal;
   quint8 FVal;

   void SetVal(double val) {
       IntVal = (qint16) (floor(val));
       FVal = (quint8) floor((val - floor(val)) * 255);
   }

   qreal GetVal() {
       return (qreal) (IntVal + FVal / 255.0);
   }
};

struct NetVector2f {
   PackedFloat xpos;
   PackedFloat ypos;

   void SetByQPointF(QPointF src) {
       xpos.SetVal(src.x());
       ypos.SetVal(src.y());
   }

   QPointF ToPointF() {
       QPointF res;
       res.setX(xpos.GetVal());
       res.setY(ypos.GetVal());
       return res;
   }

   qreal x() {
       return xpos.GetVal();
   }

   qreal y() {
       return ypos.GetVal();
   }
};


#endif

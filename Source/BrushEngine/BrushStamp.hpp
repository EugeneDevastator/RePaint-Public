//
// Created by Eugene on 12/17/2023.
//

#ifndef SOURCE_BRUSHSTAMP_HPP
#define SOURCE_BRUSHSTAMP_HPP

#include <QCoreApplication>
#include <QByteArray>
#include <qbytearray.h>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include "BrushEngine/NetBrushStamp.hpp"
#include "b_smartcolor.h"
#include <signal.h>
#include <QTabletEvent>
#include <QPainter>
#include <QtGui>
#include <QColor>
#include <QPoint>
#include <QImage>

struct ClientBrushStamp;
struct ClientBrushStamp {
   public:
      float rad_in;
      float rad_out;
      qreal opacity;
      double resangle;

      qreal crv;  // curvature
      //   quint16 resangle;
      qreal x2y;  //proportions [0-0.5]=[0-1] by x, [0.5-1]=[1-0] by y;
      qreal scale; //

      qreal cop;  // clone opacity
      qreal pwr;
      qreal sol;  // solidity // rename to dissolvance
      qreal sol2op; // solidity2opacity mod // rename to consistency.


      quint16 seed;

      quint16 noisex;  //internal
      quint16 noisey;  // internal


      quint8 NoiseID; // noise id;
      quint8 MaskID;
      quint8 pipeID;

      //  QPainter::CompositionMode compmode;
      quint8 bmidx; //blendmode index;
      quint8 noiseidx;
      quint8 preserveop; //preserve opacity
      QRgb col;
};
#endif //SOURCE_BRUSHSTAMP_HPP

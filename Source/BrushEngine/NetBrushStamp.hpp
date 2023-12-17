//
// Created by Eugene on 12/17/2023.
//

#ifndef SOURCE_NETBRUSHSTAMP_HPP
#define SOURCE_NETBRUSHSTAMP_HPP

#include <QtWidgets/QWidget>
#include <QMatrix>
#include <QObject>
#include <QtGlobal>
#include <math.h>
#include <QCoreApplication>
#include <QByteArray>
#include <qbytearray.h>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include "b_smartcolor.h"
#include "Brushes.h"
#include "BrushStamp.hpp"
#include <signal.h>
#include <QTabletEvent>
#include <QPainter>
#include <QtGui>
#include <QColor>
#include <QPoint>
#include <QImage>

struct NetBrushStamp;

struct NetBrushStamp {
   public:
      PackedFloat RadiusInner;
      PackedFloat RadiusOuter;
      quint8 FadeCurveExp;  // curvature
      quint16 resangle;
      quint8 ProportionsX2Y;  //proportions [0-0.5]=[0-1] by x, [0.5-1]=[1-0] by y;
      quint8 scale; //

      quint8 CloneOpacity;  // clone opacity
      quint8 pwr;
      quint8 Solidity;  // solidity // rename to dissolvance
      quint8 SolidityOfOpacity; // solidity2opacity mod // rename to consistency.


      quint16 seed;

      quint16 noisex;  //internal
      quint16 noisey;  // internal


      quint8 NoiseID; // noise id;
      quint8 MaskID;
      quint8 pipeID;

      //  QPainter::CompositionMode compmode;
      quint8 BlendMode; //blendmode index;
      quint8 noiseidx;
      quint8 preserveop; //preserve opacity
      QRgb col;

      QByteArray SerializeInto(QDataStream &stream);
      void Deserialize(QDataStream &stream);

      NetBrushStamp FromBrushStamp(const ClientBrushStamp &brushStamp);
      void ToBrushStamp(ClientBrushStamp &stamp);
};

QByteArray NetBrushStamp::SerializeInto(QDataStream &stream) {
    stream << RadiusInner.IntVal;
    stream << RadiusInner.FVal;
    stream << RadiusOuter.IntVal;
    stream << RadiusOuter.FVal;
    stream << BlendMode;
    stream << col;
    stream << CloneOpacity;
    stream << FadeCurveExp;
    stream << MaskID;
    stream << NoiseID;

    stream << noiseidx;
    stream << seed;

    stream << pipeID;
    stream << resangle;
    stream << scale;
    stream << seed;

    stream << Solidity;
    stream << SolidityOfOpacity;
    stream << ProportionsX2Y;
    stream << preserveop;
    stream << pwr;
}

void NetBrushStamp::Deserialize(QDataStream &stream) {
    stream >> RadiusInner.IntVal;
    stream >> RadiusInner.FVal;
    stream >> RadiusOuter.IntVal;
    stream >> RadiusOuter.FVal;
    stream >> BlendMode;
    stream >> col;
    stream >> CloneOpacity;
    stream >> FadeCurveExp;
    stream >> MaskID;
    stream >> NoiseID;

    stream >> noiseidx;
    stream >> seed;
    stream >> pipeID;
    stream >> resangle;
    stream >> scale;
    stream >> seed;
    stream >> Solidity;
    stream >> SolidityOfOpacity;
    stream >> ProportionsX2Y;
    stream >> preserveop;
    stream >> pwr;
}

NetBrushStamp NetBrushStamp::FromBrushStamp(const ClientBrushStamp &brushStamp) {
    RadiusInner.SetVal(brushStamp.rad_in);
    RadiusOuter.SetVal(brushStamp.rad_out);
    BlendMode = brushStamp.bmidx;
    //notice packed opacity is automatically stored in color and is packed in BControls
    col = brushStamp.col;
    CloneOpacity = brushStamp.cop * 255.0;
    FadeCurveExp = brushStamp.crv * 255.0;
    MaskID = brushStamp.MaskID;
    NoiseID = brushStamp.NoiseID;
    noisex = brushStamp.noisex;
    noisey = brushStamp.noisey;
    noiseidx = brushStamp.noiseidx;
    seed = brushStamp.seed;
    pipeID = brushStamp.pipeID;
    resangle = (brushStamp.resangle * 65535.0) / 360.0;
    scale = (brushStamp.scale * 51);
    seed = brushStamp.seed;
    Solidity = brushStamp.sol * 255.0;
    SolidityOfOpacity = brushStamp.sol2op * 255.0;
    ProportionsX2Y = brushStamp.x2y * 255.0;
    preserveop = brushStamp.preserveop;
    pwr = (brushStamp.pwr + 1) * 127;
    return NetBrushStamp();
}

void NetBrushStamp::ToBrushStamp(ClientBrushStamp &stamp) {
    stamp.rad_in = RadiusInner.GetVal();
    stamp.rad_out = RadiusOuter.GetVal();
    stamp.bmidx = BlendMode;
    stamp.col = col;
    stamp.opacity = qAlpha(col) / 255.0;
    stamp.cop = CloneOpacity / 255.0;
    stamp.crv = FadeCurveExp / 255.0;
    stamp.MaskID = MaskID;
    stamp.NoiseID = NoiseID;
    stamp.noisex = noisex;
    stamp.noisey = noisey;
    stamp.noiseidx = noiseidx;
    stamp.seed = seed;
    stamp.pipeID = pipeID;
    stamp.resangle = (resangle * 360.0) / 65535.0;
    stamp.scale = ((scale * 5.0) / 255.0);
    stamp.seed = seed;
    stamp.sol = Solidity / 255.0;
    stamp.sol2op = SolidityOfOpacity / 255.0;
    stamp.x2y = ProportionsX2Y / 255.0;
    stamp.preserveop = preserveop;
    stamp.pwr = (pwr / 127.0) - 1.0;
}

#endif //SOURCE_NETBRUSHSTAMP_HPP

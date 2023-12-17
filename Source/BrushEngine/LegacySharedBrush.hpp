//
// Created by Eugene on 12/17/2023.
//

#ifndef SOURCE_LEGACYSHAREDBRUSH_HPP
#define SOURCE_LEGACYSHAREDBRUSH_HPP

#include <QtWidgets/QWidget>
#include <QMatrix>
#include <QObject>
#include <QtGlobal>
#include <math.h>
#include "BrushEngine/BrushStamp.hpp"
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

struct BrushData;

struct BrushData {
      // struct NetBrushStamp;
      //   struct ClientBrushStamp;
   public:
      NetBrushStamp Pack;
      ClientBrushStamp ClientStamp;

      QByteArray Serialize();
      void Serialize(QDataStream *ba);
      void DeSerialize(QByteArray *src);
      void DeSerialize(QDataStream *ba);

      void SelfPack();
      void SelfUnpack();
      //QImage Mask;

};

QByteArray BrushData::Serialize() {

    QByteArray qba;
    QDataStream ba(&qba, QIODevice::ReadWrite);
    Serialize(&ba);
    return qba;
}

void BrushData::Serialize(QDataStream *ba) {
    SelfPack();
    *ba << Pack.RadiusInner.IntVal;
    *ba << Pack.RadiusInner.FVal;
    *ba << Pack.RadiusOuter.IntVal;
    *ba << Pack.RadiusOuter.FVal;
    *ba << Pack.BlendMode;
    *ba << Pack.col;
    *ba << Pack.CloneOpacity;
    *ba << Pack.FadeCurveExp;
    *ba << Pack.MaskID;
    *ba << Pack.NoiseID;

    *ba << Pack.noiseidx;
    *ba << Pack.seed;

    *ba << Pack.pipeID;
    *ba << Pack.resangle;
    *ba << Pack.scale;
    *ba << Pack.seed;

    *ba << Pack.Solidity;
    *ba << Pack.SolidityOfOpacity;
    *ba << Pack.ProportionsX2Y;
    *ba << Pack.preserveop;
    *ba << Pack.pwr;
}

void BrushData::DeSerialize(QByteArray *src) {
    QDataStream ba(src, QIODevice::ReadWrite);
    DeSerialize(&ba);
}

void BrushData::DeSerialize(QDataStream *ba) {
    // QDataStream ba(src,QIODevice::ReadWrite);
    *ba >> Pack.RadiusInner.IntVal;
    *ba >> Pack.RadiusInner.FVal;
    *ba >> Pack.RadiusOuter.IntVal;
    *ba >> Pack.RadiusOuter.FVal;
    *ba >> Pack.BlendMode;
    *ba >> Pack.col;
    *ba >> Pack.CloneOpacity;
    *ba >> Pack.FadeCurveExp;
    *ba >> Pack.MaskID;
    *ba >> Pack.NoiseID;

    *ba >> Pack.noiseidx;
    *ba >> Pack.seed;
    *ba >> Pack.pipeID;
    *ba >> Pack.resangle;
    *ba >> Pack.scale;
    *ba >> Pack.seed;
    *ba >> Pack.Solidity;
    *ba >> Pack.SolidityOfOpacity;
    *ba >> Pack.ProportionsX2Y;
    *ba >> Pack.preserveop;
    *ba >> Pack.pwr;
    SelfUnpack();
}

void BrushData::SelfPack() {
    Pack.RadiusInner.SetVal(ClientStamp.rad_in);
    Pack.RadiusOuter.SetVal(ClientStamp.rad_out);
    Pack.BlendMode = ClientStamp.bmidx;
    // notice packed opacity is automatically stored in color and is packed in BControls
    Pack.col = ClientStamp.col;
    Pack.CloneOpacity = ClientStamp.cop * 255.0;
    Pack.FadeCurveExp = ClientStamp.crv * 255.0;
    Pack.MaskID = ClientStamp.MaskID;
    Pack.NoiseID = ClientStamp.NoiseID;
    Pack.noisex = ClientStamp.noisex;
    Pack.noisey = ClientStamp.noisey;
    Pack.noiseidx = ClientStamp.noiseidx;
    Pack.seed = ClientStamp.seed;
    Pack.pipeID = ClientStamp.pipeID;
    Pack.resangle = (ClientStamp.resangle * 65535.0) / 360.0;
    Pack.scale = (ClientStamp.scale * 51);
    Pack.seed = ClientStamp.seed;
    Pack.Solidity = ClientStamp.sol * 255.0;
    Pack.SolidityOfOpacity = ClientStamp.sol2op * 255.0;
    Pack.ProportionsX2Y = ClientStamp.x2y * 255.0;
    Pack.preserveop = ClientStamp.preserveop;
    Pack.pwr = (ClientStamp.pwr + 1) * 127;
}

void BrushData::SelfUnpack() {
    ClientStamp.rad_in = Pack.RadiusInner.GetVal();
    ClientStamp.rad_out = Pack.RadiusOuter.GetVal();
    ClientStamp.bmidx = Pack.BlendMode;
    ClientStamp.col = Pack.col;
    ClientStamp.opacity = qAlpha(Pack.col) / 255.0;
    ClientStamp.cop = Pack.CloneOpacity / 255.0;
    ClientStamp.crv = Pack.FadeCurveExp / 255.0;
    ClientStamp.MaskID = Pack.MaskID;
    ClientStamp.NoiseID = Pack.NoiseID;
    ClientStamp.noisex = Pack.noisex;
    ClientStamp.noisey = Pack.noisey;
    ClientStamp.noiseidx = Pack.noiseidx;
    ClientStamp.seed = Pack.seed;
    ClientStamp.pipeID = Pack.pipeID;
    ClientStamp.resangle = (Pack.resangle * 360.0) / 65535.0;
    ClientStamp.scale = ((Pack.scale * 5.0) / 255.0);
    ClientStamp.seed = Pack.seed;
    ClientStamp.sol = Pack.Solidity / 255.0;
    ClientStamp.sol2op = Pack.SolidityOfOpacity / 255.0;
    ClientStamp.x2y = Pack.ProportionsX2Y / 255.0;
    ClientStamp.preserveop = Pack.preserveop;
    ClientStamp.pwr = (Pack.pwr / 127.0) - 1.0;
}

#endif //SOURCE_LEGACYSHAREDBRUSH_HPP

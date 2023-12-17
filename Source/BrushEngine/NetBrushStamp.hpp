//
// Created by Eugene on 12/17/2023.
//
#ifndef NETBRUSHSTAMP_HPP
#define NETBRUSHSTAMP_HPP

#include <QtGlobal>
#include "NetworkDTO/NetworkDTO.hpp"

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

  // CompositionMode compmode;
      quint8 BlendMode; //blendmode index;
      quint8 noiseidx;
      quint8 preserveop; //preserve opacity
      QRgb col;

void  SerializeInto(QDataStream &stream) {
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

void  Deserialize(QDataStream &stream) {
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
};
#endif
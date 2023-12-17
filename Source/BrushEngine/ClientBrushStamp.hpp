//
// Created by Eugene on 12/17/2023.
//
#ifndef ClientBrushStamp_HPP
#define ClientBrushStamp_HPP

#include "NetBrushStamp.hpp"

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

      void ToNetBrush(NetBrushStamp &netBrushStamp) {

          netBrushStamp.RadiusInner.SetVal(rad_in);
          netBrushStamp.RadiusOuter.SetVal(rad_out);
          netBrushStamp.BlendMode = bmidx;
          //notice packed opacity is automatically stored in color and is packed in BControls
          netBrushStamp.col =col;
          netBrushStamp.CloneOpacity =cop * 255.0;
          netBrushStamp.FadeCurveExp =crv * 255.0;
          netBrushStamp.MaskID =MaskID;
          netBrushStamp.NoiseID =NoiseID;
          netBrushStamp.noisex =noisex;
          netBrushStamp.noisey =noisey;
          netBrushStamp.noiseidx =noiseidx;
          netBrushStamp.seed =seed;
          netBrushStamp.pipeID =pipeID;
          netBrushStamp.resangle = (resangle * 65535.0) / 360.0;
          netBrushStamp.scale = (scale * 51);
          netBrushStamp.seed =seed;
          netBrushStamp.Solidity =sol * 255.0;
          netBrushStamp.SolidityOfOpacity =sol2op * 255.0;
          netBrushStamp.ProportionsX2Y =x2y * 255.0;
          netBrushStamp.preserveop =preserveop;
          netBrushStamp.pwr = (pwr + 1) * 127;
      }

      void FromNetBrushStamp(NetBrushStamp &netStamp) {
          rad_in = netStamp.RadiusInner.GetVal();
          rad_out = netStamp.RadiusOuter.GetVal();
          bmidx = netStamp.BlendMode;
          col = netStamp.col;
          opacity = qAlpha(netStamp.col) / 255.0;
          cop = netStamp.CloneOpacity / 255.0;
          crv = netStamp.FadeCurveExp / 255.0;
          MaskID = netStamp.MaskID;
          NoiseID = netStamp.NoiseID;
          noisex = netStamp.noisex;
          noisey = netStamp.noisey;
          noiseidx = netStamp.noiseidx;
          seed = netStamp.seed;
          pipeID = netStamp.pipeID;
          resangle = (netStamp.resangle * 360.0) / 65535.0;
          scale = ((netStamp.scale * 5.0) / 255.0);
          seed = netStamp.seed;
          sol = netStamp.Solidity / 255.0;
          sol2op = netStamp.SolidityOfOpacity / 255.0;
          x2y = netStamp.ProportionsX2Y / 255.0;
          preserveop = netStamp.preserveop;
          pwr = (netStamp.pwr / 127.0) - 1.0;
      }
};

#endif

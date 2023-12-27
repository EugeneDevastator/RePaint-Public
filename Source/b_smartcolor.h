#ifndef B_SMARTCOLOR_H
#define B_SMARTCOLOR_H

#include <QObject>
#include <QColor>
#include "BrushEngine/DialModel.hpp"

class b_SmartColor : public QObject {
   Q_OBJECT
   public:
      explicit b_SmartColor(DialModel *hueDial, DialModel *satDial, DialModel *litDial, QObject *parent = 0);
      QColor UseCol;
      float savehue;
      float savesat;
      float savelit;
   private:
      DialModel *SatDial;
      DialModel *HueDial;
      DialModel *LitDial;
   public slots:
      void RefreshColor();
      void SetHueF(float inp);
      void SetSatF(float inp);
      void SetLitF(float inp);
      void LoadAll();
      //void SaveAll();
      void SetCol(QColor col);
   protected:
   signals:
      void NewColor(QColor col);


};

#endif // B_SMARTCOLOR_H

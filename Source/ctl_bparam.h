#ifndef CTL_BPARAM_H
#define CTL_BPARAM_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QMouseEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
#include <QFile>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QPainter>
#include <QList>
#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <QPicture>
#include <QString>
#include <QStringList>
#include <QtGui>
#include <QtCore>

#include "bctl_dblslider.h"
#include "Brushes.h"
#include "BrushEngine/DialModel.hpp"

class BrushDialWidget : public QWidget {
   Q_OBJECT

   public:
      explicit BrushDialWidget(DialModel *model = nullptr, QWidget *parent = 0);

      float GetMaxValue();
      qreal GetModValue(d_StrokePars stpars);
      //int OutMode;
      //int IRange;
      //float MaxBound;
      //float MinBound;
      //float DefaultValue;
      //float *linkedval;
      //float PenMul;

      QBoxLayout *MLayout;
      //int PenMode;
      //int PenState; // state off, direct, inverse
      QList<QIcon> PenIcons;
      DialSliderWidget *Slider;
      QLabel *LbIcon;
      QStringList IcNames;
      QStringList Names;

      QToolButton *BtnPenMode;
      QMenu *MnuPen;
      QLineEdit *ValueLabel;
      QSlider *oldSlider;
      QPushButton *btn;

      DialModel *Model;
   public slots:
      void SetIcon(QString pathstr);
      void Refresh();
   private slots:
      void SetPenMode(QAction *mAct);

      void SliderChange(float val);
      void HandleModelUpdate();
   protected:
      void mousePressEvent(QMouseEvent *event);
};

#endif // CTL_BPARAM_H


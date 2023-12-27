//
// Created by Eugene on 12/25/2023.
//

#ifndef SOURCE_DIALMODEL_HPP
#define SOURCE_DIALMODEL_HPP

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

class DialModel : public QObject {

   Q_OBJECT
   private:
      float MaxBound=255;
      float MinBound=0;

   public:

      int OutMode=1;

      float DefaultValue=128;
      float DefaultMaxCursorNorm=1;
      float PenMul=1;

      float RuntimeCursorNorm=0.5;
      float MaxCursorNorm=1;
      float MinCursorNorm=0;
      float Jitter=0;

      int PenMode=0;
      int PenState=0; // state off, direct, inverse

      explicit DialModel(float minOutput = 0, float maxOutput = 1, float defaultValue = 0.5, QObject *parent = nullptr)
              : QObject(parent) {
          MaxBound = maxOutput;
          MinBound = minOutput;
          DefaultValue = defaultValue;
          MinCursorNorm=0;
          Jitter = 0;

          DefaultMaxCursorNorm = DefaultValue / GetBoundRange();
          RuntimeCursorNorm = DefaultMaxCursorNorm;
          MaxCursorNorm = DefaultMaxCursorNorm;
      }

      float GetBoundRange() {
          return MaxBound - MinBound;
      }
      void ResetValue(){
          MinCursorNorm=0;
          MaxCursorNorm=DefaultMaxCursorNorm;
          RuntimeCursorNorm = DefaultMaxCursorNorm;
          emit ChangedSignal();
      }
      float GetValueInRange(float t) {
          float cursorRange = MaxCursorNorm - MinCursorNorm;
          float boundT = cursorRange * t + MinCursorNorm;
          return boundT * GetBoundRange() + MinBound;
      }

      float GetCurrentValue() {
          float cursorRange = MaxCursorNorm - MinCursorNorm;
          float boundT = cursorRange * RuntimeCursorNorm + MinCursorNorm;
          return boundT * GetBoundRange() + MinBound;
      }

      float GetValueAtMax() {
        return MinBound + MaxCursorNorm * GetBoundRange();
      }

      float GetCurrentMin() {
          return MinBound * GetBoundRange() + MinCursorNorm;
      }

      void SetPenModeNum(int mode) {
          PenMode = mode;
          PenState = 1;
          emit ChangedSignal();
      }

      void SetOutMode(int outMode) {
          OutMode = outMode;
          emit ChangedSignal();
      }

      void Reset(){
          MinCursorNorm=0;
          MaxCursorNorm=DefaultMaxCursorNorm;
          Jitter=0;
          emit ChangedSignal();
      }

      void SetCursors(float minNormal, float maxNormal, float jitter) {
          MaxCursorNorm = maxNormal;
          MinCursorNorm = minNormal;
          Jitter = jitter;
          emit ChangedSignal();
      }

      void SetPenState(int lerpState) {
          PenState = lerpState;
          emit ChangedSignal();
      }

      void SetPenInv() {
          PenState++;
          if (PenState == 2)
              PenState = 0;
          emit ChangedSignal();
      }

   signals:
      void ChangedSignal();

   public slots:
      void SetMaxCursor(float maxNormal) {
          MaxCursorNorm = maxNormal;
          emit ChangedSignal();
      }
};

#endif //SOURCE_DIALMODEL_HPP

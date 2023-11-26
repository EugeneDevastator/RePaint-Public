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

class Ctl_BParam : public QWidget
{
    Q_OBJECT
public:
    explicit Ctl_BParam(QWidget *parent = 0);


    float GetValue();
    qreal GetModValue(d_StrokePars stpars);
    int OutMode;
    int IRange;
    float OutMax;
    float OutMin;
    float OutDef;
    float *linkedval;
    float PenMul;

    QBoxLayout *MLayout;
    int PenMode;
    int PenState; // state off, direct, inverse
    QList <QIcon> PenIcons;
    bctl_DblSlider *Gslider;
    QLabel *LbIcon;
    QStringList IcNames;
    QStringList Names;

    QToolButton *BtnPenMode;
    QMenu *MnuPen;
    QLineEdit *edline;
    QSlider *slider;
    QPushButton *btn;

signals:
       void SendPenMode(int mode);
public slots:
    void SetPenMode(QAction *mAct);
    void SetPenModeNum(int mode);
    void SetPenInv();
    void SetRange(int x);
    void SetMode(int x);

    void SetValF(float y);
    void Ed2Slide();
    void Slide2Ed();
    void ResetValue();
    void SetIcon(QString pathstr);
    void ApplyValue(float newval);
    void SliderChange(float val);
    void FlipOrder();

private:


protected:
    void mousePressEvent(QMouseEvent *event);
signals:
    void NewValue(float maxf);
};

#endif // CTL_BPARAM_H


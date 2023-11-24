#ifndef DLG_NEWCANVAS_H
#define DLG_NEWCANVAS_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include "imagearray.h"

class dlg_NewCanvas : public QDialog
{
    Q_OBJECT
public:
    explicit dlg_NewCanvas(ImageArray *lnk, QWidget *parent = 0);
    ImageArray *LinkedImage;
    QPushButton *BtnNew;
    QPushButton *BtnCancel;
    QPushButton *BtnRot;
    QToolButton *BtnLock;

    QPushButton *BtnReframe;
    QPushButton *BtnResize;

    QPushButton *BtnPickSize;
    QPushButton *BtnPickProps;

    QLineEdit *EdWidth;
    QLineEdit *EdHeight;
    QLineEdit *EdRatioW;
    QLineEdit *EdRatioH;
    QComboBox *CbPresets;
    QGridLayout *LT;


signals:
    void SendSize(QSize sz);
    void SendReframe(QSize sz, QPoint pos);
    void SendRescale(QSize sz, int stretch);

public slots:
    void NewClick();
    void RescaleClick();
    void RecalcW();
    void RecalcH();
    void Rot90();
        void SetPresetRes(int);

    void PickSize();
    void PickProps();

};

#endif // DLG_NEWCANVAS_H

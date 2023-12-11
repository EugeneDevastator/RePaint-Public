#ifndef PNL_BCONTORLS_H
#define PNL_BCONTORLS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QList>
#include <QtWidgets/QCheckBox>

#include "bctl_widgetholder.h"
#include "Brushes.h"
#include "ctl_bparam.h"
#include "bctl_dblslider.h"
#include "ctl_brushsizer.h"
#include "ctl_toolselector.h"
#include "b_smartcolor.h"
#include "ctl_maskselect.h"
//#include "ctl_usertools.h"
#include "ctl_friendlist.h"
#include "ctl_BMselector.h"
#include "ArtMaster.h"


class pnl_bcontorls : public bctl_WidgetHolder
{
    Q_OBJECT
public:
  //  ArtMaster *ARTM;
    ctl_friendlist *Flist;

    explicit pnl_bcontorls(d_RealBrush *GG_brush , QWidget *parent= 0);
    d_RealBrush *g_Brush;
    b_SmartColor *g_PaintColor;
    b_SmartColor *g_EraseColor;
    QList <QString> LCMNames;
    QList <QPainter::CompositionMode> LCMidx;
    QPushButton *BtnMasks;
    QPushButton *BtnNoises;
    ctl_MaskSelect *MaskSelWidget;
  //  ctl_UserTools *ToolSelWidget;
    BrushSizer *BrushControl;
    //QComboBox *CtlCompMode;
    ctl_BMselector *CtlCompMode;
    QComboBox *CtlNoiseMode;
    ctl_ToolSelector *CtlTools;
    QCheckBox *chkFastMode;
     //QSizeGrip SzGrip;
    Ctl_BParam *CtlLen;
    Ctl_BParam *CtlPwr;
    Ctl_BParam *CtlRad;

    Ctl_BParam *CtlRadRel;

    Ctl_BParam *CtlScaleRel;
    Ctl_BParam *CtlScale;
    Ctl_BParam *CtlAng;
    Ctl_BParam *CtlSpc; // spacing
    Ctl_BParam *CtlSpcJit;
    Ctl_BParam *Ctl;
    Ctl_BParam *CtlOp;
    Ctl_BParam *CtlCop;
    Ctl_BParam *CtlSol2;
    Ctl_BParam *CtlSol;
    Ctl_BParam *CtlCrv;

    Ctl_BParam *CtlHue;
    Ctl_BParam *CtlSat;
    Ctl_BParam *CtlLit;

    QList <Ctl_BParam*> Controllers;

    QVBoxLayout *LtLeftToolbar;

    BrushData ParseBrush(BrushData BRSH);
    ActionData ParseBrush(d_Stroke Strk, d_StrokePars stpars);


signals:
    void TransferPreset(d_ToolPreset prs);
    void SendAction(ActionData act);
 public slots:
    void BrushRequest();
    void SendPreset();
    void SetRadRel(float rel);
    void ApplyPreset(d_ToolPreset Preset);
    d_ToolPreset GenPreset();
};

#endif // PNL_BCONTORLS_H

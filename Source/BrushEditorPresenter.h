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
#include "BrushEngine/ClientBrushStamp.hpp"
#include "BrushEngine/LegacySharedBrush.hpp"
// BrushEditorModel.h


class BrushEditorPresenter : public bctl_WidgetHolder
{
    Q_OBJECT

   private:
      ClientBrushStamp *InternalBrush;
public:
  //  ArtMaster *ARTM;
    ctl_friendlist *Flist;

    explicit BrushEditorPresenter(ClientBrushStamp *mainBrush , QWidget *parent= 0);

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
    BrushDialWidget *CtlLen;
    BrushDialWidget *CtlPwr;
    BrushDialWidget *CtlRad;

    BrushDialWidget *CtlRadRel;

    BrushDialWidget *CtlScaleRel;
    BrushDialWidget *CtlScale;
    BrushDialWidget *CtlAng;
    BrushDialWidget *CtlSpc; // spacing
    BrushDialWidget *CtlScatter;
    BrushDialWidget *Ctl;
    BrushDialWidget *CtlOp;
    BrushDialWidget *CtlCop;
    BrushDialWidget *CtlSol2;
    BrushDialWidget *CtlSol;
    BrushDialWidget *CtlCrv;

    BrushDialWidget *CtlHue;
    BrushDialWidget *CtlSat;
    BrushDialWidget *CtlLit;

    QList <BrushDialWidget*> Controllers;

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

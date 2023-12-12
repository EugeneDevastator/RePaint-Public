#ifndef PNL_QUICKCONTROL_H
#define PNL_QUICKCONTROL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>

#include "ctl_bparam.h"
#include "ctl_fastbrush.h"
#include "BrushEditorPresenter.h"
#include "ctl_trueop.h"
#include "ctl_toolselector.h"
#include "ctl_BMselector.h"
#include "ctl_fastbrush.h"
#include "imagearray.h"


class pnl_QuickControl : public QWidget
     {
        Q_OBJECT

public:
    explicit pnl_QuickControl(BrushEditorPresenter *BCTLS, ClientBrush *gbrush, ImageArray *mimage);

    Ctl_BParam *QuickOp;
    Ctl_BParam *QuickSol;
    Ctl_BParam *QuickSop;
    Ctl_BParam *QuickCop;
    Ctl_BParam *QuickPow;

    Ctl_BParam *QuickHue;
    Ctl_BParam *QuickSat;
    Ctl_BParam *QuickLit;
    Ctl_BParam *QuickScat;
    Ctl_BParam *QuickSpc;
    Ctl_BParam *QuickLen;

    Ctl_BParam *QuickSize;
    Ctl_BParam *QuickScale;
    Ctl_BParam *QuickRel;
    Ctl_BParam *QuickRot;

    QWidget *FastPanelLeft;
    QWidget *FastPanelRight;
    QWidget *FastPanelGeo;
    QWidget *FastBlends;

    ctl_trueop *FastTrueOp;
    ctl_ToolSelector *FastTools;
    ctl_BMselector *QBmodes;

    QComboBox *QNmodes;
    QListView *qPresets;

    QHBoxLayout *Lfpl;
    QHBoxLayout *Lfpr;
    QVBoxLayout *Lfpg;
    QGridLayout *LTblends;

    ctl_FastBrush *FastBrush;

    ImageArray *MImage;


public slots:

   void linkSliders(Ctl_BParam *slave, Ctl_BParam *master);
   void RealignPanel();
   void appear();
   void vanish();
   void UpdateBG();
protected:
   void enterEvent(QEvent *event);

  };

#endif // PNL_QUICKCONTROL_H

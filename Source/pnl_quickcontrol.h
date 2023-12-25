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
#include "BrushEngine/ClientBrushStamp.hpp"

class pnl_QuickControl : public QWidget
     {
        Q_OBJECT

public:
    explicit pnl_QuickControl(BrushEditorPresenter *BCTLS, ClientBrushStamp *mainBrush, ImageArray *mimage);
    BrushDialWidget *QuickOp;
    BrushDialWidget *QuickSol;
    BrushDialWidget *QuickSop;
    BrushDialWidget *QuickCop;
    BrushDialWidget *QuickPow;

    BrushDialWidget *QuickHue;
    BrushDialWidget *QuickSat;
    BrushDialWidget *QuickLit;
    BrushDialWidget *QuickScat;
    BrushDialWidget *QuickSpc;
    BrushDialWidget *QuickLen;

    BrushDialWidget *QuickSize;
    BrushDialWidget *QuickScale;
    BrushDialWidget *QuickRel;
    BrushDialWidget *QuickRot;

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

   void RealignPanel();
   void appear();
   void vanish();
   void UpdateBG();
protected:
   void enterEvent(QEvent *event);

  };

#endif // PNL_QUICKCONTROL_H

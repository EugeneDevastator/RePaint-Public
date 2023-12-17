//
// Created by Eugene on 12/14/2023.
//

#ifndef SOURCE_BRUSHMODEL_H
#define SOURCE_BRUSHMODEL_H
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
#include "BrushEngine/BrushStamp.hpp"
#include "BrushEngine/LegacySharedBrush.hpp"
// BrushEditorModel.h

class BrushModel {
   public:
      BrushModel();
      BrushData ParseBrush(BrushData BRSH);
      ActionData ParseBrush(d_Stroke Strk, d_StrokePars stpars);
      d_ToolPreset GenPreset();

   private:
      ClientBrushStamp *g_Brush;
      // Add other brush-related properties here
};

#endif //SOURCE_BRUSHMODEL_H

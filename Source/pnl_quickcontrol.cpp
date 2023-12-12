#include "pnl_quickcontrol.h"

void pnl_QuickControl::linkSliders(Ctl_BParam *slave, Ctl_BParam *master)
{

    slave->MLayout->setDirection(QBoxLayout::BottomToTop);
    slave->Gslider->orient = 1;

    connect(slave->Gslider, SIGNAL(AllValChange(float, float, float)), master->Gslider, SLOT(GetVals(float, float, float)));
    connect(slave, SIGNAL(SendPenMode(int)), master, SLOT(SetPenModeNum(int)));
    connect(master->Gslider, SIGNAL(AllValChange(float, float, float)), slave->Gslider, SLOT(GetVals(float, float, float)));
    connect(master, SIGNAL(SendPenMode(int)), slave, SLOT(SetPenModeNum(int)));
    slave->Gslider->GetVals(master->Gslider->clipmaxF, master->Gslider->clipminF, master->Gslider->jitter);
    slave->Gslider->grad = master->Gslider->grad;
    slave->LbIcon->setPixmap(*master->LbIcon->pixmap());
    slave->MLayout->setMargin(1);
}

pnl_QuickControl::pnl_QuickControl(BrushEditorPresenter *BCTLS, ClientBrush *gbrush, ImageArray *mimage)
{

    QFile logfile("d:/mhplog.log");
    logfile.open(QFile::Append);

    MImage = mimage;
    this->setParent(MImage);
    QuickOp = new Ctl_BParam;
    QuickSol = new Ctl_BParam;
    QuickSop = new Ctl_BParam;
    QuickCop = new Ctl_BParam;
    QuickPow = new Ctl_BParam;

    FastBrush = new ctl_FastBrush;
    // FastBrush->show();
    FastBrush->setParent(this);
    // FastBrush->setWindowFlags(Qt::Tool|Qt::CustomizeWindowHint|Qt::WindowMinimizeButtonHint);
    FastBrush->g_Brush = gbrush;
    FastBrush->ZoomK = &MImage->ZoomK;
    connect(FastBrush, SIGNAL(SendRel(float)), BCTLS->CtlRadRel, SLOT(SetValF(float)));
    connect(FastBrush, SIGNAL(SendCrv(float)), BCTLS->CtlCrv, SLOT(SetValF(float)));
    connect(FastBrush, SIGNAL(AskBrush()), BCTLS, SLOT(BrushRequest()));
    connect(BCTLS, SIGNAL(SendAction(d_Action)), FastBrush, SLOT(ReBrush(d_Action)));
    FastBrush->setAttribute(Qt::WA_NoMousePropagation);

    // connect(FastBrush,SIGNAL(SendCrv(float)),BControls->QuickCrv,SLOT(SetValF(float)));

    logfile.write("\n fastpanel - part1 done");
    logfile.close();
    logfile.open(QFile::Append);

    Lfpl = new QHBoxLayout;
    Lfpr = new QHBoxLayout;

    linkSliders(QuickOp, BCTLS->CtlOp);
    linkSliders(QuickSol, BCTLS->CtlSol);
    linkSliders(QuickSop, BCTLS->CtlSol2);
    linkSliders(QuickCop, BCTLS->CtlCop);
    linkSliders(QuickPow, BCTLS->CtlPwr);

    // creating right panel
    logfile.write("\n fastpanel - part2 done");
    logfile.close();
    logfile.open(QFile::Append);
    QuickHue = new Ctl_BParam;
    QuickSat = new Ctl_BParam;
    QuickLit = new Ctl_BParam;
    QuickScat = new Ctl_BParam;
    QuickSpc = new Ctl_BParam;
    QuickLen = new Ctl_BParam;
    logfile.write("\n fastpanel - part2-1 done");
    QuickHue->Gslider->DsRange = 0.0833333;
    linkSliders(QuickHue, BCTLS->CtlHue);
    linkSliders(QuickSat, BCTLS->CtlSat);
    linkSliders(QuickLit, BCTLS->CtlLit);
    linkSliders(QuickScat, BCTLS->CtlSpcJit);
    linkSliders(QuickSpc, BCTLS->CtlSpc);
    linkSliders(QuickLen, BCTLS->CtlLen);

    connect(BCTLS->CtlLit->Gslider, SIGNAL(Repainted()), QuickLit->Gslider, SLOT(Redraw()));
    connect(BCTLS->CtlSat->Gslider, SIGNAL(Repainted()), QuickSat->Gslider, SLOT(Redraw()));
    connect(BCTLS->CtlHue->Gslider, SIGNAL(Repainted()), QuickHue->Gslider, SLOT(Redraw()));

    Lfpr->addWidget(QuickHue);
    Lfpr->addWidget(QuickSat);
    Lfpr->addWidget(QuickLit);
    // Lfpr->addWidget(QuickScat);
    // Lfpr->addWidget(QuickSpc);
    // Lfpr->addWidget(QuickLen);
    logfile.write("\n fastpanel - part3 done");
    logfile.close();
    logfile.open(QFile::Append);

    FastPanelRight = new QWidget;
    FastPanelRight->setLayout(Lfpr);
    FastPanelRight->setParent(this);
    // FastPanelRight->setWindowFlags(Qt::Tool|Qt::CustomizeWindowHint|Qt::WindowMinimizeButtonHint);
    FastPanelRight->setFixedWidth((64 + 8) * 3);
    FastPanelRight->setFixedHeight(256 + 256);

    QuickHue->Gslider->MaxCol = QColor::fromRgbF(1, 1, 1, 0);
    QuickLit->Gslider->MaxCol = QColor::fromRgbF(1, 1, 1, 0);
    QuickSat->Gslider->MaxCol = QColor::fromRgbF(1, 1, 1, 0);
    QuickHue->Gslider->MinCol = QColor::fromRgbF(0, 0, 0, 0.5);
    QuickLit->Gslider->MinCol = QColor::fromRgbF(0, 0, 0, 0.5);
    QuickSat->Gslider->MinCol = QColor::fromRgbF(0, 0, 0, 0.5);

    // upleft panel;

    // Lfpl->addWidget(QuickPow);
    // Lfpl->addWidget(QuickCop);
    // Lfpl->addWidget(QuickSop);
    Lfpl->addWidget(QuickSpc);
    Lfpl->addWidget(QuickSol);
    Lfpl->addWidget(QuickOp);
    // Lfpl->setMargin(0);
    FastPanelLeft = new QWidget;
    // FastPanelLeft->hide();

    FastPanelLeft->setLayout(Lfpl);
    FastPanelLeft->setParent(this);
    FastPanelLeft->setFixedWidth(FastPanelRight->width());
    FastPanelLeft->setFixedHeight(FastPanelRight->height());
    //
    QuickSize = new Ctl_BParam;
    QuickScale = new Ctl_BParam;
    QuickRel = new Ctl_BParam;
    QuickRot = new Ctl_BParam;

    logfile.write("\n fastpanel - part4 done");
    logfile.close();
    logfile.open(QFile::Append);
    linkSliders(QuickSize, BCTLS->CtlRad);
    linkSliders(QuickScale, BCTLS->CtlScale);
    linkSliders(QuickRel, BCTLS->CtlScaleRel);
    linkSliders(QuickRot, BCTLS->CtlAng);

    FastPanelGeo = new QWidget;

    Lfpg = new QVBoxLayout;
    Lfpg->addWidget(QuickSize);
    Lfpg->addWidget(QuickScale);
    Lfpg->addWidget(QuickRel);
    Lfpg->addWidget(QuickRot);
    Lfpg->setMargin(0);
    logfile.write("\n fastpanel - part5 done");
    logfile.close();
    logfile.open(QFile::Append);
    QuickSize->MLayout->setDirection(QBoxLayout::LeftToRight);
    QuickSize->Gslider->orient = 0;
    QuickScale->MLayout->setDirection(QBoxLayout::LeftToRight);
    QuickScale->Gslider->orient = 0;
    QuickRel->MLayout->setDirection(QBoxLayout::LeftToRight);
    QuickRel->Gslider->orient = 0;
    QuickRot->MLayout->setDirection(QBoxLayout::LeftToRight);
    QuickRot->Gslider->orient = 0;

    FastPanelGeo->setLayout(Lfpg);
    // FastPanelGeo->setint(this);
    FastPanelGeo->setFixedWidth(256);
    FastPanelGeo->setFixedHeight(128);

    FastPanelLeft->setAttribute(Qt::WA_NoMousePropagation);

    FastPanelRight->setAttribute(Qt::WA_NoMousePropagation);
    FastPanelGeo->setAttribute(Qt::WA_NoMousePropagation);

    logfile.write("\n fastpanel - part6 done");
    logfile.close();
    logfile.open(QFile::Append);

    FastTrueOp = new ctl_trueop();
    // FastTrueOp->rrel=&(BControls->CtlRadRel->Gslider->clipmaxF);
    // FastTrueOp->FadeCurveExp=&(BControls->CtlCrv->Gslider->clipmaxF);

    FastTrueOp->setParent(this);

    connect(BCTLS->CtlOp, SIGNAL(NewValue(float)), FastTrueOp, SLOT(setop(float)));
    connect(BCTLS->CtlSpc, SIGNAL(NewValue(float)), FastTrueOp, SLOT(setspc(float)));
    connect(QuickOp, SIGNAL(NewValue(float)), FastTrueOp, SLOT(setop(float)));
    connect(QuickSpc, SIGNAL(NewValue(float)), FastTrueOp, SLOT(setspc(float)));

    FastTrueOp->hue = &(BCTLS->CtlHue->Gslider->clipmaxF);
    FastTrueOp->sat = &(BCTLS->CtlSat->Gslider->clipmaxF);
    FastTrueOp->lit = &(BCTLS->CtlLit->Gslider->clipmaxF);

    FastTools = new ctl_ToolSelector;
    FastTools->setParent(this);
    FastTools->setAttribute(Qt::WA_NoMousePropagation);
    FastTools->setAutoFillBackground(true);
    connect(FastTools, SIGNAL(ToolSelected(int)), BCTLS->CtlTools, SLOT(AssignTool(int)));
    connect(BCTLS->CtlTools, SIGNAL(ToolSelected(int)), FastTools, SLOT(AssignTool(int)));

    // for (int i=0;i<eSTOP;i++){
    // FastTools->Btns[i]=BControls->CtlTools->Btns[i];
    // connect (FastTools->Btns[i],SIGNAL(clicked()),BControls->CtlTools->Btns[i],SLOT(click())); //good
    //   connect (BControls->CtlTools->Btns[i],SIGNAL(clicked()),FastTools,SLOT(Release()));
    //     connect (BControls->CtlTools->Btns[i],SIGNAL(clicked()),FastTools->Btns[i],SLOT(lower()));
    //}

    // FastTools->hide();

    FastBlends = new QWidget;
    /*
    QList <QToolButton *> QBMBtns;

    */
    /*
    for (int i=0;i<7;i++){
        QToolButton *nb=new QToolButton;
        QBMBtns.append(nb);
        nb->setFixedSize(24,24);
        LTblends->addWidget(nb,0,i);
        }
    */

    QBmodes = new ctl_BMselector;
    QNmodes = new QComboBox;
    QBmodes->setAttribute(Qt::WA_NoMousePropagation);
    QNmodes->setAttribute(Qt::WA_NoMousePropagation);
    QNmodes->setMaxVisibleItems(999);

    for (int i = 0; i < BCTLS->CtlNoiseMode->count(); i++)
        QNmodes->addItem(BCTLS->CtlNoiseMode->itemText(i));

    connect(QBmodes, SIGNAL(BMchanged(int)), BCTLS->CtlCompMode, SLOT(setBMbyint(int)));
    connect(BCTLS->CtlCompMode, SIGNAL(BMchanged(int)), QBmodes, SLOT(setBMbyint(int)));
    connect(QNmodes, SIGNAL(activated(int)), BCTLS->CtlNoiseMode, SLOT(setCurrentIndex(int)));
    connect(BCTLS->CtlNoiseMode, SIGNAL(activated(int)), QNmodes, SLOT(setCurrentIndex(int)));

    /*t
    qPresets=new QListView();
    qPresets->setModel(BCTLS->ToolSelWidget->itModel);
    qPresets->setSelectionModel(BCTLS->ToolSelWidget->selModel);
    connect(qPresets,SIGNAL(clicked(QModelIndex)),BCTLS->ToolSelWidget,SLOT(ClickTool(QModelIndex)));
    */
    LTblends = new QGridLayout();
    LTblends->addWidget(QBmodes, 0, 0);
    LTblends->addWidget(QNmodes, 0, 1);
    // t LTblends->addWidget(qPresets,1,0,1,2);

    FastBlends->setParent(this);
    FastBlends->setLayout(LTblends);
    // FastBlends->hide();
}

void pnl_QuickControl::RealignPanel()
{
    this->move(QPoint(0, 0));
    this->resize(MImage->size());
    int x;
    int y;
    y = MImage->height() * 0.5 - FastPanelLeft->height() * 0.5;
    x = MImage->width() * 0.5 - 128 - FastPanelLeft->width();
    FastPanelLeft->move(x, y);

    y = MImage->height() * 0.5 - FastPanelRight->height() * 0.5;
    x = MImage->width() * 0.5 + 128; //-FastPanelRight->width();
    FastPanelRight->move(x, y);

    y = FastPanelLeft->y() - FastPanelGeo->height();
    x = FastPanelLeft->x() + FastPanelLeft->width() - FastPanelGeo->width(); //-FastPanelRight->width();
    FastPanelGeo->move(x, y);

    x = -FastBrush->width() * 0.5 + MImage->width() * 0.5;
    y = -FastBrush->height() * 0.5 + MImage->height() * 0.5;
    FastBrush->move(x, y);

    x = MImage->width() * 0.5 - FastTrueOp->width() * 0.5;
    y = FastBrush->y() - FastTrueOp->height() * 1.5;
    FastTrueOp->move(x, y);

    x = MImage->width() * 0.5 - FastTools->width() * 0.5;
    y = FastTrueOp->y() - FastTools->height() - FastTrueOp->height() * 0.5;
    FastTools->move(x, y);

    x = FastPanelLeft->x();
    y = FastPanelLeft->y() + FastPanelLeft->height() + 16;
    FastBlends->move(x, y);
}
void pnl_QuickControl::UpdateBG()
{
    qint16 x = -256 + MImage->width() * 0.5;
    qint16 y = -256 + MImage->height() * 0.5;

    *FastBrush->BG = MImage->GetCollapsedRect(QRect(x, y, 512, 512));
    *FastTrueOp->BG = MImage->GetCollapsedRect(FastTrueOp->geometry());
}

void pnl_QuickControl::appear()
{
    FastBlends->show();
    FastBrush->show();
    // FastPanelGeo->show();
    FastPanelLeft->show();
    FastPanelRight->show();
    FastTools->show();
    FastTrueOp->show();
    UpdateBG();
}
void pnl_QuickControl::vanish()
{
    FastBlends->hide();
    FastBrush->hide();
    // FastPanelGeo->hide();
    FastPanelLeft->hide();
    FastPanelRight->hide();
    FastTools->hide();
    FastTrueOp->hide();
}
void pnl_QuickControl::enterEvent(QEvent *event)
{
    ((QWidget *)this->parent())->activateWindow();
}

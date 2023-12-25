#include "pnl_quickcontrol.h"

void pnl_QuickControl::SetUpSliderFromMain(BrushDialWidget *slave, BrushDialWidget *master)
{

    slave->MLayout->setDirection(QBoxLayout::BottomToTop);
    slave->Slider->orient = 1;
    slave->Slider->grad = master->Slider->grad;
    slave->LbIcon->setPixmap(*master->LbIcon->pixmap());
    slave->MLayout->setMargin(1);
}

pnl_QuickControl::pnl_QuickControl(BrushEditorPresenter *BCTLS, ClientBrushStamp *mainBrush, ImageArray *mimage)
 :  MImage(mimage)
{
  //  Check for null dependencies and throw an exception if any are null
 if (!BCTLS || !mainBrush || !mimage) {
     throw std::invalid_argument("One or more dependencies are null");
 }
    QFile logfile("d:/mhplog.log");
    logfile.open(QFile::Append);
    this->setParent(MImage);
    // Rest of your initialization code that depends on MImage

    QuickOp = new BrushDialWidget(BCTLS->CtlOp->Model,this);
    QuickSol = new BrushDialWidget(BCTLS->CtlSol->Model,this);
    QuickSpc = new BrushDialWidget(BCTLS->CtlSpc->Model,this);
    QuickHue = new BrushDialWidget(BCTLS->CtlHue->Model,this);
    QuickSat = new BrushDialWidget(BCTLS->CtlSat->Model,this);
    QuickLit = new BrushDialWidget(BCTLS->CtlLit->Model,this);

    SetUpSliderFromMain(QuickOp,BCTLS->CtlOp);
    SetUpSliderFromMain(QuickSol,BCTLS->CtlSol);
    SetUpSliderFromMain(QuickSpc,BCTLS->CtlSpc);
    SetUpSliderFromMain(QuickHue,BCTLS->CtlHue);
    SetUpSliderFromMain(QuickSat,BCTLS->CtlSat);
    SetUpSliderFromMain(QuickLit,BCTLS->CtlLit);

    QuickSop = new BrushDialWidget(BCTLS->CtlSol2->Model,this);
    QuickCop = new BrushDialWidget(BCTLS->CtlCop->Model,this);
    QuickPow = new BrushDialWidget(BCTLS->CtlPwr->Model,this);

    FastBrush = new ctl_FastBrush(mainBrush,this);
    FastBrush->show();
    // FastBrush->setWindowFlags(Qt::Tool|Qt::CustomizeWindowHint|Qt::WindowMinimizeButtonHint);
    FastBrush->ZoomK = &MImage->ZoomK;
    FastBrush->setAttribute(Qt::WA_NoMousePropagation);

    // connect(FastBrush,SIGNAL(SendCrv(float)),BControls->QuickCrv,SLOT(SetMaxCursor(float)));

    logfile.write("\n fastpanel - part1 done");
    logfile.close();
    logfile.open(QFile::Append);

    Lfpl = new QHBoxLayout;
    Lfpr = new QHBoxLayout;

    // creating right panel
    logfile.write("\n fastpanel - part2 done");
    logfile.close();
    logfile.open(QFile::Append);

    QuickScat = new BrushDialWidget(BCTLS->CtlScatter->Model);

    QuickLen = new BrushDialWidget(BCTLS->CtlLen->Model);
    logfile.write("\n fastpanel - part2-1 done");
    QuickHue->Slider->DsRange = 0.0833333;

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

    QuickHue->Slider->MaxCol = QColor::fromRgbF(1, 1, 1, 0);
    QuickLit->Slider->MaxCol = QColor::fromRgbF(1, 1, 1, 0);
    QuickSat->Slider->MaxCol = QColor::fromRgbF(1, 1, 1, 0);
    QuickHue->Slider->MinCol = QColor::fromRgbF(0, 0, 0, 0.5);
    QuickLit->Slider->MinCol = QColor::fromRgbF(0, 0, 0, 0.5);
    QuickSat->Slider->MinCol = QColor::fromRgbF(0, 0, 0, 0.5);

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
    QuickSize = new BrushDialWidget(BCTLS->CtlRad->Model,0);
    QuickScale = new BrushDialWidget(BCTLS->CtlScale->Model,0);
    QuickRel = new BrushDialWidget(BCTLS->CtlScaleRel->Model,0);
    QuickRot = new BrushDialWidget(BCTLS->CtlAng->Model,0);

    logfile.write("\n fastpanel - part4 done");
    logfile.close();
    logfile.open(QFile::Append);

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
    QuickSize->Slider->orient = 0;
    QuickScale->MLayout->setDirection(QBoxLayout::LeftToRight);
    QuickScale->Slider->orient = 0;
    QuickRel->MLayout->setDirection(QBoxLayout::LeftToRight);
    QuickRel->Slider->orient = 0;
    QuickRot->MLayout->setDirection(QBoxLayout::LeftToRight);
    QuickRot->Slider->orient = 0;

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
    // FastTrueOp->rrel=&(BControls->CtlRadRel->Slider->clipmaxF);
    // FastTrueOp->FadeCurveExp=&(BControls->CtlCrv->Slider->clipmaxF);

    FastTrueOp->setParent(this);

    FastTrueOp->hue = &(BCTLS->CtlHue->Slider->clipmaxF);
    FastTrueOp->sat = &(BCTLS->CtlSat->Slider->clipmaxF);
    FastTrueOp->lit = &(BCTLS->CtlLit->Slider->clipmaxF);

    FastTools = new ctl_ToolSelector;
    FastTools->setParent(this);
    FastTools->setAttribute(Qt::WA_NoMousePropagation);
    FastTools->setAutoFillBackground(true);


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

   //connect(BCTLS->CtlOp->Model, SIGNAL(ChangedSignal()), FastTrueOp, SLOT(setop(1)));
   //connect(BCTLS->CtlSpc->Model, SIGNAL(ChangedSignal()), FastTrueOp, SLOT(setspc(1)));
   // connect(QuickOp, SIGNAL(NewValue(float)), FastTrueOp, SLOT(setop(float)));
   // connect(QuickSpc, SIGNAL(NewValue(float)), FastTrueOp, SLOT(setspc(float)));

    connect(FastBrush, SIGNAL(SendRel(float)), BCTLS->CtlRadRel->Model, SLOT(SetMaxCursor(float)));
    connect(FastBrush, SIGNAL(SendCrv(float)), BCTLS->CtlCrv->Model, SLOT(SetMaxCursor(float)));
    connect(FastBrush, SIGNAL(AskBrush()), BCTLS, SLOT(BrushRequest()));
    connect(BCTLS, SIGNAL(SendAction(d_Action)), FastBrush, SLOT(ReBrush(d_Action)));

    connect(FastTools, SIGNAL(ToolSelected(int)), BCTLS->CtlTools, SLOT(AssignTool(int)));
    connect(BCTLS->CtlTools, SIGNAL(ToolSelected(int)), FastTools, SLOT(AssignTool(int)));
    connect(QBmodes, SIGNAL(BMchanged(int)), BCTLS->CtlCompMode, SLOT(setBMbyint(int)));
    connect(BCTLS->CtlCompMode, SIGNAL(BMchanged(int)), QBmodes, SLOT(setBMbyint(int)));
    connect(QNmodes, SIGNAL(activated(int)), BCTLS->CtlNoiseMode, SLOT(setCurrentIndex(int)));
    connect(BCTLS->CtlNoiseMode, SIGNAL(activated(int)), QNmodes, SLOT(setCurrentIndex(int)));
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
    if (!MImage || !FastBrush || !FastTrueOp) {
        return; // Safeguard against null pointers
    }

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

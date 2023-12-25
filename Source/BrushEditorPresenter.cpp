#include "BrushEditorPresenter.h"
#include "ctl_bparam.h"
#include "BrushEngine/ClientBrushStamp.hpp"
#include "BrushEngine/LegacySharedBrush.hpp"

BrushEditorPresenter::BrushEditorPresenter(ClientBrushStamp *mainBrush, QWidget *parent)
{

    setAccessibleName("BrushControls");
    setWindowTitle("Brush DNA");
    this->Title->setText("Brush DNA");
    this->setParent(parent);
    QFile logfile("d:/mhplog.log");
    logfile.open(QFile::Append);

    logfile.write("\n BCONTROLS init BEGIN");
    logfile.close();
    logfile.open(QFile::Append);
    qDebug() << ("bcontrols init begin");

    QString AppPath = RESPATH;
    CtlTools = new ctl_ToolSelector();
    InternalBrush = mainBrush;
   //BrushControl = new BrushSizer();

    logfile.write("\n BCONTROLS brushsizer+");
    logfile.close();
    logfile.open(QFile::Append);
    qDebug() << ("bcontrols brushsizer done");

    //   BrushControl->Brush=mainBrush;
    BtnMasks = new QPushButton("Presets");
    MaskSelWidget = new ctl_MaskSelect();
    chkFastMode = new QCheckBox();
    chkFastMode->setText("Simple Mode");
    qDebug() << ("bcontrols gbrush done");
    //ToolSelWidget=new ctl_UserTools(parent);
    //t  ToolSelWidget->setModal(true);
    qDebug() << ("bcontrols toolsel done");

    CtlCompMode = new ctl_BMselector();

    qDebug() << ("bcontrols components part 1 done");

//    CtlCompMode->setMaxVisibleItems(30);
    LCMNames.append("Normal");
    LCMidx.append(QPainter::CompositionMode_SourceOver);
    LCMNames.append("Erase");
    LCMidx.append(QPainter::CompositionMode_DestinationOut);
    LCMNames.append("Plus");
    LCMidx.append(QPainter::CompositionMode_Plus);
    LCMNames.append("ColorDodge");
    LCMidx.append(QPainter::CompositionMode_ColorDodge);
    LCMNames.append("Screen");
    LCMidx.append(QPainter::CompositionMode_Screen);
    LCMNames.append("Lighten");
    LCMidx.append(QPainter::CompositionMode_Lighten);
    LCMNames.append("ColorBurn");
    LCMidx.append(QPainter::CompositionMode_ColorBurn);
    LCMNames.append("Multiply");
    LCMidx.append(QPainter::CompositionMode_Multiply);
    LCMNames.append("Darken");
    LCMidx.append(QPainter::CompositionMode_Darken);
    LCMNames.append("Overlay");
    LCMidx.append(QPainter::CompositionMode_Overlay);
    LCMNames.append("HardLight");
    LCMidx.append(QPainter::CompositionMode_HardLight);
    LCMNames.append("SoftLight");
    LCMidx.append(QPainter::CompositionMode_SoftLight);
    LCMNames.append("Xor");
    LCMidx.append(QPainter::CompositionMode_Xor);
    LCMNames.append("Difference");
    LCMidx.append(QPainter::CompositionMode_Difference);
    LCMNames.append("Exclusion");
    LCMidx.append(QPainter::CompositionMode_Exclusion);

    qDebug() << ("bcontrols part 1-2 done");

/*
    for (int i=0;i<LCMidx.count();i++){
        CtlCompMode->addItem(LCMNames.at(i));
    }
*/
    /*
    for (int i=0;i<ARTM->BMnames.count();i++){
        CtlCompMode->addItem(ARTM->BMnames.at(i));
    }*/

    CtlNoiseMode = new QComboBox;
    CtlNoiseMode->addItem("Random");
    CtlNoiseMode->addItem("Constant");
    CtlNoiseMode->addItem("Stencil");

    qDebug() << ("bcontrols part 1-3 done");

    CtlRad = new BrushDialWidget();
    CtlRad->setToolTip("Radius control");

    CtlRadRel = new BrushDialWidget(new DialModel(0,0.9899,0.5));
    CtlRadRel->setToolTip("Radius control");
    qDebug() << ("bcontrols part 1-4 done");

    CtlAng = new BrushDialWidget(new DialModel(0,360,360));
    CtlAng->setToolTip("Angle control");

    // CtlAng->MLayout->setDirection(QBoxLayout::TopToBottom);
    CtlScaleRel = new BrushDialWidget(new DialModel(0,1,0.5));
    CtlScaleRel->setToolTip("Spacing");

    CtlScale = new BrushDialWidget(new DialModel(0.1,5,1));
    CtlScale->setToolTip("Scale");

    CtlScale->Slider->DsRange = 0.2;

    CtlScale->Model->ResetValue();

    CtlSpc = new BrushDialWidget(new DialModel(0.05,2,0.3));
    CtlSpc->setToolTip("Spacing");

    CtlSpcJit = new BrushDialWidget(new DialModel(0,5,0));
    CtlSpcJit->setToolTip("Scattering");

    CtlOp = new BrushDialWidget;
    CtlOp->setToolTip("Opacity");

    CtlCop = new BrushDialWidget(new DialModel(0.8,1,1));
    CtlCop->setToolTip("Clone Opacity");

    CtlLen = new BrushDialWidget(new DialModel(0,1024,256));
    CtlLen->setToolTip("Brush Length");

    CtlPwr = new BrushDialWidget(new DialModel(-1,1,0.5));
    CtlPwr->setToolTip("Tool Power");

    qDebug() << ("bcontrols part 1-5 done");

//CtlRadRel->Slider->grad=CtlPwr->Slider->grad;
//CtlCrv->Slider->grad=CtlPwr->Slider->grad;


    CtlSol = new BrushDialWidget(new DialModel(0,1,0));
    CtlSol->setToolTip("Solidity");

    CtlSol2 = new BrushDialWidget(new DialModel(0,1,0));
    CtlSol2->setToolTip("Binarity");

    CtlCrv = new BrushDialWidget(new DialModel(-1,1,0));
    CtlCrv->setToolTip("Curvature");

    CtlHue = new BrushDialWidget(new DialModel(0,1,0.45));
    CtlHue->setToolTip("Hue");
    CtlHue->Slider->DsRange = 0.0833333;

    CtlSat = new BrushDialWidget((new DialModel(0,1,1)));
    CtlSat->setToolTip("Saturation");
    CtlLit = new BrushDialWidget((new DialModel(0,1,0)));
    CtlLit->setToolTip("Lightness");

    CtlRad->SetIcon(AppPath + "/res/ctlRad.png");
    CtlRadRel->SetIcon(AppPath + "/res/ctlrrel.png");
    CtlScaleRel->SetIcon(AppPath + "/res/ctlScaleRel.png");
    CtlScale->SetIcon(AppPath + "/res/ctlScale.png");
    CtlAng->SetIcon(AppPath + "/res/ctlAng.png");
    CtlSpcJit->SetIcon(AppPath + "/res/ctlSpcJit.png");
    CtlSpc->SetIcon(AppPath + "/res/ctlSpc.png");
    CtlOp->SetIcon(AppPath + "/res/ctlop.png");
    CtlCop->SetIcon(AppPath + "/res/ctlcop.png");
    CtlSol->SetIcon(AppPath + "/res/ctlsol.png");
    CtlSol2->SetIcon(AppPath + "/res/ctlsol2op.png");
    CtlCrv->SetIcon(AppPath + "/res/ctlcrv.png");
    CtlPwr->SetIcon(AppPath + "/res/ctlpwr.png");
    CtlLen->SetIcon(AppPath + "/res/ctllen.png");
    CtlHue->SetIcon(AppPath + "/res/ctlhue.png");
    CtlSat->SetIcon(AppPath + "/res/ctlsat.png");
    CtlLit->SetIcon(AppPath + "/res/ctllit.png");
    CtlLit->Model->SetMaxCursor(0.5);

    float step = RngConv(M_PI / 6, 0, M_PI, 0, 1);
    CtlHue->Slider->grad->setColorAt(0, Qt::red);
    CtlHue->Slider->grad->setColorAt(0 + step, Qt::yellow);
    CtlHue->Slider->grad->setColorAt(0 + step * 2, Qt::green);
    CtlHue->Slider->grad->setColorAt(0 + step * 3, Qt::cyan);
    CtlHue->Slider->grad->setColorAt(0 + step * 4, Qt::blue);
    CtlHue->Slider->grad->setColorAt(0 + step * 5, QColor::fromRgbF(1, 0, 1, 1));
    CtlHue->Slider->grad->setColorAt(1, Qt::red);
    //CtlHue->Slider->orient=1;
    CtlSat->Slider->grad->setColorAt(0, QColor::fromHslF(1, 0, 0.5));
    CtlSat->Slider->grad->setColorAt(1, QColor::fromHslF(1, 1, 0.5));

    CtlLit->Slider->grad->setColorAt(0, QColor::fromHslF(0, 0, 0));
    CtlLit->Slider->grad->setColorAt(0.5, QColor::fromHslF(1, 1, 0.5));
    CtlLit->Slider->grad->setColorAt(1, QColor::fromHslF(0, 0, 1));



    LtLeftToolbar = new QVBoxLayout(this);
//    LtLeftToolbar->setMargin(0);
    //   LtLeftToolbar->setSpacing(0);

    //  LtLeftToolbar->addWidget(CtlTools);
    CtlTools->setFixedHeight(92);
    CtlTools->setFixedWidth(128);
    LtLeftToolbar->addWidget(chkFastMode);

    // LtLeftToolbar->addWidget(BrushControl);
    LtLeftToolbar->addWidget(BtnMasks);
    LtLeftToolbar->addWidget(CtlRad);
    LtLeftToolbar->addWidget(CtlRadRel);
    LtLeftToolbar->addWidget(CtlScale);
    LtLeftToolbar->addWidget(CtlScaleRel);
    LtLeftToolbar->addWidget(CtlAng);
    LtLeftToolbar->addWidget(CtlSpc);
    LtLeftToolbar->addWidget(CtlSpcJit);
    LtLeftToolbar->addWidget(CtlCompMode);
    LtLeftToolbar->addWidget(CtlNoiseMode);
    LtLeftToolbar->addWidget(CtlOp);
    LtLeftToolbar->addWidget(CtlSol);

    LtLeftToolbar->addWidget(CtlSol2);
    LtLeftToolbar->addWidget(CtlCrv);
    LtLeftToolbar->addWidget(CtlCop);
    LtLeftToolbar->addWidget(CtlPwr);
    LtLeftToolbar->addWidget(CtlHue);
    LtLeftToolbar->addWidget(CtlSat);
    LtLeftToolbar->addWidget(CtlLit);
    LtLeftToolbar->addWidget(CtlLen);


    //  LtLeftToolbar->addStretch(1);
    //   LtLeftToolbar->setMargin(0);
    LtLeftToolbar->setSpacing(0);
    LtLeftToolbar->setContentsMargins(QMargins());
    //  BrushControl->setFixedWidth(128);
    //  BrushControl->setFixedHeight(128);
    // LtLeftToolbar->addWidget(new QSizeGrip(this), 0, Qt::AlignBottom | Qt::AlignRight);

    setLayout2(LtLeftToolbar);

    InternalBrush->rad_out = 15;
    InternalBrush->rad_in = 5;

    //this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    logfile.write("\n BCONTROLS components inited+");
    logfile.close();
    logfile.open(QFile::Append);


//    connect (BrushControl,SIGNAL(SendRadRel(float)),this,SLOT(SetRadRel(float)));

    //t   connect (BtnMasks,SIGNAL(clicked()),ToolSelWidget,SLOT(show()));
    //t  connect (BtnMasks,SIGNAL(clicked()),this,SLOT(SendPreset()));
    //t   connect (this,SIGNAL(TransferPreset(d_ToolPreset)),ToolSelWidget,SLOT(GetPreset(d_ToolPreset)));
//t   connect (ToolSelWidget,SIGNAL(SetPreset(d_ToolPreset)),this,SLOT(ApplyPreset(d_ToolPreset)));

    Controllers.append(CtlRad);
    Controllers.append(CtlRadRel);

    Controllers.append(CtlScaleRel);
    Controllers.append(CtlScale);
    Controllers.append(CtlAng);
    Controllers.append(CtlSpc); // spacing
    Controllers.append(CtlSpcJit);
    Controllers.append(CtlOp);
    Controllers.append(CtlCop);
    Controllers.append(CtlSol2);
    Controllers.append(CtlSol);
    Controllers.append(CtlCrv);
    Controllers.append(CtlLen);
    Controllers.append(CtlPwr);

    Controllers.append(CtlHue);
    Controllers.append(CtlSat);
    Controllers.append(CtlLit);

    foreach (BrushDialWidget *ct, Controllers) connect(ct->Slider, SIGNAL(Repainted()), this, SLOT(BrushRequest()));
/*
    CtlPwr->Slider->grad->setColorAt(0,QColor::fromHslF(0,0,1));
    CtlPwr->Slider->grad->setColorAt(0.5,QColor::fromHslF(0,0,0));
    CtlPwr->Slider->grad->setColorAt(1,QColor::fromHslF(0,0,1));
    CtlScaleRel->Slider->grad->setColorAt(0,QColor::fromHslF(0,0,1));
    CtlScaleRel->Slider->grad->setColorAt(0.5,QColor::fromHslF(0,0,0));
    CtlScaleRel->Slider->grad->setColorAt(1,QColor::fromHslF(0,0,1));
*/
    logfile.write("\n BCONTROLS INIT+++");
    logfile.close();
    logfile.open(QFile::Append);
}


void BrushEditorPresenter::SetRadRel(float rel) {
    CtlRadRel->Model->SetMaxCursor(rel);
}

ActionData BrushEditorPresenter::ParseBrush(d_Stroke Strk, d_StrokePars stpars) {

    BrushData nb;
    ActionData act;

    nb.ClientStamp.opacity = 0.8;
    act.Brush = nb;
    qreal op = CtlOp->GetModValue(stpars);
    act.Brush.ClientStamp.opacity = op;
    act.Brush.ClientStamp.opacity = CtlOp->GetModValue(stpars);
    act.Brush.ClientStamp.rad_out = InternalBrush->rad_out * CtlRad->GetModValue(stpars);//*stpars.pressure;
    qreal minrad = 3.0;
    if (act.Brush.ClientStamp.rad_out < minrad) {
        act.Brush.ClientStamp.opacity *= (act.Brush.ClientStamp.rad_out) / minrad;
        act.Brush.ClientStamp.rad_out = (minrad + act.Brush.ClientStamp.rad_out) * 0.5;
    }
    act.Brush.ClientStamp.rad_in = act.Brush.ClientStamp.rad_out * CtlRadRel->GetModValue(
            stpars);//Brush->rad_in*CtlRad->GetModValue(stpars);//*stpars.pressure;


    act.Brush.ClientStamp.resangle = InternalBrush->resangle + CtlAng->GetModValue(stpars);
    if ((act.Brush.ClientStamp.resangle == 45) | (act.Brush.ClientStamp.resangle == -45) | (act.Brush.ClientStamp.resangle == 135) |
        (act.Brush.ClientStamp.resangle == 225) | (act.Brush.ClientStamp.resangle == 315))
        act.Brush.ClientStamp.resangle = act.Brush.ClientStamp.resangle + 0.00001;

    QColor DrawCol = QColor::fromHslF(CtlHue->GetModValue(stpars),
                                      CtlSat->GetModValue(stpars),
                                      CtlLit->GetModValue(stpars)
    );

    act.Brush.ClientStamp.col = qRgba(DrawCol.red(), DrawCol.green(), DrawCol.blue(), round(act.Brush.ClientStamp.opacity * 255));

//if (stpars.Pars[csERASER]==1) act.Brush.col = EraserColor->UseCol;

    act.Brush.ClientStamp.crv = CtlCrv->GetModValue(stpars);
    act.Brush.ClientStamp.cop = CtlCop->GetModValue(stpars);
    act.Brush.ClientStamp.sol = CtlSol->GetModValue(stpars);
    act.Brush.ClientStamp.sol2op = CtlSol2->GetModValue(stpars);
    act.Brush.ClientStamp.x2y = CtlScaleRel->GetModValue(stpars);
    act.Brush.ClientStamp.scale = CtlScale->GetModValue(stpars);
    act.Brush.ClientStamp.scale = CtlScale->GetModValue(stpars);
//act.Brush.compmode= //ARTM->BMmodes.at(CtlCompMode->currentIndex());
    act.Brush.ClientStamp.bmidx = CtlCompMode->GetIdx();
    act.Brush.ClientStamp.noiseidx = CtlNoiseMode->currentIndex();
    if (CtlNoiseMode->currentIndex() == 0) //random
    {
        act.Brush.ClientStamp.seed = RNDf(65535);
        act.Brush.ClientStamp.noisex = RawRnd(act.Brush.ClientStamp.seed, 1024);
        act.Brush.ClientStamp.noisey = RawRnd(act.Brush.ClientStamp.seed + 21, 1024);
        //  act.Brush.noisex=(qrand()/(float)RAND_MAX)*(1024);
        //  act.Brush.noisey=(qrand()/(float)RAND_MAX)*(1024);

    } else if (CtlNoiseMode->currentIndex() == 1) { //constant
        act.Brush.ClientStamp.seed = 45698;
        //act.Brush.noisex=34;
        //act.Brush.noisey=76;
    } else if (CtlNoiseMode->currentIndex() == 2) { //stencil
        //act.Brush.noisex=Strk.pos1.x();
        //act.Brush.noisey=Strk.pos1.y();
    }
    act.Noisemode = CtlNoiseMode->currentIndex();
    act.Brush.ClientStamp.noisex -= 1024 * floor(act.Brush.ClientStamp.noisex / 1024);
    act.Brush.ClientStamp.noisey -= 1024 * floor(act.Brush.ClientStamp.noisey / 1024);
//if (act.Brush.noisex<0) act.Brush.noisex-=1024*ceil(act.Brush.noisex/1024);
//if (act.Brush.noisey<0) act.Brush.noisey-=1024*ceil(act.Brush.noisey/1024);
    if (act.Brush.ClientStamp.noisex < act.Brush.ClientStamp.rad_out) act.Brush.ClientStamp.noisex += 1024;
    if (act.Brush.ClientStamp.noisey < act.Brush.ClientStamp.rad_out) act.Brush.ClientStamp.noisey += 1024;
    act.Brush.ClientStamp.pipeID = plCFNSR;
    if (chkFastMode->isChecked()) act.Brush.ClientStamp.pipeID = plRS;

    act.ToolID = CtlTools->ToolID;
    act.Brush.ClientStamp.pwr = CtlPwr->GetModValue(stpars);
    act.Stroke = Strk;
    if (stpars.Pars[csERASER] == 1) {
        act.Brush.ClientStamp.bmidx = 1;
    }

    //act.Brush.SelfPack();
    // thats quite strange, might be needed.

    return act;


}

void BrushEditorPresenter::ApplyPreset(d_ToolPreset Preset) {
    int cshift = 0;
    if (Preset.usecolor == true) cshift = 3;
    CtlCompMode->setBMbyint(Preset.compmode);
    CtlCompMode->SendChangedIdx(Preset.compmode);

    for (int i = 0; i < Controllers.count() - cshift; i++) { //min and max are for all but colors if needed.
        Controllers[i]->Slider->clipmaxF = Preset.pars[i].fmax;
        Controllers[i]->Slider->clipminF = Preset.pars[i].fmin;
    }
    for (int i = 0; i < Controllers.count(); i++) { //jitter is saved for everyone
        Controllers[i]->Slider->jitter = Preset.pars[i].fjit;
        //Controllers[i]->SetPenModeNum(Preset.pars[i].tmode);
    }
    CtlTools->Btns[Preset.toolid]->click();

            foreach (BrushDialWidget *ctl, Controllers) {
            ctl->Slider->change();
            ctl->Slider->Redraw();
        }

}

d_ToolPreset BrushEditorPresenter::GenPreset() {
    d_ToolPreset Preset;

    Preset.compmode = CtlCompMode->GetIdx();
    Preset.toolid = CtlTools->ToolID;
    for (int i = 0; i < Controllers.count(); i++) { //min and max are for all but colors if needed.
        Preset.pars[i].fmax = Controllers[i]->Slider->clipmaxF;
        Preset.pars[i].fmin = Controllers[i]->Slider->clipminF;
    }
    for (int i = 0; i < Controllers.count(); i++) { //jitter is saved for everyone
        Preset.pars[i].fjit = Controllers[i]->Slider->jitter;
        Preset.pars[i].tmode = Controllers[i]->Model->PenMode;

    }
    return Preset;
}

void BrushEditorPresenter::SendPreset() {
    emit TransferPreset(GenPreset());

}

void BrushEditorPresenter::BrushRequest() {
    d_Stroke strk1;
    d_StrokePars stp1;
    for (int i = 0; i < csSTOP; i++) stp1.Pars[i] = 1;
    emit SendAction(ParseBrush(strk1, stp1));
}

#include "pnl_bcontorls.h"
#include "ctl_bparam.h"

pnl_bcontorls::pnl_bcontorls(d_RealBrush *GG_brush,QWidget *parent)
{

    this->Title->setText("Brush DNA");
    this->setParent(parent);
    QFile logfile("d:/mhplog.log");
    logfile.open(QFile::Append);

    logfile.write("\n BCONTROLS init BEGIN");  logfile.close(); logfile.open(QFile::Append);
    qDebug()<<("bcontrols init begin");

QString AppPath=RESPATH;
CtlTools=new ctl_ToolSelector();
g_Brush = new d_RealBrush;
//BrushControl = new BrushSizer();

logfile.write("\n BCONTROLS brushsizer+");  logfile.close(); logfile.open(QFile::Append);
qDebug()<<("bcontrols brushsizer done");

 //   BrushControl->g_Brush=GG_brush;
    BtnMasks=new QPushButton("Presets");
    MaskSelWidget=new ctl_MaskSelect();
    chkFastMode=new QCheckBox();
    chkFastMode->setText("Simple Mode");
 qDebug()<<("bcontrols gbrush done");
    //ToolSelWidget=new ctl_UserTools(parent);
    //t  ToolSelWidget->setModal(true);
 qDebug()<<("bcontrols toolsel done");


    CtlCompMode=new ctl_BMselector();

    qDebug()<<("bcontrols components part 1 done");

//    CtlCompMode->setMaxVisibleItems(30);
    LCMNames.append("Normal");         LCMidx.append(QPainter::CompositionMode_SourceOver);
    LCMNames.append("Erase");          LCMidx.append(QPainter::CompositionMode_DestinationOut);
    LCMNames.append("Plus");           LCMidx.append(QPainter::CompositionMode_Plus);
    LCMNames.append("ColorDodge");     LCMidx.append(QPainter::CompositionMode_ColorDodge);
    LCMNames.append("Screen");         LCMidx.append(QPainter::CompositionMode_Screen);
    LCMNames.append("Lighten");        LCMidx.append(QPainter::CompositionMode_Lighten);
    LCMNames.append("ColorBurn");      LCMidx.append(QPainter::CompositionMode_ColorBurn);
    LCMNames.append("Multiply");       LCMidx.append(QPainter::CompositionMode_Multiply);
    LCMNames.append("Darken");         LCMidx.append(QPainter::CompositionMode_Darken);
    LCMNames.append("Overlay");        LCMidx.append(QPainter::CompositionMode_Overlay);
    LCMNames.append("HardLight");      LCMidx.append(QPainter::CompositionMode_HardLight);
    LCMNames.append("SoftLight");      LCMidx.append(QPainter::CompositionMode_SoftLight);
    LCMNames.append("Xor");            LCMidx.append(QPainter::CompositionMode_Xor);
    LCMNames.append("Difference");     LCMidx.append(QPainter::CompositionMode_Difference);
    LCMNames.append("Exclusion");      LCMidx.append(QPainter::CompositionMode_Exclusion);

    qDebug()<<("bcontrols part 1-2 done");

/*
    for (int i=0;i<LCMidx.count();i++){
        CtlCompMode->addItem(LCMNames.at(i));
    }
*/
    /*
    for (int i=0;i<ARTM->BMnames.count();i++){
        CtlCompMode->addItem(ARTM->BMnames.at(i));
    }*/

    CtlNoiseMode=new QComboBox;
    CtlNoiseMode->addItem("Random");
    CtlNoiseMode->addItem("Constant");
    CtlNoiseMode->addItem("Stencil");

        qDebug()<<("bcontrols part 1-3 done");

        CtlRad=new Ctl_BParam(); CtlRad->setToolTip("Radius control");
        CtlRadRel=new Ctl_BParam(); CtlRadRel->setToolTip("Radius control");
    CtlRadRel->OutMax=0.9899;
    CtlRadRel->SetValF(0.5);

        qDebug()<<("bcontrols part 1-4 done");

    CtlAng=new Ctl_BParam; CtlAng->setToolTip("Angle control");
    CtlAng->OutMax=360;
    CtlAng->OutMin=0;
    CtlAng->OutDef=360;
    CtlAng->ResetValue();
   // CtlAng->MLayout->setDirection(QBoxLayout::TopToBottom);
    CtlScaleRel=new Ctl_BParam; CtlScaleRel->setToolTip("Spacing");
    CtlScaleRel->OutMax=1;
    CtlScaleRel->OutMin=0;
    CtlScaleRel->OutDef=0.5;
    CtlScaleRel->ResetValue();
    CtlScale=new Ctl_BParam; CtlScale->setToolTip("Scale");
    CtlScale->OutMax=5;
    CtlScale->OutMin=0;
    CtlScale->OutDef=1;
    CtlScale->Gslider->DsRange=0.2;

    CtlScale->ResetValue();

    CtlSpc=new Ctl_BParam; CtlSpc->setToolTip("Spacing");
    CtlSpc->OutMax=2;
    CtlSpc->OutMin=0.05;
    CtlSpc->OutDef=0.3;
    CtlSpcJit=new Ctl_BParam; CtlSpcJit->setToolTip("Scattering");
    CtlSpcJit->OutMax=5;
    CtlSpcJit->OutMin=0;
    CtlSpcJit->OutDef=0;
    CtlSpc->ResetValue();
    CtlSpcJit->ResetValue();
    CtlOp=new Ctl_BParam; CtlOp->setToolTip("Opacity");
    CtlCop=new Ctl_BParam; CtlCop->setToolTip("Clone Opacity");
    CtlCop->OutMin=0.8;
    CtlLen=new Ctl_BParam; CtlLen->setToolTip("Brush Length");
    CtlLen->OutMax=1024;
    CtlLen->OutMin=0;
    CtlLen->OutDef=256;
    CtlLen->ResetValue();
    CtlPwr=new Ctl_BParam; CtlPwr->setToolTip("Tool Power");
    CtlPwr->OutMax=1;
    CtlPwr->OutMin=-1;
    CtlPwr->OutDef=0.5;
    CtlPwr->ResetValue();

    qDebug()<<("bcontrols part 1-5 done");

//CtlRadRel->Gslider->grad=CtlPwr->Gslider->grad;
//CtlCrv->Gslider->grad=CtlPwr->Gslider->grad;


    CtlSol=new Ctl_BParam; CtlSol->setToolTip("Solidity");
    CtlSol2=new Ctl_BParam; CtlSol2->setToolTip("Binarity");
    CtlCrv=new Ctl_BParam;
        CtlCrv->OutMax=1;
        CtlCrv->OutMin=-1;
        CtlCrv->OutDef=0;
        CtlCrv->setToolTip("Curvature");
        CtlCrv->ResetValue();
    CtlHue=new Ctl_BParam; CtlHue->setToolTip("Hue");
    CtlHue->Gslider->DsRange=0.0833333;
    CtlSat=new Ctl_BParam; CtlSat->setToolTip("Saturation");
    CtlLit=new Ctl_BParam; CtlLit->setToolTip("Lightness");

    CtlRad->SetIcon(AppPath+"/res/ctlRad.png");
    CtlRadRel->SetIcon(AppPath+"/res/ctlrrel.png");
    CtlScaleRel->SetIcon(AppPath+"/res/ctlScaleRel.png");
    CtlScale->SetIcon(AppPath+"/res/ctlScale.png");
    CtlAng->SetIcon(AppPath+"/res/ctlAng.png");
    CtlSpcJit->SetIcon(AppPath+"/res/ctlSpcJit.png");
    CtlSpc->SetIcon(AppPath+"/res/ctlSpc.png");
    CtlOp->SetIcon(AppPath+"/res/ctlop.png");
    CtlCop->SetIcon(AppPath+"/res/ctlcop.png");
    CtlSol->SetIcon(AppPath+"/res/ctlsol.png");
    CtlSol2->SetIcon(AppPath+"/res/ctlsol2op.png");
    CtlCrv->SetIcon(AppPath+"/res/ctlcrv.png");
    CtlPwr->SetIcon(AppPath+"/res/ctlpwr.png");
    CtlLen->SetIcon(AppPath+"/res/ctllen.png");
    CtlHue->SetIcon(AppPath+"/res/ctlhue.png");
    CtlSat->SetIcon(AppPath+"/res/ctlsat.png");
    CtlLit->SetIcon(AppPath+"/res/ctllit.png");
    CtlLit->SetValF(0.5);


    float step=RngConv(M_PI/6,0,M_PI,0,1);
    CtlHue->Gslider->grad->setColorAt(0,Qt::red);
    CtlHue->Gslider->grad->setColorAt(0+step,Qt::yellow);
    CtlHue->Gslider->grad->setColorAt(0+step*2,Qt::green);
    CtlHue->Gslider->grad->setColorAt(0+step*3,Qt::cyan);
    CtlHue->Gslider->grad->setColorAt(0+step*4,Qt::blue);
    CtlHue->Gslider->grad->setColorAt(0+step*5,QColor::fromRgbF(1,0,1,1));
    CtlHue->Gslider->grad->setColorAt(1,Qt::red);
    //CtlHue->Gslider->orient=1;
    CtlSat->Gslider->grad->setColorAt(0,QColor::fromHslF(1,0,0.5));
    CtlSat->Gslider->grad->setColorAt(1,QColor::fromHslF(1,1,0.5));

    CtlLit->Gslider->grad->setColorAt(0,QColor::fromHslF(0,0,0));
    CtlLit->Gslider->grad->setColorAt(0.5,QColor::fromHslF(1,1,0.5));
    CtlLit->Gslider->grad->setColorAt(1,QColor::fromHslF(0,0,1));

    CtlHue->SetValF(0.35);
    CtlSat->SetValF(1);
    CtlLit->SetValF(0);

    CtlCrv->ResetValue();





    LtLeftToolbar=new QVBoxLayout(this);
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

    g_Brush->rad_out=15;
    g_Brush->rad_in=5;

    //this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    logfile.write("\n BCONTROLS components inited+");  logfile.close(); logfile.open(QFile::Append);


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

    foreach (Ctl_BParam *ct,Controllers) connect(ct->Gslider,SIGNAL(Repainted()),this,SLOT(BrushRequest())) ;
/*
    CtlPwr->Gslider->grad->setColorAt(0,QColor::fromHslF(0,0,1));
    CtlPwr->Gslider->grad->setColorAt(0.5,QColor::fromHslF(0,0,0));
    CtlPwr->Gslider->grad->setColorAt(1,QColor::fromHslF(0,0,1));
    CtlScaleRel->Gslider->grad->setColorAt(0,QColor::fromHslF(0,0,1));
    CtlScaleRel->Gslider->grad->setColorAt(0.5,QColor::fromHslF(0,0,0));
    CtlScaleRel->Gslider->grad->setColorAt(1,QColor::fromHslF(0,0,1));
*/
    logfile.write("\n BCONTROLS INIT+++");  logfile.close(); logfile.open(QFile::Append);
}

d_Brush pnl_bcontorls::ParseBrush(d_Brush BRSH){

d_Brush nb;
return nb;
}

void pnl_bcontorls::SetRadRel(float rel){
    CtlRadRel->SetValF(rel);
}

d_Action pnl_bcontorls::ParseBrush(d_Stroke Strk,d_StrokePars stpars){

    d_Brush nb;
    d_Action act;

    nb.Realb.opacity=0.8;
        act.Brush=nb;
qreal op =CtlOp->GetModValue(stpars);
act.Brush.Realb.opacity=op;
    act.Brush.Realb.opacity=CtlOp->GetModValue(stpars);
    act.Brush.Realb.rad_out=g_Brush->rad_out*CtlRad->GetModValue(stpars);//*stpars.pressure;
    qreal minrad=3.0;
    if (act.Brush.Realb.rad_out<minrad) {
        act.Brush.Realb.opacity*=(act.Brush.Realb.rad_out)/minrad;
        act.Brush.Realb.rad_out=(minrad+act.Brush.Realb.rad_out)*0.5;
        }
act.Brush.Realb.rad_in=act.Brush.Realb.rad_out*CtlRadRel->GetModValue(stpars);//g_Brush->rad_in*CtlRad->GetModValue(stpars);//*stpars.pressure;


act.Brush.Realb.resangle=g_Brush->resangle+CtlAng->GetModValue(stpars);
if ((act.Brush.Realb.resangle==45)|(act.Brush.Realb.resangle==-45)|(act.Brush.Realb.resangle==135)|(act.Brush.Realb.resangle==225)|(act.Brush.Realb.resangle==315)) act.Brush.Realb.resangle=act.Brush.Realb.resangle+0.00001;

QColor DrawCol=QColor::fromHslF(CtlHue->GetModValue(stpars)
                                ,
                                CtlSat->GetModValue(stpars)
                                ,
                                CtlLit->GetModValue(stpars)
                                );

act.Brush.Realb.col=qRgba(DrawCol.red(),DrawCol.green(),DrawCol.blue(),round(act.Brush.Realb.opacity*255));

//if (stpars.Pars[csERASER]==1) act.Brush.col = g_EraseColor->UseCol;

act.Brush.Realb.crv=CtlCrv->GetModValue(stpars);
act.Brush.Realb.cop=CtlCop->GetModValue(stpars);
act.Brush.Realb.sol=CtlSol->GetModValue(stpars);
act.Brush.Realb.sol2op=CtlSol2->GetModValue(stpars);
act.Brush.Realb.x2y=CtlScaleRel->GetModValue(stpars);
act.Brush.Realb.scale=CtlScale->GetModValue(stpars);
act.Brush.Realb.scale=CtlScale->GetModValue(stpars);
//act.Brush.compmode= //ARTM->BMmodes.at(CtlCompMode->currentIndex());
act.Brush.Realb.bmidx=CtlCompMode->GetIdx();
act.Brush.Realb.noiseidx=CtlNoiseMode->currentIndex();
if (CtlNoiseMode->currentIndex()==0) //random
{
    act.Brush.Realb.seed= RNDf(65535);
            act.Brush.Realb.noisex=RawRnd(act.Brush.Realb.seed,1024);
            act.Brush.Realb.noisey=RawRnd(act.Brush.Realb.seed+21,1024);
  //  act.Brush.noisex=(qrand()/(float)RAND_MAX)*(1024);
  //  act.Brush.noisey=(qrand()/(float)RAND_MAX)*(1024);

}
else if (CtlNoiseMode->currentIndex()==1) { //constant
     act.Brush.Realb.seed=45698;
    //act.Brush.noisex=34;
    //act.Brush.noisey=76;
}
else if (CtlNoiseMode->currentIndex()==2) { //stencil
    //act.Brush.noisex=Strk.pos1.x();
    //act.Brush.noisey=Strk.pos1.y();
}
act.Noisemode=CtlNoiseMode->currentIndex();
act.Brush.Realb.noisex-=1024*floor(act.Brush.Realb.noisex/1024);
act.Brush.Realb.noisey-=1024*floor(act.Brush.Realb.noisey/1024);
//if (act.Brush.noisex<0) act.Brush.noisex-=1024*ceil(act.Brush.noisex/1024);
//if (act.Brush.noisey<0) act.Brush.noisey-=1024*ceil(act.Brush.noisey/1024);
if (act.Brush.Realb.noisex<act.Brush.Realb.rad_out) act.Brush.Realb.noisex+=1024;
if (act.Brush.Realb.noisey<act.Brush.Realb.rad_out) act.Brush.Realb.noisey+=1024;
act.Brush.Realb.pipeID=plCFNSR;
if (chkFastMode->isChecked()) act.Brush.Realb.pipeID=plRS;

act.ToolID=CtlTools->ToolID;
act.Brush.Realb.pwr=CtlPwr->GetModValue(stpars);
act.Stroke=Strk;
if (stpars.Pars[csERASER]==1) {
    act.Brush.Realb.bmidx=1;
}

act.Brush.SelfPack();

//emit SendAction(act);
return act;


}
void pnl_bcontorls::ApplyPreset(d_ToolPreset Preset){
    int cshift=0;
    if (Preset.usecolor==true) cshift=3;
        CtlCompMode->setBMbyint(Preset.compmode);
        CtlCompMode->SendChangedIdx(Preset.compmode);

       for (int i=0;i<Controllers.count()-cshift;i++){ //min and max are for all but colors if needed.
        Controllers[i]->Gslider->clipmaxF=Preset.pars[i].fmax;
        Controllers[i]->Gslider->clipminF=Preset.pars[i].fmin;
     }
    for (int i=0;i<Controllers.count();i++){ //jitter is saved for everyone
        Controllers[i]->Gslider->jitter=Preset.pars[i].fjit;
        //Controllers[i]->SetPenModeNum(Preset.pars[i].tmode);
     }
    CtlTools->Btns[Preset.toolid]->click();

    foreach (Ctl_BParam *ctl,Controllers) {
 ctl->Gslider->change();
        ctl->Gslider->Redraw();
}

}
d_ToolPreset pnl_bcontorls::GenPreset(){
    d_ToolPreset Preset;

        Preset.compmode=CtlCompMode->GetIdx();
        Preset.toolid=CtlTools->ToolID;
    for (int i=0;i<Controllers.count();i++){ //min and max are for all but colors if needed.
        Preset.pars[i].fmax=Controllers[i]->Gslider->clipmaxF;
        Preset.pars[i].fmin=Controllers[i]->Gslider->clipminF;
     }
    for (int i=0;i<Controllers.count();i++){ //jitter is saved for everyone
        Preset.pars[i].fjit=Controllers[i]->Gslider->jitter;
        Preset.pars[i].tmode=Controllers[i]->PenMode;

     }
return Preset;
}

void pnl_bcontorls::SendPreset(){
    emit TransferPreset(GenPreset());

}
void pnl_bcontorls::BrushRequest(){
    d_Stroke strk1;
    d_StrokePars stp1;
    for (int i=0;i<csSTOP;i++) stp1.Pars[i]=1;
    emit SendAction(ParseBrush(strk1,stp1));
}

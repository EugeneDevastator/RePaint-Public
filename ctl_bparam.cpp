#include "ctl_bparam.h"
#include "Brushes.h"
Ctl_BParam::Ctl_BParam(QWidget *parent) :
    QWidget(parent)
{
    qDebug()<<"bparam start";
    //Gslider=new bctl_ImageWidget;
    IRange=255*255;
    OutMax=1;
    OutMin=0;
    OutDef=1;
    slider = new QSlider(Qt::Horizontal);
    Gslider = new bctl_DblSlider;
    //Gslider->setFixedHeight(24);
    btn = new QPushButton;
    BtnPenMode=new QToolButton;
    BtnPenMode->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    MnuPen = new QMenu;
    PenState=0;
    QIcon Aicon;
    //QList <QString> IcNames;
    //QString IcNames[20];
   // QString Names[20];
    for (int i=0;i<20;i++)
    {
        //IcNames[i]="tct_nil";
        IcNames.append("nil");
        Names.append("nil");
    }
    /*
    enum csParams{
        csNone, csPressure, csVel, csDir, csRot, csTilt, csXtilt, csYtilt, csLen, csLenpx, csAcc, csCrv, csIdir, csHVdir, csHVrot, csSTOP
    };
    */
    IcNames[csNone]="tct_none";         Names[csNone]="MAX";
    IcNames[csPressure]="tct_pressure"; Names[csPressure]="Pressure";
    IcNames[csVel]="tct_vel";           Names[csVel]="Velocity";
    IcNames[csDir]="tct_dir";           Names[csDir]="Travel Direction";
    IcNames[csRot]="tct_rot";           Names[csRot]="Pen z-angle";
    IcNames[csTilt]="tct_tilt";         Names[csTilt]="Pen Tilt";
    IcNames[csHtilt]="tct_Htilt";       Names[csHtilt]="Pen H-Tilt";
    IcNames[csVtilt]="tct_Vtilt";       Names[csVtilt]="Pen V-Tilt";
    IcNames[csRelang]="tct_relang";     Names[csRelang]="angle between pen and dir";

    IcNames[csLen]="tct_len";           Names[csLen]="Length by brushes";
    IcNames[csLenpx]="tct_lenpx";       Names[csLenpx]="Length";
    IcNames[csAcc]="tct_acc";           Names[csAcc]="Acceleration";
    IcNames[csCrv]="tct_crv";           Names[csCrv]="Line Curvature";
    IcNames[csIdir]="tct_idir";         Names[csIdir]="Initial Dir";
    IcNames[csHVdir]="tct_hvdir";       Names[csHVdir]="Horizontality of Dir";
    IcNames[csHVrot]="tct_hvrot";       Names[csHVrot]="Horizontality of Pen angle";
    IcNames[csXtilt]="tct_xtilt";       Names[csXtilt]="Pen X-Tilt";
    IcNames[csYtilt]="tct_ytilt";       Names[csYtilt]="Pen Y-Tilt";


    /*IcNames.append("tct_none");         //+
    IcNames.append("tct_pressure");     //+
    IcNames.append("tct_vel");          //+
    IcNames.append("tct_rot");          //+ pen rotation;
    IcNames.append("tct_dir");          //+ travel direction
    //IcNames.append("tct_lineangle");  //-doubtful
    IcNames.append("tct_tilt");         //+ pen tilt
    IcNames.append("tct_xtilt");        //+ pen xtilt
    IcNames.append("tct_ytilt");        //+pen ytilt
    IcNames.append("tct_len");          //+spacing len
    IcNames.append("tct_lenpx");        //+real len
    IcNames.append("tct_HVdir");        //+HV by direction
    IcNames.append("tct_HVrot");        //+ pen HV*/

        qDebug()<<"bparam names done";
//QIcon nic;
//QString path;
        int range=csSTOP;
        int a=10;
    for (quint8 i = 0;i<range;i++){
       // path=RESPATH+"/res/"+IcNames[i]+".png";
     //qDebug()<<("bparam loop state at 0"+path);
        //nic=QIcon(path);
     qDebug()<<("bparam loop state at 1");
        //   nic.addFile(IcNames[i]+"_i.png",QSize(19,19),QIcon::Selected,QIcon::Off);
//        nic.addFile("tct_off.png",QSize(19,19),QIcon::Disabled,QIcon::Off);
        PenIcons.append(QIcon(RESPATH+"/res/"+IcNames[i]+".png"));
     qDebug()<<("bparam loop state at 2");
     MnuPen->addAction(PenIcons.last(),Names[i]);
     qDebug()<<("bparam loop state at 3");
        MnuPen->actions().at(i)->setProperty("val",i);
     qDebug()<<("bparam loop state at 4");
        //MnuPen->actionAt(QPoint(i,i))->setProperty("val",i);
     qDebug()<<("bparam loop done at"+QString::number(i)+" "+QString::number(PenIcons.count()));
    }
        qDebug()<<"bparam icons done";

  BtnPenMode->setMenu(MnuPen);
  BtnPenMode->setPopupMode(QToolButton::InstantPopup);
  BtnPenMode->setAttribute(Qt::WA_NoMousePropagation);
  connect(MnuPen,SIGNAL(triggered(QAction*)),this,SLOT(SetPenMode(QAction*)));
  connect(BtnPenMode,SIGNAL(clicked()),this,SLOT(SetPenInv()));
  connect(Gslider,SIGNAL(ValChange(float)),this,SLOT(SliderChange(float)));
    edline=new QLineEdit;
    edline->setFixedWidth(24);
    OutMode=1;
    SetRange(IRange);
    slider->setMinimum(0);
    slider->setMaximum(IRange);
    //slider->setStyle();
    ResetValue();
    linkedval=new float;

    qDebug()<<"bparam part1 done";

    LbIcon=new QLabel;
    MLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    MLayout->setDirection(QBoxLayout::LeftToRight);
    MLayout->addWidget(LbIcon);
    MLayout->addWidget(Gslider);
    MLayout->addWidget(BtnPenMode);
    MLayout->setSpacing(4);

    setLayout(MLayout);
    //this->setFixedHeight(34);
    MLayout->setMargin(5);
    //LbIcon->setFixedSize(32,32);
    LbIcon->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    MLayout->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    BtnPenMode->setFixedWidth(48);
    BtnPenMode->setFixedHeight(23);
    Gslider->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(Slide2Ed()));
qDebug()<<"bparam end";
}

void Ctl_BParam::FlipOrder(){

}

void Ctl_BParam::SetPenMode(QAction *mAct){
  //  BtnPenMode->icon()=mAct->icon();//setText(QString::number(mAct->property("val").toInt()));
    PenMode=mAct->property("val").toInt();
    BtnPenMode->setIcon(PenIcons.at(PenMode));
    //BtnPenMode->setText(QString::number(PenMode));
    PenState=1;
    emit SendPenMode(PenMode);
}
void Ctl_BParam::SetPenModeNum(int mode){
  //  BtnPenMode->icon()=mAct->icon();//setText(QString::number(mAct->property("val").toInt()));
    PenMode=mode;
    BtnPenMode->setIcon(PenIcons.at(mode));
    //BtnPenMode->setText(QString::number(PenMode));
    PenState=1;
}

void Ctl_BParam::SetPenInv(){
    PenState++;
    if (PenState==2) PenState=0;
    if (PenState==0) BtnPenMode->setIcon(PenIcons.at(PenMode).pixmap(QSize(24,24),QIcon::Disabled,QIcon::Off));
    else if (PenState==1) BtnPenMode->setIcon(PenIcons.at(PenMode).pixmap(QSize(24,24),QIcon::Active,QIcon::Off));
    //else if (PenState==1) BtnPenMode->setIcon(PenIcons.at(PenMode).pixmap(QSize(24,24),QIcon::Selected,QIcon::Off));
    /*
    QImage np(BtnPenMode->iconSize(),QImage::Format_ARGB32);
    //np.fill(QColor.fromRgbF(1,0,1,0.0));
    QPainter painter(&np);
    painter.drawImage(0,0,BtnPenMode->icon());
    //QPainter painter(&BtnPenMode->icon());
 //   BtnPenMode->;

*/

}

// ------------------------ SLIDER OPERATIONS ----------------------

void Ctl_BParam::Ed2Slide(){
    }
void Ctl_BParam::Slide2Ed(){
    edline->setText(QString::number(this->GetValue()));
}


void Ctl_BParam::SetRange(int x){
    if (OutMode>1) OutMode=x;
    slider->setMaximum(x);
}
void Ctl_BParam::SetMode(int x){
    if (x==2) OutMode=slider->maximum();
}
void Ctl_BParam::SetValF(float y){

    y=qMax(y,OutMin);
    y=qMin(y,OutMax);
    Gslider->GetVals(y, Gslider->clipminF, Gslider->jitter);
    Gslider->change();
    //Gslider->clipmaxF=y;
    Gslider->Redraw();

}

float Ctl_BParam::GetValue(){
    float OutR=OutMax-OutMin;
    //return (float)(OutR/IRange)*slider->value()+OutMin;
    return Gslider->clipmaxF*OutR+OutMin;

    //return ((float)slider->value()/(float)OutMode);
}

qreal Ctl_BParam::GetModValue(d_StrokePars stpars){
float cpar=1;
    if (PenState>0) {
        if (PenMode>0)
            cpar=stpars.Pars[PenMode];
    }
    float OutR=OutMax-OutMin;
    float rng = Gslider->clipmaxF-Gslider->clipminF;
    float respar = cpar*rng+Gslider->clipminF;
    //float randm = ((float)qrand()/RAND_MAX)*(Gslider->jitter*2)-(Gslider->jitter);
    float randm = (((float)qrand()/RAND_MAX)-0.5)*2*Gslider->jitter;
float res=(respar+randm);
res=qMax(res,(float)0.0);
res=qMin(res,(float)1);
return res*OutR+OutMin;
    //return (respar+rand)*OutR+OutMin;

    //return ((float)slider->value()/(float)OutMode);
}

void Ctl_BParam::ResetValue(){
    float OutR=OutMax-OutMin;


    //slider->setValue((float)(OutDef-OutMin)*IRange/OutR);
   Gslider->clipmaxF=(double)(OutDef-OutMin)/OutR;
    //return ((float)slider->value()/(float)OutMode);
}

void Ctl_BParam::SliderChange(float val){
    emit NewValue(GetValue());
}

void Ctl_BParam::ApplyValue(float newval){
    float OutR=OutMax-OutMin;
    slider->setValue((float)(newval-OutMin)*IRange/OutR);
    //return ((float)slider->value()/(float)OutMode);
}
void Ctl_BParam::SetIcon(QString pathstr){
QPixmap *pic=new QPixmap;
pic->load(pathstr);
LbIcon->setPixmap(*pic);
//LbIcon->setScaledContents(true);
//LbIcon->set
//LbIcon->setFixedHeight(24);
//LbIcon->setFixedWidth(24);
}

void Ctl_BParam::mousePressEvent(QMouseEvent *event){
 if (event->button()==Qt::RightButton) ResetValue();
}



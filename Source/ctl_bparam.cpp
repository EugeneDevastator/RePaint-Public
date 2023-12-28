#include "ctl_bparam.h"
#include "Brushes.h"

BrushDialWidget::BrushDialWidget(DialModel *model,QWidget *parent) :
        QWidget(parent){

    if(model)
        Model = model;
    else {
        Model = new DialModel(0,1,1);
    }
    Model->PenState = 0;
    Model->PenMode = 0;
    Model->OutMode = 1;

    Slider = new DialSliderWidget(Model);
    // Slider->setFixedHeight(24);
    BtnPenMode = new QToolButton;
    BtnPenMode->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    MnuPen = new QMenu;

    QIcon Aicon;
    // QList <QString> IcNames;
    // QString IcNames[20];
    // QString Names[20];
    for (int i = 0; i < 20; i++) {
        // IcNames[i]="tct_nil";
        IcNames.append("nil");
        Names.append("nil");
    }
    /*
    enum ParameterControlKind{
        csNone, PenPressure, Velocity, Direction, csRot, csTilt, csXtilt, csYtilt, csLen, csLenpx, csAcc, csCrv, csIdir, csHVdir, csHVrot, csSTOP
    };
    */
    IcNames[csNone] = "tct_none";
    Names[csNone] = "MAX";
    IcNames[PenPressure] = "tct_pressure";
    Names[PenPressure] = "Pressure";
    IcNames[Velocity] = "tct_vel";
    Names[Velocity] = "Velocity";
    IcNames[Direction] = "tct_dir";
    Names[Direction] = "Travel Direction";
    IcNames[csRot] = "tct_rot";
    Names[csRot] = "Pen z-angle";
    IcNames[csTilt] = "tct_tilt";
    Names[csTilt] = "Pen Tilt";
    IcNames[csHtilt] = "tct_Htilt";
    Names[csHtilt] = "Pen H-Tilt";
    IcNames[csVtilt] = "tct_Vtilt";
    Names[csVtilt] = "Pen V-Tilt";
    IcNames[csRelang] = "tct_relang";
    Names[csRelang] = "angle between pen and dir";

    IcNames[csLen] = "tct_len";
    Names[csLen] = "Length by brushes";
    IcNames[csLenpx] = "tct_lenpx";
    Names[csLenpx] = "Length";
    IcNames[csAcc] = "tct_acc";
    Names[csAcc] = "Acceleration";
    IcNames[csCrv] = "tct_crv";
    Names[csCrv] = "Line Curvature";
    IcNames[csIdir] = "tct_idir";
    Names[csIdir] = "Initial Dir";
    IcNames[csHVdir] = "tct_hvdir";
    Names[csHVdir] = "Horizontality of Dir";
    IcNames[csHVrot] = "tct_hvrot";
    Names[csHVrot] = "Horizontality of Pen angle";
    IcNames[csXtilt] = "tct_xtilt";
    Names[csXtilt] = "Pen X-Tilt";
    IcNames[csYtilt] = "tct_ytilt";
    Names[csYtilt] = "Pen Y-Tilt";

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
    // QIcon nic;
    // QString path;
    int range = csSTOP;
    for (quint8 i = 0; i < range; i++) {
        PenIcons.append(QIcon(RESPATH + "/res/" + IcNames[i] + ".png"));
        MnuPen->addAction(PenIcons.last(), Names[i]);
        MnuPen->actions().at(i)->setProperty("val", i);
        // MnuPen->actionAt(QPoint(i,i))->setProperty("val",i);
    }
    BtnPenMode->setMenu(MnuPen);
    BtnPenMode->setPopupMode(QToolButton::InstantPopup);
    BtnPenMode->setAttribute(Qt::WA_NoMousePropagation);

    ValueLabel = new QLineEdit;
    ValueLabel->setFixedWidth(24);


    // oldSlider->setStyle();
    Slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    LbIcon = new QLabel;
    MLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    MLayout->setDirection(QBoxLayout::LeftToRight);
    MLayout->addWidget(LbIcon);
    MLayout->addWidget(Slider);
    MLayout->addWidget(BtnPenMode);
    MLayout->setSpacing(4);

    setLayout(MLayout);
    // this->setFixedHeight(34);
    MLayout->setMargin(5);
    // LbIcon->setFixedSize(32,32);
    LbIcon->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    MLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    BtnPenMode->setFixedWidth(48);
    BtnPenMode->setFixedHeight(23);


    connect(MnuPen, SIGNAL(triggered(QAction * )), this, SLOT(SetPenMode(QAction * )));
    connect(BtnPenMode, SIGNAL(clicked()), Model, SLOT(SetPenInv()));
    connect(Slider, SIGNAL(ValChange(float)), this, SLOT(SliderChange(float)));
    //connect(oldSlider, SIGNAL(valueChanged(int)), this, SLOT(HandleSliderUpdate()));
    connect(Model, SIGNAL(ChangedSignal()), this, SLOT(HandleModelUpdate()));

    HandleModelUpdate();
}

void BrushDialWidget::SetPenMode(QAction *mAct) {
    //  BtnPenMode->icon()=mAct->icon();//setText(QString::number(mAct->property("val").toInt()));
    auto mode = mAct->property("val").toInt();
    Model->SetPenModeNum(mAct->property("val").toInt());
}

void BrushDialWidget::HandleModelUpdate() {
    BtnPenMode->setIcon(PenIcons.at(Model->PenMode));

    if (Model->PenState == 0)
        BtnPenMode->setIcon(PenIcons.at(Model->PenMode).pixmap(QSize(24, 24), QIcon::Disabled, QIcon::Off));
    else if (Model->PenState == 1)
        BtnPenMode->setIcon(PenIcons.at(Model->PenMode).pixmap(QSize(24, 24), QIcon::Active, QIcon::Off));
    // else if (PenState==1) BtnPenMode->setIcon(PenIcons.at(PenMode).pixmap(QSize(24,24),QIcon::Selected,QIcon::Off));
    /*
    QImage np(BtnPenMode->iconSize(),QImage::Format_ARGB32);
    //np.fill(QColor.fromRgbF(1,0,1,0.0));
    QPainter painter(&np);
    painter.drawImage(0,0,BtnPenMode->icon());
    //QPainter painter(&BtnPenMode->icon());
 //   BtnPenMode->;
*/
    Slider->SetValues( Model->MaxCursorNorm,Model->MinCursorNorm, Model->Jitter);
    ValueLabel->setText(QString::number(Model->GetValueAtMax()));
}

// ------------------------ SLIDER OPERATIONS ----------------------

qreal BrushDialWidget::GetModValue(d_StrokePars stpars) {
    float modulationValue = 1;
    if (Model->PenState > 0) {
        if (Model->PenMode > 0)
            modulationValue = stpars.Pars[Model->PenMode];
    }
    float OutR = Model->GetBoundRange();
    float sliderRange = Slider->clipmaxF - Slider->clipminF;
    float respar = modulationValue * sliderRange + Slider->clipminF;

    // float randm = ((float)qrand()/RAND_MAX)*(Slider->jitter*2)-(Slider->jitter);
    float randm = (((float) qrand() / RAND_MAX) - 0.5) * 2 * Slider->jitter;

    float res = (respar + randm);
    res = modulationValue + randm;
    res = qMax(res, (float) 0.0);
    res = qMin(res, (float) 1);

    return Model->GetValueInCursorRange(modulationValue);
    //return res * OutR + Model->MinBound;


    // return (respar+rand)*OutR+MinBound;
    // return ((float)oldSlider->value()/(float)OutMode);
}


void BrushDialWidget::SliderChange(float val) {
    Model->SetCursors(Slider->clipminF, Slider->clipmaxF, Slider->jitter);
   // emit NewValue(GetMaxValue());
}

float BrushDialWidget::GetMaxValue(){
    return Model->GetValueAtMax();
}

void BrushDialWidget::SetIcon(QString pathstr) {
    QPixmap *pic = new QPixmap;
    pic->load(pathstr);
    LbIcon->setPixmap(*pic);
    // LbIcon->setScaledContents(true);
    // LbIcon->set
    // LbIcon->setFixedHeight(24);
    // LbIcon->setFixedWidth(24);
}

void BrushDialWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton)
        Model->ResetValue();
}

void BrushDialWidget::Refresh() {
    HandleModelUpdate();
}

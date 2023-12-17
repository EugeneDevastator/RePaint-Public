#include <QtGui>
#include "MHPwindow.h"
#include <QtNetwork>
#include <QTabletEvent>
#include <QtWidgets/QFileDialog>
#include "geomaster.h"
#include "BrushEngine/BrushStamp.hpp"
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplashScreen>
// #include <QDomDocument>
// #include "signal.h"
MHPwindow::MHPwindow()
{

    // ctl_ColorPicker *cpik=new ctl_ColorPicker;
    // cpik->show();
    PanelsPinned = true;
    this->hide();
    /* Ctl_BParam *CtlTest=new Ctl_BParam; ///CtlScaleRel->setToolTip("Spacing");
     CtlTest->OutMax=1;
     CtlTest->OutMin=0;
     CtlTest->OutDef=0.5;
     CtlTest->ResetValue();
     CtlTest->show();
 CtlTest->SetIcon(RESPATH+"/res/ctlRad.png");
     */
    Dedicated = false;

    QFile logfile("d:/mhplog.log");
    logfile.open(QFile::ReadWrite | QFile::Truncate);

    QPixmap SplashPic(RESPATH + "/res/splash.png");
    QSplashScreen splash(SplashPic);
    splash.show();
    splash.setPixmap(SplashPic);
    // splash.showMessage("Loading REPAINT full...");
    // splash.resize(500,500);
    // splash.move(500,200);
    QApplication::processEvents();
    QString spath;
    spath = RESPATH + "res/icon16.png";
    this->setWindowIcon(QIcon(RESPATH + "/res/icon16.png"));
    SetAppMetaInfo();

    SnapIdx = 0;
    ActiveLayer = 0;
    PansHidden = false;

    // Root level Dependencies
    KBLINK = new c_KeyLink();
    Brush = new ClientBrushStamp;

    Brush->sol2op = 1;
    Brush->rad_out = 20;
    Brush->rad_in = 10;
    Brush->resangle = 0.0;

    BControls = new BrushEditorPresenter(Brush);

    AllPanels.append(BControls);

    MainImage = new ImageArray(Brush, BControls,KBLINK);

    DlgAbout = new QDialog();

    //--------------- MASTERS INIT --------------

    PaintColor = new b_SmartColor();
    EraserColor = new b_SmartColor();


    bool singlecore = false;
    ActionExecutor = new ActionMaster(MainImage, singlecore, this);
    ActionExecutor->executing = true;
    NET = new NetClient();
    sNET = new NetServer();

    /* ARTM= new ArtThread(MainImage);
     if (QThread::idealThreadCount()<2) {
             ARTM->terminate();
             ARTM->ForceSingleCore();
          }
    // ARTM->ForceSingleCore();
 */

    //--------------- PANELS INIT
    CHAT = new pnl_Chat;
    CHAT->setAccessibleName("Chat");
    CHAT->setWindowTitle("Chat");
    //     AllPanels.append(CHAT);
    CHAT->hide();

    LayersPanel = new LayersPanelPresenter();
    AllPanels.append(LayersPanel);
    LayersPanel->show();
    FileMenu = new pnl_FileMenu;
    FileMenu->setAccessibleName("FileMenu");
    FileMenu->setWindowTitle("File Operations");
    AllPanels.append(FileMenu);
    DlgNew = new dlg_NewCanvas(MainImage);


    NetControls = new pnl_NetControls();
    NetControls->setAccessibleName("NetControls");
    NetControls->setWindowTitle("Network");
    AllPanels.append(NetControls);
    // NetControls->hide();
    FriendList = new ctl_friendlist(NET);
    FriendList->setAccessibleName("FriendList");
    FriendList->setWindowTitle("Friends");
    AllPanels.append(FriendList);
    // FriendList->hide();

    StrokeMaster = new class StrokeMaster(BControls, LayersPanel, MainImage);
    NetControls->init(NET, sNET);
    FriendList->NET = NET;

    PaintColor->SetCol(Qt::black);
    EraserColor->SetCol(Qt::white);
    //  BControls->BrushControl->Brush=Brush;

    //------------- end of fast panel init;



    logfile.write("\n third init block complete-1");
    logfile.close();
    logfile.open(QFile::Append);

    // -------------------------------------

    // TcpClient->connectToHost("127.0.0.1",33789);
    // TcpServer->serverPort()=33789;
    // if (TcpServer->listen()) {

    Label1 = new QLabel("jkl");

    // LblServIp->setTextInteractionFlags(Qt::TextSelectableByMouse);
    MainLayout = new QGridLayout;
    MainLayout->setMargin(0);

    // connect(MainImage,SIGNAL(SendNewB(QPoint,int,qreal,qreal)),HoverBrush,SLOT(GetNewB(QPoint,int,qreal,qreal)));

    // MainImage->updateGeometry();
    logfile.write("\n layout block complete");
    logfile.close();
    logfile.open(QFile::Append);

    PRESETS = new ctl_UserTools(BControls, this);
    PRESETS->setAccessibleName("PresetPanel");
    AllPanels.append(PRESETS);

    foreach (QWidget *w, AllPanels)
    {
        w->setParent(this);
        //  w->setWindowFlags(Qt::Tool|Qt::CustomizeWindowHint|Qt::WindowMinimizeButtonHint);
        w->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
        w->show();
    }

    MainLayout->addWidget(MainImage, 1, 0);

    setLayout(MainLayout);

    logfile.write("\n main layout block complete");
    logfile.close();
    logfile.open(QFile::Append);

    QVBoxLayout *DAbt = new QVBoxLayout;
    Lb1 = new QLabel;
    QPixmap pic1 = QPixmap(RESPATH + "/res/repaint.jpg");

    Lb1->setPixmap(pic1);
    DAbt->addWidget(Lb1);
    DAbt->setAlignment(DAbt, Qt::AlignTop);
    DAbt->setSpacing(0);
    DAbt->setMargin(0);
    DlgAbout->setLayout(DAbt);
    DlgAbout->setFixedWidth(pic1.width());
    DlgAbout->setFixedHeight(pic1.height());
    DlgAbout->setModal(true);
    // DlgCol->setMask(QRegion(0,0,0,0));
    // DlgCol->setGraphicsEffect();
    // DlgAbout->setWindowFlags(Qt::FramelessWindowHint);
    // DlgAbout->hide();
    DlgAbout->hide();

    BControls->Flist = FriendList;

    DlgLogin = new dlg_login;
    DlgLogin->hide();

    logfile.write("\n dialog init block complete");
    logfile.close();
    logfile.open(QFile::Append);

    QuickPanel = new pnl_QuickControl(BControls, Brush, MainImage);
    //  please connect with qp
    connect(QuickPanel->QuickLit->Gslider, SIGNAL(ValChange(float)), this, SLOT(RedrawCol()));
    connect(QuickPanel->QuickSat->Gslider, SIGNAL(ValChange(float)), this, SLOT(RedrawCol()));
    connect(QuickPanel->QuickHue->Gslider, SIGNAL(ValChange(float)), this, SLOT(RedrawCol()));
    connect(QuickPanel->QuickHue->Gslider, SIGNAL(ValChange(float)), PaintColor, SLOT(SetHueF(float)));
    connect(QuickPanel->QuickSat->Gslider, SIGNAL(ValChange(float)), PaintColor, SLOT(SetSatF(float)));
    connect(QuickPanel->QuickLit->Gslider, SIGNAL(ValChange(float)), PaintColor, SLOT(SetLitF(float)));

    QuickPanel->RealignPanel();

    connect(FileMenu->BtnNew, SIGNAL(clicked()), DlgNew, SLOT(exec()));
    connect(DlgNew, SIGNAL(SendSize(QSize)), this, SLOT(NewImage(QSize)));
    connect(DlgNew, SIGNAL(SendRescale(QSize, int)), MainImage, SLOT(ResizeScene(QSize, int)));
    qDebug() << ("MHPW connects part-1 done");

    connect(FileMenu->BtnOpen, SIGNAL(clicked()), this, SLOT(OpenImage()));
    connect(FileMenu->BtnReload, SIGNAL(clicked()), this, SLOT(ReOpenImage()));
    connect(FileMenu->BtnSaveAs, SIGNAL(clicked()), this, SLOT(SaveImgAs()));
    connect(FileMenu->BtnSave, SIGNAL(clicked()), this, SLOT(SaveImg()));
    connect(FileMenu->BtnSnap, SIGNAL(clicked()), this, SLOT(SnapImage()));
    connect(NetControls->BtnAbout, SIGNAL(clicked()), DlgAbout, SLOT(show()));
    connect(FileMenu->BtnPaste, SIGNAL(clicked()), this, SLOT(PasteImage()));
    connect(FileMenu->BtnPinPanels, SIGNAL(clicked()), this, SLOT(SwitchPinPanels()));
    //    connect(BtnLogin,SIGNAL(clicked()),DlgLogin,SLOT(show()));
    //---------
    qDebug() << ("MHPW connects part-2 done");
    connect(NET, SIGNAL(SendAction(d_Action)), this, SLOT(GetAction(d_Action)));
    connect(NET, SIGNAL(SendStatus(QString)), this, SLOT(GetMsg(QString)));
    connect(NET, SIGNAL(SendServIp(QString)), this, SLOT(GetServIp(QString)));
    connect(NET, SIGNAL(SendChatMsg(QString)), CHAT, SLOT(GetChatMsg(QString)));
    connect(NET, SIGNAL(ReqLogin(QString)), this, SLOT(LoadLogin()));
    connect(NET, SIGNAL(ReqLogin(QString)), DlgLogin, SLOT(RequestLog(QString)));

    qDebug() << ("MHPW connects part-3 done");
    connect(NET, SIGNAL(LoginSuccess()), DlgLogin, SLOT(hide()));
    connect(NET, SIGNAL(LoginSuccess()), this, SLOT(SaveLogin()));
    connect(NET, SIGNAL(ClientEnd(QString)), FriendList, SLOT(DelUser(QString)));
    connect(NET, SIGNAL(NetReset()), FriendList, SLOT(Purge()));
    connect(NET, SIGNAL(SendUserStatus(QString, int)), FriendList, SLOT(SetUserStatus(QString, int)));
    connect(NET, SIGNAL(SendUserRJ(QString, int)), FriendList, SLOT(AddToRoom(QString, int)));
    connect(NET, SIGNAL(SendPartRoom(QString)), FriendList, SLOT(DelFromRoom(QString)));

    connect(NET, SIGNAL(LockCanvas(qint8)), this, SLOT(LockCanvas(qint8)));
    if (!Dedicated)
        connect(NET, SIGNAL(GetImage(QByteArray)), ActionExecutor, SLOT(OpenImgBa(QByteArray)));
    connect(NET, SIGNAL(GetImage(QByteArray)), this, SLOT(ConfirmImage(QByteArray)));
    connect(NET, SIGNAL(ReqImage(QString)), this, SLOT(GrabImg(QString)));
    if (!Dedicated)
        connect(NET, SIGNAL(SendLAction(LayerAction)), ActionExecutor, SLOT(ExecLayerAction(LayerAction)));

    qDebug() << ("MHPW connects part-4 done");
    connect(NetControls, SIGNAL(sendlock(qint8)), LayersPanel, SLOT(SetLock(qint8)));
    connect(NetControls, SIGNAL(sendlock(qint8)), MainImage, SLOT(SetLock(qint8)));
    //---------
    //        connect(ARTM,SIGNAL(SendMsg(QString)),CHAT,SLOT(GetChatMsg(QString)));

    //  connect(MainImage,SIGNAL(SendStroke(d_Stroke)),this,SLOT(GetStroke(d_Stroke)));
    connect(MainImage, SIGNAL(SendTStroke2(d_Stroke, d_StrokePars, d_StrokePars)), StrokeMaster, SLOT(GetRawStroke(d_Stroke, d_StrokePars, d_StrokePars)));
    connect(MainImage, SIGNAL(SendTStroke(d_Stroke, d_StrokePars)), this, SLOT(GetTStroke(d_Stroke, d_StrokePars)));

    // connect(BControls->BrushControl,SIGNAL(SendSize(int)),MainImage,SLOT(GetBSize(int)));

    // connect(this,SIGNAL(destroyed()),DlgCol,SLOT(close()));
    // connect(MainImage,SIGNAL(SendColor(QColor)),CtlCol,SLOT(SetColor(QColor)));
    connect(MainImage, SIGNAL(AskResetPos()), StrokeMaster, SLOT(ResetLocalPos()), Qt::DirectConnection);
    connect(MainImage, SIGNAL(SendColor(QColor)), this, SLOT(GetColor(QColor)));
    connect(MainImage, SIGNAL(SendColor(QColor)), PaintColor, SLOT(SetCol(QColor)));
    qDebug() << ("MHPW connects part-5 done");
    connect(MainImage, SIGNAL(MouseIn()), this, SLOT(GrabKB()));
    connect(LayersPanel, SIGNAL(MouseIn()), this, SLOT(GrabKB()));
    //    connect(MainImage,SIGNAL(MouseOut()),this,SLOT(RelKB()));

    connect(MainImage, SIGNAL(SendThumb(int, QImage)), LayersPanel, SLOT(SetThumb(int, QImage)));
    connect(ActionExecutor, SIGNAL(SendLAction(LayerAction)), LayersPanel, SLOT(ExecLAction(LayerAction)));
    connect(MainImage, SIGNAL(SendPoly(QPolygonF)), this, SLOT(GetPoly(QPolygonF)));

    // please remove this later
    connect(StrokeMaster, SIGNAL(SendReadySect(StrokeSection)), ActionExecutor, SLOT(ExecSection(StrokeSection)));
    connect(ActionExecutor, SIGNAL(SendSection(StrokeSection)), NET, SLOT(GetSection(StrokeSection)));
    if (!Dedicated)
        connect(NET, SIGNAL(SendSection(StrokeSection)), ActionExecutor, SLOT(ExecNetSection(StrokeSection)));

    connect(LayersPanel, SIGNAL(SendActiveLayer(int)), MainImage, SLOT(SetActiveLayer(int)));
    connect(LayersPanel, SIGNAL(SendLayerVis(int, bool)), MainImage, SLOT(SetLvis(int, bool)));
    connect(LayersPanel, SIGNAL(SendLayerAction(LayerAction)), this, SLOT(ExecLayerAction(LayerAction)));
    connect(LayersPanel, SIGNAL(ActionDone()), ActionExecutor, SLOT(ConfirmLAction()));
    connect(LayersPanel, SIGNAL(ActionDone()), MainImage, SLOT(GenAllThumbs()));
    connect(LayersPanel, SIGNAL(ActionDone()), QuickPanel, SLOT(UpdateBG()));

    qDebug() << ("MHPW connects part-6 done");

    // connect(CtlCol,SIGNAL(SendColor(QColor)),this,SLOT(GetColor(QColor)));
    connect(BControls->CtlHue->Gslider, SIGNAL(ValChange(float)), PaintColor, SLOT(SetHueF(float)));
    connect(BControls->CtlSat->Gslider, SIGNAL(ValChange(float)), PaintColor, SLOT(SetSatF(float)));
    connect(BControls->CtlLit->Gslider, SIGNAL(ValChange(float)), PaintColor, SLOT(SetLitF(float)));

    connect(BControls->CtlHue->Gslider, SIGNAL(ValChange(float)), this, SLOT(RedrawCol()));
    connect(BControls->CtlSat->Gslider, SIGNAL(ValChange(float)), this, SLOT(RedrawCol()));
    connect(BControls->CtlLit->Gslider, SIGNAL(ValChange(float)), this, SLOT(RedrawCol()));

    connect(BControls->CtlLen, SIGNAL(NewValue(float)), MainImage, SLOT(SetLengths(float)));
    connect(CHAT->ChatLine, SIGNAL(returnPressed()), this, SLOT(SendChatMsg()));
    // ----------------
    // connect(DlgLogin,SIGNAL(SendLogin(QString,QString)),this,SLOT(GetLogin(QString,QString)));
    connect(DlgLogin, SIGNAL(SendLogin(QString, QString)), NET, SLOT(C_Login(QString, QString)));
    connect(DlgLogin, SIGNAL(SendReg(QString, QString)), NET, SLOT(C_Regin(QString, QString)));
    connect(DlgLogin, SIGNAL(rejected()), NET, SLOT(N_Disconnect()));
    connect(DlgLogin, SIGNAL(rejected()), sNET, SLOT(N_Disconnect()));
    //  connect(DlgLogin,SIGNAL(SendLogin(QString,QString)),this,SLOT(SaveLogin()));

    connect(FriendList, SIGNAL(SendJoin(QString)), NET, SLOT(C_JoinRoom(QString)));
    connect(FriendList, SIGNAL(SendJoin(QString)), CHAT, SLOT(GetChatMsg(QString)));
    connect(FriendList, SIGNAL(SendIntMsg(QString)), CHAT, SLOT(GetIntchatMsg(QString)));
    connect(NetControls->BtnPartRoom, SIGNAL(clicked()), FriendList, SLOT(LeaveRoom()));

    qDebug() << ("MHPW connects part-all done");

    // ARTM->start();
    logfile.write("\n connects init block complete");
    logfile.close();
    logfile.open(QFile::Append);

    LayersPanel->LayerProps = &MainImage->LayerProps;
    logfile.write("\n props init block complete");
    logfile.close();
    logfile.open(QFile::Append);



    FileMenu->BtnSnap->setEnabled(false);
    FileMenu->BtnSave->setEnabled(false);
    FileMenu->BtnReload->setEnabled(false);

    qDebug() << ("buttons done");
    GetColor(Qt::cyan);

    RedrawCol();
    ShowQP();
    HideQP();
    // MainImage->AddZeroLayer(QSize(1920,1080),QColor::fromRgb(1,1,0.9));
    qDebug() << ("color done");
    //   if (!Dedicated)
    //      MainImage->NewImg(QSize(1920,1200),3);
    //  else
    //   MainImage->NewImg(QSize(1920,1200),1);
    LayerAction lact;
    lact.ActID = laNewCanvas;
    lact.layer = 3;
    lact.rect.setSize(QSize(1920, 1200));
    ActionExecutor->ExecLayerAction(lact);
    //       MainImage->NewImg(QSize(1920,1200),1);

    //        QImage img(RESPATH+"/res/eyeris.jpg");

    QPainter Pnt(&MainImage->ViewCanvas[0]);
    // Pnt.setCompositionMode(QPainter::CompositionMode_Source);
    // Pnt.drawImage(0,0,clipboard->image());

    Pnt.setPen(QPen(Qt::darkGreen));
    Pnt.setBrush(Qt::yellow);

    GeoMaster GM;
    GM.AddVec(QVector<QPointF>() << QPointF(10, 10) << QPointF(10, 50));
    GM.AddVec(QVector<QPointF>() << QPointF(20, 10) << QPointF(20, 60));
    GM.AddVec(QVector<QPointF>() << QPointF(50, 4) << QPointF(45, 30));

    // unlock for polypaint Pnt.drawPolygon(GM.IssuePoly(),Qt::WindingFill);

    // this->setWindowTitle(NET->GenForumHash("[)evastator","password"));
    //$1$LKUcDhZo$
    //$1$LKUcDhZo$drghukl = wrong
    // this->setWindowTitle(NET->GenForumHash("drghukl$1$LKUcDhZo",""));
    // qsrand(456);
    //       this->setWindowTitle(QString::number(qrand()));
    //    this->setWindowTitle(this->windowTitle()+ " " +QString::number(qrand()));
    // LKUcDhZo

    // ---- testing spacing

    // CHAT->ChatLine->setText(QString::number(sizeof(d_PointF))+"qpointf"+QString::number(sizeof(act.Brush.rad_in)) +" ral:"+QString::number(bas.size()));

    // BControls->BrushControl->Redraw();
    // QRect screen = QApplication::desktop()->screenGeometry();
    QRect screen(0, 0, 1200, 800);
    // QDesktopWidget *screen;
    int nw = 1200; // screen.width()*(0.6666+0.3333*0.6666);
    // int nh=screen.height()*0.6666;

    /*QRect availableGeometry(QApplication::desktop()->availableGeometry());
  availableGeometry.setRect((screen.width()-nw)*0.5,
                          (screen.height()-nh)*0.5,
                          nw,
                          nh
                          );
                          */

    this->move(QPoint(0, 0));
    this->setGeometry((screen.width() - nw) * 0.5,
                      32,
                      nw,
                      this->height());
    MainImage->FitScreen();
    MainImage->ActualPixels();
    ShowQP();
    QuickPanel->RealignPanel();
    HideQP();

    //   CHAT->Chat->append("Threadcount"+QString::number(ARTM->corecount));
    //  if (ARTM->singlecore) CHAT->Chat->append("running as SINGLECORE");
    //  if (!ARTM->singlecore) CHAT->Chat->append("running as threaded");
    CHAT->Chat->append("arththreadcount " + QString::number(ActionExecutor->atcount));
    CHAT->Chat->append(QString::number(RawRnd(10, 0)) + " ");
    CHAT->Chat->append(QString::number(RawRnd(20, 0)) + " ");
    CHAT->Chat->append(QString::number(RawRnd(30, 0)) + " ");

    // CHAT->Chat->append(QString::number(act.SerializeInto().count()));

    StrokeSection tsec1;
    // tsec1.DeSerialize(act.SerializeInto());
    int h = 0;

    // Applying settings to window geometries;

    this->setGeometry(AppSettings->value("MainGeo", this->geometry()).toRect());
    foreach (QWidget *w, AllPanels)
    {
        w->setGeometry(AppSettings->value(w->accessibleName(), w->geometry()).toRect());
        w->setFocusProxy(this);
    }
    this->show();

    //// Brush image selector. looks like it causes some bugs.
//
    //ctl_AssetScene *ASV;
    //ASV = new ctl_AssetScene;
    //ASV->InitByPath(QApplication::applicationDirPath() + "/Brushes/");
    //  //ASV->show();
//
    //ctl_assetview *GV = new ctl_assetview(ASV);
    //GV->setParent(this);
    //GV->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
    //GV->show();

    // ActionExecutor->NewLog();
    // ActionExecutor->OpenLog("/Users/Devastator/QTProjects/MHP/debug/pics/log5.ELI");
    CHAT->GetIntchatMsg("HUIHUI=" + QString::number(1 % 3));

    /*
            QStandardItemModel *itmodel=new QStandardItemModel;
            QListView *ListView = new QListView;
    ListView->setModel(itmodel);

            ListView->setIconSize(QSize(64,64));
            ListView->setSelectionMode(QAbstractItemView::SingleSelection);

        //    ListView->setDropIndicatorShown(true);
    ListView->setSelectionBehavior(QAbstractItemView::SelectRows);
           ListView->setDragEnabled(true);
            ListView->setAcceptDrops(true);
       //     ListView->setDropIndicatorShown(true);
    ListView->setDefaultDropAction(Qt::MoveAction);
    ListView->setDragDropMode(QAbstractItemView::InternalMove);
    //ListView->setDragDropOverwriteMode(false);

         //   itmodel->setSupportedDragActions(Qt::MoveAction);
            ListView->show();

            QStandardItem *itm1=new QStandardItem;
            itm1->setText("item1");
            QStandardItem *itm2=new QStandardItem;
            itm2->setText("item2");
            QStandardItem *itm3=new QStandardItem;
            itm3->setText("item3");

            itm1->setFlags(itm1->flags() ^ (Qt::ItemIsDropEnabled));
            itm2->setFlags(itm2->flags() ^ (Qt::ItemIsDropEnabled));
            itm3->setFlags(itm3->flags() ^ (Qt::ItemIsDropEnabled));

    itmodel->appendRow(itm1);
    itmodel->appendRow(itm2);
    itmodel->appendRow(itm3);

    ListView->showDropIndicator();
    FriendList->AddUser("hui1");
    FriendList->AddUser("hui2");
    FriendList->AddUser("hui3");

    QListWidget *listWidget = new QListWidget();
    new QListWidgetItem(tr("Oak"), listWidget);
    new QListWidgetItem(tr("Fir"), listWidget);
    new QListWidgetItem(tr("Pine"), listWidget);
    QListWidgetItem *Litm=new QListWidgetItem("lwi1");
    Litm->setIcon(QIcon(RESPATH+"/res/uOnline.png"));
    listWidget->addItem(Litm);
    Litm->setFlags(Litm->flags() ^ (Qt::ItemIsDropEnabled));
    Litm->setCheckState(Qt::Unchecked);
    listWidget->setDragEnabled(true);
    listWidget->setAcceptDrops(true);
    listWidget->viewport()->setAcceptDrops(true);
    listWidget->show();
    listWidget->setDefaultDropAction(Qt::MoveAction);
    listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    */
    this->setWindowTitle(RESPATH);
    MainImage->RepaintWidgetRect(MainImage->rect());
    QuickPanel->UpdateBG();

    SwitchPinPanels();

}

void MHPwindow::SetAppMetaInfo() {
    qRegisterMetaType<ActionData>("ActionData");
    QApplication::setApplicationVersion("v1.01");
    setWindowTitle(QApplication::applicationVersion());
    QCoreApplication::setOrganizationName("Repaint");
    QCoreApplication::setOrganizationDomain("Repaint.me");
    QCoreApplication::setApplicationName("Repaint");
    QCoreApplication::setApplicationVersion("Release 1.0");
    AppSettings = new QSettings("Repaint.ini", QSettings::IniFormat); //, QSettings::UserScope,
//  "Repaint", "username maybe"); // Eyelips

}
// EOI

void MHPwindow::ShowQP()
{

    // MainImage->uptimer->stop();

    // MainImage->setAttribute(Qt::WA_OpaquePaintEvent,false);
    //    MainImage->repainted=false;
    MainImage->RepaintWidgetRect(MainImage->rect());
    MainImage->update();
    // MainImage->setAttribute(Qt::WA_Disabled);
    MainImage->SetLock(1);
    QuickPanel->appear();
    QuickPanel->show();

    foreach (QWidget *w, AllPanels)
    {
        w->show();
        w->raise();
        w->setAutoFillBackground(true);
    }
    MainImage->GenAllThumbs();
    this->activateWindow();
}

void MHPwindow::HideQP()
{
    MainImage->RepaintWidgetRect(MainImage->rect());
    //   MainImage->uptimer->start();
    MainImage->SetLock(-1);
    QuickPanel->vanish();
    QuickPanel->hide();
    foreach (QWidget *w, AllPanels)
    {
        w->hide();
    }
    this->activateWindow();
    // this->grabKeyboard();
}

void MHPwindow::RealignQP()
{
}

void MHPwindow::SendMsg()
{
    //    QByteArray buf("ada");
    //  TcpClient->write(buf);
}

void MHPwindow::GetAction(ActionData act)
{
    if (!Dedicated)
    {

        // used for receiving actions from network!
        ARTM->ExecAction(&MainImage->ViewCanvas[act.layer], act, false);
    }
    // LOGM->AddAct(act);
    if (!ARTM->isRunning())
        ARTM->start(QThread::NormalPriority);

    MainImage->update();
}
void MHPwindow::ConnectAddr()
{
    NET->ConnectToServer(NetControls->EdTargetIP->text());
}

void MHPwindow::GetTStroke(d_Stroke Strk, d_StrokePars stpars)
{
    /*
// get stroke from local user
Strk.packpos1.SetByQPointF(Strk.pos1);
Strk.packpos2.SetByQPointF(Strk.pos2);
Strk.pos1=Strk.packpos1.ToPointF();
Strk.pos2=Strk.packpos2.ToPointF();
    ActionData nact=BControls->ParseBrush(Strk,stpars);
    if (stpars.Pars[csERASER]==1) nact.Brush.BlendMode=1;
    nact.layer=LayersPanel->GetActiveLayer();

//    LOGM->AddAct(nact);
    if (LayersPanel->BtnPresOp->isChecked()) nact.Brush.preserveop=1;
    else nact.Brush.preserveop=0;

     if (!Dedicated){
        //ARTM->ExecAction(&(MainImage->ViewCanvas[nact.layer]),nact,true);
        StrokeSection sect;
        sect.BrushFrom=nact.Brush;
        sect.Brush=nact.Brush;
        sect.layer=nact.layer;
        sect.spacing=2;
        //sect.spacing_offset=0;
        sect.ToolID=nact.ToolID;
        sect.Stroke=nact.Stroke;

        sect.Stroke.pos2=lastpos;
         lastpos=CalcLastPos(sect);

        StrokeMaster->ExecSection(sect,true);
        ChatLine->setText(QString::number(StrokeMaster->offset));
        Chat->append("ugjgjf");
     }
     else


       NET->GetAction(nact);

    ChatLine->setText(QString::number(stpars.Pars[PenPressure]));

*/
}
void MHPwindow::GetPoly(QPolygonF Poly)
{
    QPainter Pnt(&MainImage->ViewCanvas[LayersPanel->GetActiveLayer()]);
    // QPainter Pnt(&ViewCanvas[0]);
    Pnt.setPen(Qt::NoPen);
    Pnt.setBrush(QColor::fromHslF(BControls->CtlHue->GetValue(),
                                  BControls->CtlSat->GetValue(),
                                  BControls->CtlLit->GetValue()));
    Pnt.setOpacity(BControls->CtlOp->GetValue());
    //  Pnt.setOpacity(1);
    Pnt.drawPolygon(Poly, Qt::WindingFill);
    MainImage->RepaintImgRect(Poly.boundingRect().toRect());
}

void MHPwindow::GetMsg(QString msg)
{
    Label1->setText(msg);
}
void MHPwindow::GetServIp(QString msg)
{
    LblServIp->setText(msg);
}
void MHPwindow::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();
    float k;
    if (delta > 0)
        k = 1.05;
    if (delta < 0)
        k = 0.95;
    MainImage->ZoomK *= k;
    //    int dx=MainImage->geometry().width()*0.5;
    //    int dy=MainImage->geometry().height()*0.5;
    MainImage->RepaintWidgetRect(MainImage->rect());
    MainImage->update();
}
void MHPwindow::keyReleaseEvent(QKeyEvent *event)
{
    KBLINK->UnPressKey(event);
    /*
     if (!FastPanelLeft->isHidden()) {
     //    HideQP();
     MainImage->RepaintWidgetRect(MainImage->rect());
     }
     */
}

void MHPwindow::ConfirmAct(ActionData act)
{
    NET->GetAction(act);
}

void MHPwindow::ExecLayerAction(LayerAction lact)
{
    // filter mode for layers;
    if (NET->NetMode == emNone)
    {
        if (!Dedicated)
            ActionExecutor->ExecLayerAction(lact);
        // net send layer action
    }
    else
        NET->C_SendLaction(lact);

    // LOGM->AddAct(lact);
}

void MHPwindow::keyPressEvent(QKeyEvent *event)
{

    qint64 tmp = KBLINK->Compose(event->modifiers(), event->key());
    Label1->setText(QString::number(tmp));
    KBLINK->PressKey(event);

    if (KBLINK->KBstate[ekCPICK] == true)
    {
        // if (DlgCol->isVisible()) { DlgCol->hide();
        //   this->releaseKeyboard();
    }
    /*   else {

           QPoint p=QCursor::pos();
           int rx=p.x()-DlgCol->height()-(DlgCol->height()-DlgCol->width())*0.5;
           int ry=p.y()-DlgCol->height()*0.5;
                   rx=qMax(rx,0);
                   ry=qMax(ry,0);
                   QDesktopWidget screen;
                  rx=qMin(screen.availableGeometry().width()-DlgCol->width(),rx);
                  ry=qMin(screen.availableGeometry().height()-DlgCol->height(),ry);
           p.setX(rx);
           p.setY(ry);

           CtlCol->SetColor(QColor::fromHslF(
                        BControls->CtlHue->GetValue()
                        ,
                        BControls->CtlSat->GetValue()
                        ,
                        BControls->CtlLit->GetValue()
                        ));

           CtlCol->DrawSliders(CtlCol->LColor);
           DlgCol->setGeometry(QRect(p,QSize(DlgCol->width(),DlgCol->height())));
           DlgCol->show();
 //  DlgCol->pos()=QPoint(90,90);//QCursor::pos();



   }*/
    else if (KBLINK->KBstate[ekBrushSizeP] == true)
    {
        Brush->rad_out += 0.1;
        Brush->rad_in += 0.1;
    }
    else if (KBLINK->KBstate[ekBrushSizeM] == true)
    {

        Brush->rad_out -= 0.1;
        Brush->rad_in -= 0.1;
    }
    if (event->key() == Qt::Key_BraceLeft)
    {
        BControls->CtlCrv->ApplyValue(BControls->CtlCrv->GetValue() - 0.01);
    }
    else if (event->key() == Qt::Key_BraceRight)
    {
        BControls->CtlCrv->ApplyValue(BControls->CtlCrv->GetValue() + 0.01);
    }
    else if (KBLINK->KBstate[ekLitP] == true)
    {

        BControls->CtlLit->SetValF(BControls->CtlLit->GetValue() + 0.01);
    }
    else if (KBLINK->KBstate[ekLitM] == true)
    {

        BControls->CtlLit->SetValF(BControls->CtlLit->GetValue() - 0.01);
    }
    else if (KBLINK->KBstate[ekHueP] == true)
    {
        float reshue = BControls->CtlHue->GetValue() + 0.005;
        reshue = reshue - floor(reshue);
        //        Brush->col.setHslF(reshue,Brush->col.hslSaturationF(),Brush->col.valueF());
        BControls->CtlHue->SetValF(reshue);
    }
    else if (KBLINK->KBstate[ekFastBrush] == true)
    {
        // please return fast brush panel
        MainImage->EndPainting();
        if (QuickPanel->isHidden())
        {
            ShowQP();
        }
        else
            HideQP();
        QuickPanel->RealignPanel();
    }
    else if (KBLINK->KBstate[ekHueM] == true)
    {
        float reshue = BControls->CtlHue->GetValue() - 0.005;
        reshue = reshue - ceil(reshue - 1);
        // if (reshue<0) reshue+=1;
        // Brush->col.setHslF(reshue,Brush->col.hslSaturationF(),Brush->col.valueF());
        BControls->CtlHue->SetValF(reshue);
    }
    else if (KBLINK->KBstate[ekActualPixels] == true)
    {
        MainImage->ActualPixels();
    }
    else if (KBLINK->KBstate[ekTouchScreen] == true)
    {
        MainImage->TouchScreen();
    }
    else if (KBLINK->KBstate[ekFitScreen] == true)
    {
        MainImage->FitScreen();
    }

    else if (KBLINK->KBstate[ekPanelhide] == true)
    {
        /*
            QList <int> ZTMsizes;
        ZTMsizes.append(0);
        ZTMsizes.append(1000);
        ZTMsizes.append(0);
    */

        if (PansHidden)
        {
            this->setGeometry(MainGeometry);
            this->setWindowState(Qt::WindowNoState);
            // MCSplitter->widget(0)->show();
            // MCSplitter->widget(2)->show();

            MainImage->RepaintWidgetRect(MainImage->rect());
            MainImage->update();
            PansHidden = false;
        }
        else
        {
            // TMsizes=TMSplitter->sizes();
            // TMSplitter->setSizes(ZTMsizes);
            MainGeometry = this->geometry();
            this->setWindowState(Qt::WindowFullScreen);

            //   MCSplitter->widget(0)->hide();
            //  MCSplitter->widget(2)->hide();
            MainImage->RepaintWidgetRect(MainImage->rect());
            MainImage->update();

            PansHidden = true;
        }

        /*
        QList <int> MCsizes;
    MCsizes.append(912);
    MCsizes.append(128);
    MCSplitter->setSizes(MCsizes);
*/
    }
    else if (KBLINK->KBstate[ekTPresets] == true)
    {
        KBLINK->UnPressAll();
        BControls->BtnMasks->click();
    }

    else if (KBLINK->KBstate[ekSatP] == true)
        BControls->CtlSat->SetValF(BControls->CtlSat->GetValue() + 0.01);
    else if (KBLINK->KBstate[ekSatM] == true)
        BControls->CtlSat->SetValF(BControls->CtlSat->GetValue() - 0.01);
}
void MHPwindow::ImgOpen()
{
    QFileDialog::Options options;
    //    if (!native->isChecked())
    //      options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("QFileDialog::getOpenFileName()"),
                                                    "",
                                                    tr("eyeris layer image ELI PNG Files (*.ELI);;Portable Network Graphic (*.PNG);;JPG Files (*.JPG)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty())
    {
        Label1->setText(fileName);
    }
}

void MHPwindow::GetClient()
{
}
void MHPwindow::RedrawCol()
{
    // please
    QColor col = PaintColor->UseCol;
    float step = RngConv(M_PI / 6, 0, M_PI, 0, 1);
    BControls->CtlHue->Gslider->grad->setColorAt(0, QColor::fromHslF(QColor(Qt::red).hueF(), col.hslSaturationF(), col.lightnessF()));
    BControls->CtlHue->Gslider->grad->setColorAt(0 + step, QColor::fromHslF(QColor(Qt::yellow).hueF(), col.hslSaturationF(), col.lightnessF()));
    BControls->CtlHue->Gslider->grad->setColorAt(0 + step * 2, QColor::fromHslF(QColor(Qt::green).hueF(), col.hslSaturationF(), col.lightnessF()));
    BControls->CtlHue->Gslider->grad->setColorAt(0 + step * 3, QColor::fromHslF(QColor(Qt::cyan).hueF(), col.hslSaturationF(), col.lightnessF()));
    BControls->CtlHue->Gslider->grad->setColorAt(0 + step * 4, QColor::fromHslF(QColor(Qt::blue).hueF(), col.hslSaturationF(), col.lightnessF()));
    BControls->CtlHue->Gslider->grad->setColorAt(0 + step * 5, QColor::fromHslF(QColor(QColor::fromRgbF(1, 0, 1, 1)).hueF(), col.hslSaturationF(), col.lightnessF()));
    BControls->CtlHue->Gslider->grad->setColorAt(1, QColor::fromHslF(QColor(Qt::red).hueF(), col.hslSaturationF(), col.lightnessF()));

    BControls->CtlSat->Gslider->grad->setColorAt(0, QColor::fromHslF(col.hslHueF(), 0, col.lightnessF()));
    BControls->CtlSat->Gslider->grad->setColorAt(1, QColor::fromHslF(col.hslHueF(), 1, col.lightnessF()));

    BControls->CtlLit->Gslider->grad->setColorAt(0, QColor::fromHslF(0, 0, 0));
    BControls->CtlLit->Gslider->grad->setColorAt(0.5, QColor::fromHslF(col.hslHueF(), col.hslSaturationF(), 0.5));
    BControls->CtlLit->Gslider->grad->setColorAt(1, QColor::fromHslF(0, 0, 1));

    BControls->CtlSat->Gslider->Redraw();
    BControls->CtlHue->Gslider->Redraw();
    BControls->CtlLit->Gslider->Redraw();

    // QuickPanel->QuickHue->update();
}
void MHPwindow::GetColor(QColor col)
{ // get color from various sources ike canvas cpicker etc.
    PaintColor->SetCol(col);

    BControls->CtlHue->SetValF(col.hslHueF());
    BControls->CtlSat->SetValF(col.hslSaturationF());
    BControls->CtlLit->SetValF(col.lightnessF());
    RedrawCol();
    // CtlCol->SetColor(col);
    // CtlLit->SetValF(col.lightnessF());
}
void MHPwindow::SendChatMsg()
{
    QString Msg = CHAT->ChatLine->text();
    Msg.prepend(NET->LocalClient->RegName + ": ");
    Msg.append("\n");
    CHAT->ChatLine->clear();

    //    Chat->insertPlainText(Msg);
    //  Chat->autoFormatting();

    NET->GetChatMsg(Msg);
    // Chat->ensureCursorVisible();
}

void MHPwindow::GetLogin(QString user, QString pass)
{
    NET->LogIn(user, pass);
}
void MHPwindow::GrabKB()
{
    MainImage->setFocus();
}
void MHPwindow::RelKB()
{
    //    this->releaseKeyboard();
    //    Label1->setText("kb released");
}
void MHPwindow::SaveLogin()
{
    QString nval = "netlogn/" + NetControls->EdTargetIP->text();
    QString pval = "netlogp/" + NetControls->EdTargetIP->text();

    //    AppSettings->setValue(nval,DlgLogin->EdUsername->text());
    //  AppSettings->setValue(pval,DlgLogin->EdPassword->text());
    //  AppSettings->sync();
}

void MHPwindow::LoadLogin()
{
    QString nval = "netlogn/" + NetControls->EdTargetIP->text();
    QString pval = "netlogp/" + NetControls->EdTargetIP->text();

    // DlgLogin->EdUsername->setText(AppSettings->value(nval).toString());
    //  DlgLogin->EdPassword->setText(AppSettings->value(pval).toString());
}

void MHPwindow::OpenImage()
{

    QFileDialog::Options options;
    //    if (!native->isChecked())
    //      options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("QFileDialog::getOpenFileName()"),
                                                    "",
                                                    tr("All supported types (*.ELI *.Png *.Jpg *.RIL);;Repaint layer image ELI Files (*.ELI);;Portable Network Graphic (*.PNG);;JPG Files (*.JPG);;RePAINT Image Log file (*.RIL)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty())
    {
        {
            QString EXT = fileName.right(3);
            if (EXT.compare("ril", Qt::CaseInsensitive) == 0)
            {
                fileName.chop(4);
                // ActionExecutor->OpenLog((fileName));
            }
            else
            {
                WorkFilePath = fileName;
                //   MainImage->ViewCanvas[0].load(fileName);
                FileMenu->BtnSnap->setEnabled(true);
                FileMenu->BtnSave->setEnabled(true);
                FileMenu->BtnReload->setEnabled(true);
                MainImage->RepaintWidgetRect(MainImage->rect());
                MainImage->update();
                ActionExecutor->OpenImg(fileName);
            }
        }
    }
}

void MHPwindow::ReOpenImage()
{
    if (!WorkFilePath.isEmpty())
        /*
         MainImage->ViewCanvas[0].load(WorkFilePath);*/
        ActionExecutor->OpenImg(WorkFilePath);
    MainImage->RepaintWidgetRect(MainImage->rect());
    MainImage->update();
}
void MHPwindow::SnapImage()
{
    if (!WorkFilePath.isEmpty())
    {

        QString Nameonly;
        Nameonly.append(WorkFilePath);
        Nameonly.chop(4);
        QString Ext;
        Ext.append(WorkFilePath.right(4));

        SnapIdx++;
        QString Num = QString::number(SnapIdx);
        for (int i = Num.length(); i < 3; i++)
            Num.prepend("0");
        // QFile ftest;

        while (QFile(Nameonly + "_" + Num + Ext).exists() == true)
        {
            SnapIdx++;
            Num = QString::number(SnapIdx);
            for (int i = Num.length(); i < 3; i++)
                Num.prepend("0");
            // ftest.setFileName(Nameonly+"_"+Num+Ext);
        }

        MainImage->SaveImg(Nameonly + "_" + Num + Ext);
    }
}
void MHPwindow::SaveImg()
{
    if (!WorkFilePath.isEmpty())
        MainImage->SaveImg(WorkFilePath);
}
void MHPwindow::SaveImgAs()
{
    QFileDialog::Options options;

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("QFileDialog::getOpenFileName()"),
                                                    "",
                                                    tr("eyeris layer image ELI PNG Files (*.ELI);;Portable Network Graphic (*.PNG);;JPG Files (*.JPG)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty())
    {
        MainImage->ViewCanvas[0].save(fileName);

        WorkFilePath = fileName;
        FileMenu->BtnSnap->setEnabled(true);
        FileMenu->BtnSave->setEnabled(true);
        FileMenu->BtnReload->setEnabled(true);
        MainImage->SaveImg(fileName);
        // ActionExecutor->SaveLog(fileName);
    }
}
void MHPwindow::NewImage(QSize sz)
{
    FileMenu->BtnSnap->setEnabled(false);
    FileMenu->BtnSave->setEnabled(false);
    FileMenu->BtnReload->setEnabled(false);

    LayerAction lact;
    lact.ActID = laNewCanvas;
    lact.rect.setSize(sz);
    lact.layer = 1;
    ActionExecutor->ExecLayerAction(lact);
}

void MHPwindow::PasteImage()
{
    // please update button state depending on CB data.
    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard->image().isNull())
    {

        // QByteArray bytes;
        // QBuffer buffer(&bytes);
        // buffer.open(QIODevice::WriteOnly);
        // clipboard->image().save(&buffer, "PNG");
        QImage img(clipboard->image());

        QPainter Pnt(&MainImage->ViewCanvas[0]);
        Pnt.setCompositionMode(QPainter::CompositionMode_Source);
        Pnt.drawImage(0, 0, clipboard->image());
    }
}
void MHPwindow::LockCanvas(qint8 lk)
{
    /*
     MainImage->IsLocked=MainImage->IsLocked+lk;
      if (MainImage->IsLocked >0)
          ARTM->suspended=true;
      else
          ARTM->suspended=false;
          */
}
void MHPwindow::GrabImg(QString asker)
{
    QByteArray ELI(1, 'c');
    MainImage->WriteToBytes(&ELI);
    NET->SendImageData(asker, ELI);
}

void MHPwindow::ConfirmImage(QByteArray ba)
{
    MainImage->SetLock(-1);
    LayersPanel->SetLock(-1);
}
void MHPwindow::closeEvent(QCloseEvent *event)
{
    AppSettings->setValue(("MainGeo"), this->geometry());
    foreach (QWidget *w, AllPanels)
    {
        AppSettings->setValue((w->accessibleName()), w->geometry());
    }
}

void MHPwindow::SwitchPinPanels()
{
    int HideComp;
    int ShowComp;
    if (PanelsPinned)
    {
        foreach (QWidget *w, AllPanels)
        {
            //   w->setWindowFlags(Qt::Tool|Qt::CustomizeWindowHint|Qt::WindowMinimizeButtonHint);

            int newx = w->pos().x() + this->geometry().x();
            int newy = w->pos().y() + this->geometry().y();
            w->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint);
            w->hide();
            w->show();
            w->setGeometry(newx, newy, w->geometry().width(), w->geometry().height());
        }
    }
    else
    { // pinning panels back
        foreach (QWidget *w, AllPanels)
        {
            int newx = w->geometry().x() - this->geometry().x();
            int newy = w->geometry().y() - this->geometry().y();

            if ((newx + w->width()) > this->width())
                newx = this->width() - w->width();
            if (newy + w->height() > this->height())
                newy = this->height() - w->height();
            w->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
            w->hide();
            w->setGeometry(newx, newy, w->geometry().width(), w->geometry().height());
            w->show();
        }
    }
    PanelsPinned = !PanelsPinned;
    this->update();
    MainImage->RepaintWidgetRect(MainImage->rect());
}

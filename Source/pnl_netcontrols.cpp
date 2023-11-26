#include "pnl_netcontrols.h"
#include <QtWidgets/QDialog>
#include <QtWidgets/QInputDialog>

pnl_NetControls::pnl_NetControls(QWidget *parent)
{


    BtnAbout=new QPushButton(tr("About"));
    BtnConnect=new QPushButton(tr("Connect"));
    BtnLogin= new QPushButton("Log In");
    BtnNetStop= new QPushButton("Disconnect");
    BtnStartRoom= new QPushButton("Create Room");
    BtnPartRoom= new QPushButton("Leave Room");
    BtnSearch = new QPushButton("Find Friend");
    BtnLocal= new QPushButton("Local Mode");
    BtnStartServer=new QPushButton(tr("Start Server"));
    //BtnSearch = new QPushButton("Del Friend");


    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    BtnAbout->setSizePolicy(sizePolicy);
    BtnConnect->setSizePolicy(sizePolicy);
    BtnLogin->setSizePolicy(sizePolicy);
    BtnNetStop->setSizePolicy(sizePolicy);
    BtnStartServer->setSizePolicy(sizePolicy);
    BtnPartRoom->setSizePolicy(sizePolicy);
    BtnSearch->setSizePolicy(sizePolicy);
    BtnLocal->setSizePolicy(sizePolicy);
 //   BtnStartServer->styleSheet().setUnicode("")
    BtnNetStop->setFixedWidth(142);
    EdTargetIP=new QLineEdit;
    EdTargetIP->setAlignment(Qt::AlignCenter);
    EdTargetIP->setText("46.252.193.146");
    EdTargetIP->setFixedWidth(192);
//QFont lrgfont;
//lrgfont.setPointSize(16);
  //  BtnStartServer->setFont(lrgfont);
    Lt = new QHBoxLayout;
    Lt->addWidget(EdTargetIP);
    Lt->addWidget(BtnConnect);
    Lt->addWidget(BtnStartServer);
    Lt->addWidget(BtnNetStop);
    Lt->addStretch(0);
    Lt->addWidget(BtnAbout);
    //Lt->addWidget(BtnLocal);


    BtnNetStop->hide();
    setLayout2(Lt);
    Lt->setMargin(0);
    Lt->setSpacing(0);
    DlgAbout=new QDialog;
    QVBoxLayout *DAbt=new QVBoxLayout;
    Lb1=new QLabel;
    QPixmap pic1=QPixmap(RESPATH+"/res/eyeris.jpg");
    Lb1->setPixmap(pic1);
    DAbt->addWidget(Lb1);
    DAbt->setAlignment(DAbt,Qt::AlignTop);
    DAbt->setSpacing(0);
    DAbt->setMargin(0);
    DlgAbout->setLayout(DAbt);
    DlgAbout->setFixedWidth(pic1.width());
    DlgAbout->setFixedHeight(pic1.height());
    DlgAbout->setModal(true);
    //DlgCol->setMask(QRegion(0,0,0,0));
    //DlgCol->setGraphicsEffect();
    //DlgAbout->setWindowFlags(Qt::FramelessWindowHint);
    //DlgAbout->hide();
    DlgAbout->hide();

}
void pnl_NetControls::init(NetClient *net, NetServer *snet){
    NET=net;
    sNET=snet;
    connect(BtnAbout,SIGNAL(clicked()),DlgAbout,SLOT(show()));
    connect(BtnStartServer,SIGNAL(clicked()),sNET,SLOT(S_Start()));
    connect(BtnLocal,SIGNAL(clicked()),this,SLOT(ToLocal()));

    connect(BtnNetStop,SIGNAL(clicked()),NET,SLOT(N_Disconnect()));
    connect(BtnNetStop,SIGNAL(clicked()),sNET,SLOT(N_Disconnect()));

    connect(BtnConnect,SIGNAL(clicked()),this,SLOT(Cstart()));
    connect(BtnStartRoom,SIGNAL(clicked()),NET,SLOT(C_StartRoom()));
    connect(BtnPartRoom,SIGNAL(clicked()),NET,SLOT(C_PartRoom()));
    connect(BtnSearch,SIGNAL(clicked()),this,SLOT(SearchFriend()));
//    connect(BtnLogin,SIGNAL(clicked()),DlgLogin,SLOT(show()));
//---------
    connect(sNET,SIGNAL(SendServIp(QString)),this,SLOT(ToServer()));
    connect(sNET,SIGNAL(SendServIp(QString)),this,SLOT(Cloopback()));

    connect(NET,SIGNAL(LoginSuccess()),this,SLOT(ToClient()));
    connect(NET,SIGNAL(NetReset()),this,SLOT(ToNone()));

//    connect(NET,SIGNAL(ReqLogin(QString)),DlgLogin,SLOT(RequestLog(QString)));
//    connect(NET,SIGNAL(LoginS(QString)),FriendList,SLOT(AddUser(QString)));
//    connect(NET,SIGNAL(LoginSuccess()),DlgLogin,SLOT(hide()));

}


void pnl_NetControls::CreateRoom(){
    //NET->C;
}

void pnl_NetControls::Cstart(){
    NET->C_Connect(EdTargetIP->text());
    emit sendlock(1);
}
void pnl_NetControls::Cloopback(){
    NET->C_Connect("127.0.0.1");

}

void pnl_NetControls::ToClient(){
    EdTargetIP->setEnabled(false);
    BtnNetStop->setText("Disconnect");
    BtnConnect->hide();
    BtnStartServer->hide();
   // BtnLocal->hide();
    BtnNetStop->show();

}
void pnl_NetControls::ToServer(){
    EdTargetIP->setEnabled(false);
    BtnNetStop->setText("Kill Server");
    BtnConnect->hide();
    //BtnLocal->hide();
    BtnStartServer->hide();
    BtnNetStop->show();
}
void pnl_NetControls::ToLocal(){
    EdTargetIP->setEnabled(false);
    BtnNetStop->setText("Open up.");
    BtnConnect->hide();
    BtnStartServer->hide();
    //BtnLocal->hide();
    BtnNetStop->show();

}

void pnl_NetControls::ToNone(){
    EdTargetIP->setEnabled(true);
    //BtnLocal->show();
    BtnConnect->show();
    BtnStartServer->show();
    BtnNetStop->hide();
//    NET->N_Disconnect();
//    sNET->N_Disconnect();
    emit sendlock(-1);

}
void pnl_NetControls::SearchFriend(){
     bool ok;
    QString iname=QInputDialog::getText(this, tr("Search for a hand"),
                                              tr("Name:"), QLineEdit::Normal,
                          "Iris", &ok);
    if (ok){
        NET->C_FindFriend(iname);
    }
}



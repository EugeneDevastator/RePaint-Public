#include "ESwindow.h"
#include <QBoxLayout>
#include <QtGui>
#include <QString>
#include <QtPlugin>
#include <QTableView>
#include <QLabel>

MainWindow::MainWindow()
{
    Btn1 = new QPushButton("olol");
    LT = new QHBoxLayout;
    UTable = new QTableView;
    //      UBase= QSqlDatabase::addDatabase("QSQLITE");
    TestLBL = new QLabel("LABELTEXT");
    NET = new c_Netmaster;
    TextView = new QTextEdit;

    LT->addWidget(Btn1);
    LT->addWidget(TestLBL);
    LT->addWidget(TextView);

    this->setLayout(LT);
    DBK = new DBKeeper;
    connect(DBK, SIGNAL(SendMsg(QString)), this, SLOT(GetMsg(QString)));
    connect(DBK, SIGNAL(FailLogin(c_rsock *)), NET, SLOT(S_SendFail(c_rsock *)));
    connect(DBK, SIGNAL(FailReg(c_rsock *)), NET, SLOT(S_SendFail(c_rsock *)));

    connect(NET, SIGNAL(SendStatus(QString)), this, SLOT(GetMsg(QString)));
    // connect (NET,SIGNAL(SendLog(c_rsock*)),DBK,SLOT(TryLog(c_rsock*)));
    // connect (NET,SIGNAL(SendReg(c_rsock*)),DBK,SLOT(TryReg(c_rsock*)));
    connect(NET, SIGNAL(SendAuth(quint8, c_rsock *)), DBK, SLOT(TryAuthSQL(quint8, c_rsock *)));

    connect(NET, SIGNAL(NeedFriends(long)), DBK, SLOT(GatherFriends(long)));
    connect(NET, SIGNAL(DBFindUser(QString, c_rsock *)), DBK, SLOT(DBFindFriends(QString, c_rsock *)));
    connect(NET, SIGNAL(DBAddFriend(QString, c_rsock *)), DBK, SLOT(DBAddFriend(QString, c_rsock *)));
    connect(NET, SIGNAL(DBDelFriend(QString, c_rsock *)), DBK, SLOT(DBDelFriend(QString, c_rsock *)));

    connect(DBK, SIGNAL(GoodLogin(c_rsock *)), NET, SLOT(S_RegisterSock(c_rsock *)));
    connect(DBK, SIGNAL(GoodReg(c_rsock *)), NET, SLOT(S_RegisterSock(c_rsock *)));
    connect(DBK, SIGNAL(GoodAuth(c_rsock *)), NET, SLOT(S_RegisterSock(c_rsock *)));
    connect(DBK, SIGNAL(SendFlists(long, QList<long>, QList<QString>)), NET, SLOT(S_GetFriends(long, QList<long>, QList<QString>)));
    connect(DBK, SIGNAL(SendPeople(QStringList, c_rsock *)), NET, SLOT(S_GetPeople(QStringList, c_rsock *)));
    connect(DBK, SIGNAL(SendNewFriend(QString, c_rsock *)), NET, SLOT(S_FoundFriend(QString, c_rsock *)));
    connect(DBK, SIGNAL(SendDeadFriend(QString, c_rsock *)), NET, SLOT(S_DeleteFriend(QString, c_rsock *)));

    // DBK->ReadBase();

    DBK->ReadBase();
    NET->SG_Start();

    TextView->append("Server Started");
}

MainWindow::~MainWindow()
{
}

void MainWindow::GetMsg(QString msg)
{
    TextView->append(msg);
    // TestLBL->setText(msg);
}

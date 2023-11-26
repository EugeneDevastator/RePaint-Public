#include "NetClient.h"

#include "iostream"
#include <QtWidgets/QApplication>
#include <signal.h>
#include <QtGui>

// ------------------ N E T M A S T E R    MAIN METHODS -----------------
// ----------------------------------------------------------------------

NetClient::NetClient(QWidget *parent)
{
    MainPort = 33789;
    StartSession();
    NetMode = emNone;
    // LClient =new QTcpSocket;
    // LServer =new QTcpServer;
}

void NetClient::StartSession()
{

    // NetMode=eNone;
    QNetworkConfigurationManager manager;
    QNetworkConfiguration config = manager.defaultConfiguration();
    NetSession = new QNetworkSession(config);
    NetSession->open();

    LClient = new NetSocket;
}

// -------------------------- C L I E N T functions ------------------------
// -------------------------------------------------------------------------

void NetClient::C_Connect(QString addr)
{
    if (NetMode == emNone)
    {
        //   LClient->Sck->close();
        //   delete LClient;
        LClient = new NetSocket;
        QHostAddress haddr;
        haddr.setAddress(addr);
        LClient->Sck->connectToHost(haddr, MainPort);
        connect(LClient->Sck, SIGNAL(connected()), this, SLOT(C_ConnectSuccess()));
        connect(LClient, SIGNAL(RSDisconnect(NetSocket *)), this, SLOT(C_Suicide(NetSocket *)));
        connect(LClient->Sck, SIGNAL(disconnected()), this, SLOT(N_Disconnect()));
    }
}

void NetClient::C_Suicide(NetSocket *sock)
{
    N_Disconnect();
}

void NetClient::C_StartRoom()
{
    G_SendData(sdRoomCreate, QByteArray(1, 'R'), LClient);
}

void NetClient::C_ConnectSuccess()
{

    NetMode = emClient;
    // SendStatus("connected to :"+addr);
    //     connect(LClient,SIGNAL(RSAction(d_Action)),this,SLOT(C_GetAction(d_Action)));
    // connect(LClient,SIGNAL(SendDataHid(QString,QByteArray)),this,SLOT(G_ExecData(QString,QByteArray)));
    connect(LClient, SIGNAL(SendDataObj(stNetHead, QByteArray, NetSocket *)), this, SLOT(G_ExecData(stNetHead, QByteArray, NetSocket *)));
    LClient->init();
    LClient->Registered = true;
}

void NetClient::C_Login(QString Uname, QString Upass)
{

    sAuth auth;
    auth.uname = Uname;
    auth.upass = Upass;
    auth.aType = atLogin;
    G_SendData(sdAuth, auth.Serialize(), LClient);
}

void NetClient::C_Regin(QString Uname, QString Upass)
{
    sAuth auth;
    auth.uname = Uname;
    auth.upass = Upass;
    auth.aType = atRegister;
    G_SendData(sdAuth, auth.Serialize(), LClient);
}

void NetClient::C_SendLaction(d_LAction la)
{
    G_SendData(sdLAction, la.Serialize(), LClient);
}

void NetClient::C_JoinRoom(QString uname)
{
    if (uname.compare(LClient->RegName) != 0)
    {
        LClient->inroom = false;
        G_SendData(sdRoomJoin, SZstring(uname), LClient);
    }
}

void NetClient::C_PartRoom()
{
    LClient->inroom = false;
    G_SendData(sdRoomPart, QByteArray('z', 1), LClient);
}
void NetClient::C_FindFriend(QString name)
{
    G_SendData(sdFriendSeek, SZstring(name), LClient);
}
void NetClient::C_AddFriend(QString name)
{
    G_SendData(sdFriendAdd, SZstring(name), LClient);
}
void NetClient::C_DelFriend(QString name)
{
    G_SendData(sdFriendDel, SZstring(name), LClient);
}

void NetClient::C_SendImage(QString asker, QByteArray ELI)
{
    QByteArray *ba = new QByteArray;
    QDataStream ds(ba, QIODevice::ReadWrite);
    ds << asker;
    ds << ELI;
    G_SendData(sdGetImg, *ba, LClient);
    delete ba;
}

// --------------------- C-S SHARED FUNCTIONS -----------------
// ------------------------------------------------------------

void NetClient::G_ExecData(stNetHead HEAD, QByteArray data, NetSocket *dest)
{
    qDebug() << "size: " + QString::number(data.size()) + "head id:" + QString::number(HEAD.Hid);
    if (HEAD.Hid == sdLogin)
    { // user is connected and i send him request for authentification;
        emit ReqLogin(DSZstring(data));
    }

    else if (HEAD.Hid == sdLoginS)
    { // login success, you get this when you are connected;
        emit LoginS(DSZstring(data));
        emit LoginSuccess();
        LClient->RegName = DSZstring(data);
    }

    else if (HEAD.Hid == sdAction)
    {
        d_Action act;
        act.DeSerialize(data);
        emit SendAction(act);
    }
    else if (HEAD.Hid == sdSection)
    {
        d_Section sect;
        sect.DeSerialize(data);
        emit SendSection(sect);
    }
    else if (HEAD.Hid == sdGetMsg)
    {
        emit SendChatMsg(DSZstring(data));
    }

    else if (HEAD.Hid == sdUserAdded)
    { // user added, everyone gets this when someone connected
        emit LoginS(DSZstring(data));
    }

    else if (HEAD.Hid == sdUserDel)
    {
        emit ClientEnd(DSZstring(data));
    }

    else if (HEAD.Hid == sdGetPass)
    {
        // here should be performed check if user was registered, and also check for if it is a new registration
    }

    else if (HEAD.Hid == sdUserStat)
    { // user status
        // QString dname=DSZstring(data);
        // emit SendUserStatus(dname.left(dname.length()-1),(unsigned char)data.at(data.length()-1));
        stUserState us;
        us.DeSerialize(data);
        emit SendUserStatus(us.Uname, us.Ustate);
    }
    else if (HEAD.Hid == sdRoomJoin)
    { // user status

        QString dname = DSZstring(data);
        // emit SendUserRJ(dnamet(dname.length()-1),(unsigned char)data.at(data.length()-1));
        emit SendUserRJ(dname, 0);
        // please separate it :
        // emit SendUserStatus(dname.left(dname.length()-1),(unsigned char)data.at(data.length()-1));
    }
    else if (HEAD.Hid == sdRoomPart)
    {
        QString dname = DSZstring(data);
        emit SendPartRoom(dname);
    }
    else if (HEAD.Hid == sdLAction)
    {

        /*
        d_LAction *GotAction=new d_LAction;
        QByteArray ReadBuf(data);//(this->Sck->read(sizeof(d_Action)));
        for (unsigned int i = 0; i<sizeof(d_LAction); i++) {
            // note this can be optimized to pointer switching
            ((unsigned char*)GotAction)[i]=(unsigned char)ReadBuf.at(i);
            }

        //
        emit SendLAction(*GotAction);
        delete GotAction;
        */
        d_LAction lact;
        lact.DeSerialize(data);
        emit SendLAction(lact);
    }
    else if (HEAD.Hid == sdLock)
    {
        emit LockCanvas(1);
    }
    else if (HEAD.Hid == sdUnlock)
    {
        emit LockCanvas(-1);
    }
    else if (HEAD.Hid == sdReqImg)
    {
        emit ReqImage(DSZstring(data));
    }
    else if (HEAD.Hid == sdPeopleResults)
    {
        emit SendPeopleList(DSZstring(data).split(","));
    }
    else if (HEAD.Hid == sdGetImg)
    {
        /*QDataStream ds(data,QIODevice::ReadWrite);
        QByteArray ELI;
        QString asker;
        ds >> asker;
        ds >> ELI;*/
        emit GetImage(data);
        // assume this blockk kwill be only executed on client;
        G_SendData(sdConfirmImg, QByteArray(1, 'c'), LClient);
    }
    else if (HEAD.Hid == sdFAIL)
    {
        emit NetReset();
    }
}

void NetClient::GetAction(d_Action st)
{
    G_SendData(sdAction, st.Serialize(), LClient);
}
void NetClient::GetSection(d_Section sect)
{
    if (NetMode != emNone)
        G_SendData(sdSection, sect.Serialize(), LClient);
}

void NetClient::GetChatMsg(QString msg)
{
    G_SendData(sdGetMsg, SZstring(msg), LClient);
}

void NetClient::N_Disconnect()
{
    if (NetMode != emNone)
    {
        NetMode = emNone;
        LClient->Sck->close();
        emit NetReset();
    }
}

// void NetClient::C_Stop(){
//     LClient->Sck->close();
// }

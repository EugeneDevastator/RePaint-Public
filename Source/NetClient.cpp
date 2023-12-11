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
    // LocalClient =new QTcpSocket;
    // LServer =new QTcpServer;
}

void NetClient::StartSession()
{

    // NetMode=eNone;
    QNetworkConfigurationManager manager;
    QNetworkConfiguration config = manager.defaultConfiguration();
    NetSession = new QNetworkSession(config);
    NetSession->open();

    LocalClient = new NetSocket;
}

// -------------------------- C L I E N T functions ------------------------
// -------------------------------------------------------------------------

void NetClient::ConnectToServer(QString addr)
{
    if (NetMode == emNone)
    {
        //   LocalClient->Sck->close();
        //   delete LocalClient;
        LocalClient = new NetSocket;
        QHostAddress haddr;
        haddr.setAddress(addr);
        LocalClient->Sck->connectToHost(haddr, MainPort);
        connect(LocalClient->Sck, SIGNAL(connected()), this, SLOT(OnConnectionSuccess()));
        connect(LocalClient, SIGNAL(RSDisconnect(NetSocket *)), this, SLOT(DisconnectFromServer(NetSocket * )));
        connect(LocalClient->Sck, SIGNAL(disconnected()), this, SLOT(N_Disconnect()));
    }
}

void NetClient::DisconnectFromServer(NetSocket *sock)
{
    N_Disconnect();
}

void NetClient::StartNewRoom()
{
    G_SendData(sdRoomCreate, QByteArray(1, 'R'), LocalClient);
}

void NetClient::OnConnectionSuccess()
{

    NetMode = emClient;
    // SendStatus("connected to :"+addr);
    //     connect(LocalClient,SIGNAL(RSAction(ActionData)),this,SLOT(C_GetAction(ActionData)));
    // connect(LocalClient,SIGNAL(SendDataHid(QString,QByteArray)),this,SLOT(G_ExecData(QString,QByteArray)));
    connect(LocalClient, SIGNAL(SendDataObj(stNetHead, QByteArray, NetSocket *)), this, SLOT(G_ExecData(stNetHead, QByteArray, NetSocket *)));
    LocalClient->init();
    LocalClient->IsRegistered = true;
}

void NetClient::LogIn(QString Uname, QString Upass)
{

    AuthPacket auth;
    auth.uname = Uname;
    auth.upass = Upass;
    auth.aType = atLogin;
    G_SendData(sdAuth, auth.Serialize(), LocalClient);
}

void NetClient::RegisterNewUser(QString Uname, QString Upass)
{
    AuthPacket auth;
    auth.uname = Uname;
    auth.upass = Upass;
    auth.aType = atRegister;
    G_SendData(sdAuth, auth.Serialize(), LocalClient);
}

void NetClient::C_SendLaction(LayerAction la)
{
    G_SendData(sdLAction, la.Serialize(), LocalClient);
}

void NetClient::JoinRoom(QString roomName)
{
    if (roomName.compare(LocalClient->RegName) != 0)
    {
        LocalClient->isInRoom = false;
        G_SendData(sdRoomJoin, SZstring(roomName), LocalClient);
    }
}

void NetClient::LeaveCurrentRoom()
{
    LocalClient->isInRoom = false;
    G_SendData(sdRoomPart, QByteArray('z', 1), LocalClient);
}
void NetClient::FindFriend(QString name)
{
    G_SendData(sdFriendSeek, SZstring(name), LocalClient);
}
void NetClient::AddFriend(QString name)
{
    G_SendData(sdFriendAdd, SZstring(name), LocalClient);
}
void NetClient::RemoveFriend(QString name)
{
    G_SendData(sdFriendDel, SZstring(name), LocalClient);
}

void NetClient::SendImageData(QString asker, QByteArray imageData)
{
    QByteArray *bytes = new QByteArray;
    QDataStream stream(bytes, QIODevice::ReadWrite);
    stream << asker;
    stream << imageData;
    G_SendData(sdGetImg, *bytes, LocalClient);
    delete bytes;
}

// --------------------- C-S SHARED FUNCTIONS -----------------
// ------------------------------------------------------------

void NetClient::G_ExecData(NetPacketHeader HEAD, QByteArray data, NetSocket *dest)
{
    qDebug() << "size: " + QString::number(data.size()) + "head id:" + QString::number(HEAD.Id);
    if (HEAD.Id == sdLogin)
    { // user is connected and i send him request for authentification;
        emit ReqLogin(DSZstring(data));
    }

    else if (HEAD.Id == sdLoginS)
    { // login success, you get this when you are connected;
        emit LoginS(DSZstring(data));
        emit LoginSuccess();
        LocalClient->RegName = DSZstring(data);
    }

    else if (HEAD.Id == sdAction)
    {
        ActionData act;
        act.DeSerialize(data);
        emit SendAction(act);
    }
    else if (HEAD.Id == sdSection)
    {
        StrokeSection sect;
        sect.DeSerialize(data);
        emit SendSection(sect);
    }
    else if (HEAD.Id == sdGetMsg)
    {
        emit SendChatMsg(DSZstring(data));
    }

    else if (HEAD.Id == sdUserAdded)
    { // user added, everyone gets this when someone connected
        emit LoginS(DSZstring(data));
    }

    else if (HEAD.Id == sdUserDel)
    {
        emit ClientEnd(DSZstring(data));
    }

    else if (HEAD.Id == sdGetPass)
    {
        // here should be performed check if user was registered, and also check for if it is a new registration
    }

    else if (HEAD.Id == sdUserStat)
    { // user status
        // QString dname=DSZstring(data);
        // emit SendUserStatus(dname.left(dname.length()-1),(unsigned char)data.at(data.length()-1));
        NetUserState us;
        us.DeSerialize(data);
        emit SendUserStatus(us.UserName, us.Ustate);
    }
    else if (HEAD.Id == sdRoomJoin)
    { // user status

        QString dname = DSZstring(data);
        // emit SendUserRJ(dnamet(dname.length()-1),(unsigned char)data.at(data.length()-1));
        emit SendUserRJ(dname, 0);
        // please separate it :
        // emit SendUserStatus(dname.left(dname.length()-1),(unsigned char)data.at(data.length()-1));
    }
    else if (HEAD.Id == sdRoomPart)
    {
        QString dname = DSZstring(data);
        emit SendPartRoom(dname);
    }
    else if (HEAD.Id == sdLAction)
    {

        /*
        LayerAction *GotAction=new LayerAction;
        QByteArray ReadBuf(data);//(this->Sck->read(sizeof(ActionData)));
        for (unsigned int i = 0; i<sizeof(LayerAction); i++) {
            // note this can be optimized to pointer switching
            ((unsigned char*)GotAction)[i]=(unsigned char)ReadBuf.at(i);
            }

        //
        emit SendLAction(*GotAction);
        delete GotAction;
        */
        LayerAction lact;
        lact.DeSerialize(data);
        emit SendLAction(lact);
    }
    else if (HEAD.Id == sdLock)
    {
        emit LockCanvas(1);
    }
    else if (HEAD.Id == sdUnlock)
    {
        emit LockCanvas(-1);
    }
    else if (HEAD.Id == sdReqImg)
    {
        emit ReqImage(DSZstring(data));
    }
    else if (HEAD.Id == sdPeopleResults)
    {
        emit SendPeopleList(DSZstring(data).split(","));
    }
    else if (HEAD.Id == sdGetImg)
    {
        /*QDataStream ds(data,QIODevice::ReadWrite);
        QByteArray ELI;
        QString asker;
        ds >> asker;
        ds >> ELI;*/
        emit GetImage(data);
        // assume this blockk kwill be only executed on client;
        G_SendData(sdConfirmImg, QByteArray(1, 'c'), LocalClient);
    }
    else if (HEAD.Id == sdFAIL)
    {
        emit NetReset();
    }
}

void NetClient::GetAction(ActionData st)
{
    G_SendData(sdAction, st.Serialize(), LocalClient);
}
void NetClient::GetSection(StrokeSection sect)
{
    if (NetMode != emNone)
        G_SendData(sdSection, sect.Serialize(), LocalClient);
}

void NetClient::GetChatMsg(QString msg)
{
    G_SendData(sdGetMsg, SZstring(msg), LocalClient);
}

void NetClient::N_Disconnect()
{
    if (NetMode != emNone)
    {
        NetMode = emNone;
        LocalClient->Sck->close();
        emit NetReset();
    }
}

// void NetClient::C_Stop(){
//     LocalClient->Sck->close();
// }

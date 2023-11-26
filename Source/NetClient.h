#ifndef NETCLIENT_H
#define NETCLIENT_H


#include <QtWidgets/QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtNetwork>
#include <QDataStream>
#include "Brushes.h"
#include "Enums.h"
#include "NetSocket.h"

/*
QString DSZstring(QByteArray ba);
QByteArray SZstring(QString st);
*/
QT_BEGIN_NAMESPACE
class QWidget;


//-------------------  R E M O T E   S O C K E T  --------------------

//enum eHeads{
  //  hAction, hLAction, hLogin, hRegin, hSTOP
//};

//----------------------- N E T M A S T E R ------------------------

class NetClient : public QObject
{
   Q_OBJECT

public:

NetClient(QWidget *parent = 0);


ConnectionMode NetMode;



    NetSocket *LClient; // local client

    QNetworkSession *NetSession;

public slots:

    // global network functions
    void StartSession();
    //  void retr(QString msg); // unused
    void N_Disconnect();
    void G_ExecData(stNetHead HEAD, QByteArray data,NetSocket *dest);
    // input functions
    void GetAction(d_Action st); // slot to receive action from local canvas
    void GetSection(d_Section sect);
    void GetChatMsg(QString msg);

    // ------------ client functions- -----------
    void C_Connect(QString addr);
    void C_ConnectSuccess();
    void C_Login(QString Uname,QString Upass);
    void C_Regin(QString Uname,QString Upass);
 //  void C_Send(d_Action st); // send action client to server.
    void C_SendLaction(d_LAction st);
    void C_Suicide(NetSocket* sock);
    void C_StartRoom();
    void C_JoinRoom(QString uname);
    void C_PartRoom();
    void C_FindFriend(QString name);
    void C_AddFriend(QString name);
    void C_DelFriend(QString name);
    void C_SendImage(QString asker,QByteArray ELI);



signals:
    // outside connection functions
    void SendAction(d_Action st); // signal to send action to local canvas
    void SendSection(d_Section sect);
    void SendStatus(QString msg);

    void SendChatMsg(QString msg);
    void ReqLogin(QString msg);  // request for username and pass
    void LoginS(QString data); //successful login maybe you or any other.

    void SendLog(NetSocket *client);
    void SendReg(NetSocket *client);
    void SendAuth(quint8 aType,NetSocket *client);

    void SendUserStatus(QString name,int status);
    void SendUserRJ(QString name,int status);
    void SendPartRoom(QString name);

    void SendPeopleList(QStringList stl);

    void SendLayerAdd(int at);
    void SendLayerProp(int at,float op,int bm);
    void SendLayerDel(int at);
    void SendLayerMove(int at,int to);
    void SendLAction(d_LAction lact);

    void LoginSuccess();
    void NetReset();

    void LockCanvas(qint8);
    void ReqImage(QString asker);
    void GetImage(QByteArray ELI);


    void ClientEnd(QString data); //successful login maybe you or any other.


private:
   int MainPort;



};



#endif // NETCLIENT_H

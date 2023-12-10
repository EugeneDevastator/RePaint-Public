#ifndef NETSERVER_H
#define NETSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtNetwork>
#include <QDataStream>
#include "Brushes.h"
#include "Enums.h"
#include "NetSocket.h"
#include "SharedNetCode/NetStructures.h"

class NetServer : public QObject
{
    Q_OBJECT
public:
    explicit NetServer(QObject *parent = 0);

    bool GlobalRoomEnabled;


       ConnectionMode ServMode;
       ConnectionMode NetMode;
       QTcpServer *LServer; // local server
       QList<NetSocket *> RSockClientList; // remote clients
       QHash<long,NetSocket*> GuidClientList;
       QHash<QString,NetSocket*> NameClientList;
       QNetworkSession *NetSession;

signals:
           void SendServIp(QString msg);
           void SendStatus(QString msg);
           void LoginS(QString data);
           void NeedFriends(long guid);
           void SendAuth(quint8 aType,NetSocket *client);
           void DBFindUser(QString name,NetSocket *caller);
           void DBDelFriend(QString name,NetSocket *caller);
           void DBAddFriend(QString name,NetSocket *caller);
           void ClientEnd(QString data); //successful login maybe you or any other.
           void ServerOff();

public slots:
    void StartSession();
           //  void retr(QString msg); // unused
    void S_Disconnect();
    void S_Start();
    void S_GetClient();
    void S_Retranslate(stNetHead HEAD,QByteArray fulldata,NetSocket *caller);
    void S_ResendToRest(stNetHead HEAD,QByteArray fulldata,NetSocket *caller,bool inroom);
    void S_ResendToAll(stNetHead HEAD,QByteArray fulldata,NetSocket *caller,bool inroom);
    void S_UpdateUserStatus(NetSocket *user,quint8 status);

    void S_RegisterSock(NetSocket *sock);
    void S_SendFail(NetSocket *sock);
    void SG_Start();
    void S_AddToRoom(NetSocket *client, QString rname);
    void S_DelFromRoom(NetSocket *client);
    void S_SetUserStatus(NetSocket *client,int status);

    void S_KillClient(NetSocket *caller);
    void S_Stop();
 //   void S_SendAll(d_Action st);
 //   void S_SendAll(quint8 Hid,QByteArray data);

    void S_GetFriends(long DGUID,QList <long> fguids,QList <QString> fnames);
    void S_GetPeople(QStringList pList,NetSocket *asker);
    void S_FoundFriend(QString name,NetSocket *caller);
    void S_DeleteFriend(QString name,NetSocket *caller);


   QString GenForumHash(QString uname, QString upass);
private:
int MainPort;

};

#endif // NETSERVER_H

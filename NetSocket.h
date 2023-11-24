#ifndef NETSOCKET_H
#define NETSOCKET_H

#include <QObject>
#include <QtWidgets/QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtNetwork>
#include <QDataStream>
#include "Brushes.h"
#include "Enums.h"

enum ConnectionMode {
    emServer,
    emGServer,
    emClient,
    emNone
};

enum dStates{
    sdNONE,         //0
    sdAction,       //AA 1
    sdSection,   //2
    sdLAction,      //LA
    sdGetName,      //GN
    sdGetPass,      //GP
    sdGetMsg,       //CM 5
    sdLogin,        //SL
    sdLoginS,       //LS 7
    sdUserAdded,    //UA    like saying hello to everyone
    sdUserDel,      //DU    Goodbye everyone;
    sdUserStat,     //US 10 status, "he everyone, i a killer"
    sdRoomJoin,     //RJ
    sdRoomPart,     //RP
    sdRoomCreate,   //RC
    sdAuth,         //    new one for combined auth;
    sdReg,          // RR
    sdLog,          // LL 15
    sdFriendSeek,    // FS
    sdFriendAdd,
    sdFriendDel,    //FD
    sdLock,
    sdUnlock,
    sdGetImg,
    sdReqImg,
    sdConfirmImg,
    sdPeopleResults,


    sdMisc,
    sdFAIL,
    sdSTOP
};

//----------------- STRUCTURES

struct sAuth{
    QString uname;
    QString upass;
    quint8 aType; //auth type;

    QByteArray Serialize();
    sAuth DeSerialize(QByteArray ba);
    };


struct stNetHead{
    quint8 Hid;
    quint32 Hsize;

    quint8 GetSSize();
    void Serialize(QDataStream *ds);
    void DeSerialize(QDataStream *ds);

    };

struct stUserState{
    quint8 Ustate;
    QString Uname;
    QByteArray Serialize();
    void DeSerialize(QByteArray ba);
};

class NetSocket;

class NetRoom :public QObject
{
    Q_OBJECT
public:
    NetRoom();
    NetSocket *server;
    NetSocket *imageowner;
    QList <NetSocket*> RCList;
public slots:
    void AddUser(NetSocket *client);
    void DelUser(NetSocket *client);

};



class NetSocket : public QObject
{
    Q_OBJECT
public:
    explicit NetSocket(QObject *parent = 0);
    
    int HeadSize;
    stNetHead HEAD;

    qint32 RDataSize;
    int Role;
    int uStatus;
    int DataState;
    long UGUID;
    bool Registered;
    bool inroom;
    bool blocker;
    bool reading;
    bool imageowner;

    QString RegName;
    QString RegPass;
    QString Hid;

    int UserIdx;
    int RoomIdx;
    QByteArray DataBuffer;
    QByteArray HeadBuffer;

    QByteArray SavedStream;
  //  QByteArray FullBuffer;
    QDataStream *SockStream;

    NetRoom *Room;
    QList <long> FGuids;


    QTcpSocket *Sck;
    int listIdx;

public slots:
    void ParseData(); // get data from connection
    void Terminate(); // get data from connection
                      // get data from connection
   void init();
signals:
    void RSAction(d_Action st); //send parsed data outside
    void RSActionObj(d_Action st,NetSocket *caller);
    void RSDisconnect(NetSocket *caller);
    void SendMsg(QString msg);
    //void SendDataHid(QString hid,QByteArray data);
    void SendDataObj(stNetHead HEAD,QByteArray fulldata,NetSocket *caller);
    void TryLogin(NetSocket *caller);

};

void G_SendData(quint8 Hid,QByteArray data,NetSocket *sock);
void G_SendUserStatus(NetSocket* user,NetSocket *dest);
void G_SendUserStatus(QString uname,int status,NetSocket *dest);

#endif // NETSOCKET_H

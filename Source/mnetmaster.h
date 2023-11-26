#ifndef MNETMASTER_H
#define MNETMASTER_H

#include <QWidget>
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

enum eHeads{
    hAction, hLAction, hLogin, hRegin, hSTOP
    };

enum dStates{
    sdNONE,
    sdAction,       //AA 1
    sdSection,
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

struct stNetHead{
    quint8 Hid;
    quint32 Hsize;
    quint8 GetSSize();
    void Serialize(QDataStream *ds);
    void DeSerialize(QDataStream *ds);
    };


struct sAuth{
    QString uname;
    QString upass;
    quint8 aType; //auth type;
    QByteArray Serialize();
    sAuth DeSerialize(QByteArray ba);
    };

struct stUserState{
    quint8 Ustate;
    QString Uname;
    QByteArray Serialize();
    void DeSerialize(QByteArray ba);
};

class NetRoom;
class c_rsock : public QObject  // description of remote socket, needed for socket list.
{
   Q_OBJECT

public:
    c_rsock();

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
    void RSActionObj(d_Action st,c_rsock *caller);
    void RSDisconnect(c_rsock *caller);
    void SendMsg(QString msg);
    //void SendDataHid(QString hid,QByteArray data);
    void SendDataObj(stNetHead HEAD,QByteArray fulldata,c_rsock *caller);
    void TryLogin(c_rsock *caller);
};


class NetRoom :public QObject
{
    Q_OBJECT
    public:
        NetRoom();
        c_rsock *server;
        QList <c_rsock*> RCList;
    public slots:
        void AddUser(c_rsock *client);
        void DelUser(c_rsock *client);

};



class mNetMaster : public QWidget
{
    Q_OBJECT
public:
    explicit mNetMaster(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // MNETMASTER_H

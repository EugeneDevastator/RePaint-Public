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



//----------------- STRUCTURES

struct AuthPacket {
   QString uname;
   QString upass;
   quint8 aType; //auth type;

   QByteArray Serialize();

   AuthPacket DeSerialize(QByteArray ba);
};

struct NetPacketHeader {
   quint8 Id;
   quint32 Hsize;

   quint8 GetSSize();

   void Serialize(QDataStream *ds);

   void DeSerialize(QDataStream *ds);
};

struct NetUserState {
   quint8 Ustate;
   QString UserName;
   QByteArray Serialize();
   void DeSerialize(QByteArray ba);
};

class NetSocket;

class NetRoom : public QObject {
   Q_OBJECT
   public:
      NetRoom();

      NetSocket *server;
      NetSocket *imageowner;
      QList<NetSocket *> RemoteClients;
   public slots:

      void AddUser(NetSocket *client);

      void DelUser(NetSocket *client);

};

class NetSocket : public QObject {
   Q_OBJECT
   public:
      explicit NetSocket(QObject *parent = 0);

      int HeadSize;
      NetPacketHeader HEAD;

      qint32 RDataSize;
      int Role;
      int uStatus;
      int DataState;
      long UGUID;
      bool IsRegistered;
      bool isInRoom;
      bool blocker;
      bool IsReading;
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
      QList<long> FGuids;

      QTcpSocket *Sck;
      int listIdx;

   public slots:

      void ParseData(); // get data from connection
      void Terminate(); // get data from connection
      // get data from connection
      void init();

   signals:

      void RSAction(ActionData st); //send parsed data outside
      void RSActionObj(ActionData st, NetSocket *caller);

      void RSDisconnect(NetSocket *caller);

      void SendMsg(QString msg);

      //void SendDataHid(QString hid,QByteArray data);
      void SendDataObj(NetPacketHeader HEAD, QByteArray fulldata, NetSocket *caller);

      void TryLogin(NetSocket *caller);

};

void G_SendData(quint8 Hid, QByteArray data, NetSocket *sock);

void G_SendUserStatus(NetSocket *user, NetSocket *dest);

void G_SendUserStatus(QString uname, int status, NetSocket *dest);

#endif // NETSOCKET_H

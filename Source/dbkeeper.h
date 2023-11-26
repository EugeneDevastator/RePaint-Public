#ifndef DBKEEPER_H
#define DBKEEPER_H

#include <QObject>
#include <QFile>
#include <QtSql>

#include "netmaster.h"

enum eUserStatus{
    sServer, sClient, sOn, sOff, sSTOP
};
enum SocialLinkType{
    slLocal, slSMF, slPHPSocial, slSTOP
};

typedef QList <long> LongList;

struct UserRec{
    long GUID;
    QString Name;
    QString Pass;
    QList <long> Friends;
    QList <QString> FriendNames;
    int Status;


};


class DBKeeper : public c_Netmaster
{
    Q_OBJECT
public:
    explicit DBKeeper(QObject *parent = 0);
    QList <UserRec> BASE;
    QFile *DBfile;
    QString DBPath;

    QSqlDatabase MainDB;
    QSqlTableModel *TableUsers;


    QString DBHost;
    QString DBName;
    QString DBLogin;
    QString DBPass;

    QString TLogins;
    QString TFriends;

    QString Flogin;
    QString FMainId;
    QString Fpass;
    QString Ffriends;
    QString FlinkedID;

    quint8 SLType;

signals:
    void SendMsg(QString msg);
    void GoodLogin(c_rsock *client);
    void GoodReg(c_rsock *client);
    void GoodAuth(c_rsock *client);
    void FailLogin(c_rsock *client);
    void FailReg(c_rsock *client);
    void SendFlists(long guid,QList <long> fguids,QList <QString> fnames);
    void SendNewFriend(QString name, c_rsock *caller);
    void SendDeadFriend(QString name, c_rsock *caller);
    void SendPeople(QStringList names, c_rsock *caller);
public slots:
    void ReadBase();
    void SaveBase();
  //  void TryLog(c_rsock *client);
  //  void TryReg(c_rsock *client);
    void TryAuth(quint8 atype,c_rsock *client);
    void TryAuthSQL(quint8 atype,c_rsock *client);
    void GatherFriends(long guid);
    void DBAddFriend(QString name, c_rsock *caller);
    void DBDelFriend(QString name,c_rsock *caller);
    void DBFindFriends(QString wc, c_rsock *caller);
    LongList OpenStringList(QString StList);
    QString FoldLongList(LongList lst);
    QList <QString> GenFnames(LongList lst);

};

#endif // DBKEEPER_H

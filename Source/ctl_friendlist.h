#ifndef CTL_FRIENDLIST_H
#define CTL_FRIENDLIST_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListView>
#include <QtWidgets/QVBoxLayout>
#include <QStandardItemModel>
#include <QtWidgets/QTreeView>
#include <QItemSelectionModel>
#include "Enums.h"
#include "NetClient.h"
#include "dlg_usersearch.h"
#include "bctl_widgetholder.h"

enum eUserStatus {
usOn, usOff, usBusy, usIris, usSTOP

};



class ctl_friendlist : public bctl_WidgetHolder
{
    Q_OBJECT
public:
    explicit ctl_friendlist(NetClient *net , QWidget *parent= 0);
    //QListView *FLIST;

    QTreeView *FLIST;
    QVBoxLayout *LT;
    QStandardItemModel *stimodel;
    QList <QStandardItem> stiusers;
    QStandardItemModel *roommates;
    QStandardItemModel *spectators;
    QStandardItemModel *friends;
    //QTreeModel *treemodel;
    QStandardItem *FriendRoot;
    QStandardItem *RoomRoot;
    QStandardItem *SpectRoot;
    QList <QIcon> sIcons;
    QString *LocalName;
    QItemSelectionModel *selmodel;

    QMenu *MnuPopup;

    dlg_UserSearch *DlgFind;

    NetClient *NET;

signals:
    void SendJoin(QString);
    void SendIntMsg(QString msg);
public slots:
    void ShowPopup(QPoint center);
    void AddUser(QString uName);
    void SetUserStatus(QString uName,int status);
    void DelUser(QString uName);
    void Purge();
    void Refresh();
    void JoinRoom(QModelIndex idx);
    void JoinRoom();
    void AddToRoom(QString uName,int status);
    void DelFromBranch(QStandardItem *root,QString uname);
    void DelFromRoom(QString uName);
    void LeaveRoom();
    void StartRoom();
    void SearchFriend();
    void DelFriend();
    int GetLayer();

  //  void AddLayer();

};


#endif // CTL_FRIENDLIST_H

#ifndef PNL_NETCONTROLS_H
#define PNL_NETCONTROLS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include "NetClient.h"
#include "NetServer.h"
#include "bctl_widgetholder.h"

class pnl_NetControls : public bctl_WidgetHolder
{
    Q_OBJECT
public:
    explicit pnl_NetControls(QWidget *parent = 0);
    
    QPushButton *BtnConnect;
    QPushButton *BtnStartServer;
    QPushButton *BtnLogin;
    QPushButton *BtnNetStop;
    QPushButton *BtnStartRoom;
    QPushButton *BtnPartRoom;
    QPushButton *BtnSearch;
    QPushButton *BtnDelFriend;
    QPushButton *BtnLocal;
    QPushButton *BtnAbout;
    QLineEdit *EdTargetIP;
    QDialog *DlgAbout;
    QLabel *Lb1;
    QHBoxLayout *Lt;
    NetClient *netClient;
    NetServer *netServer;
signals:
    void sendlock (qint8);
public slots:
    void ToClient();
    void ToServer();
    void ToLocal();
    void ToNone();
    void Cstart();
    void init(NetClient *net, NetServer *snet);
    void CreateRoom();
    void Cloopback();
    void SearchFriend();
};



#endif // PNL_NETCONTROLS_H

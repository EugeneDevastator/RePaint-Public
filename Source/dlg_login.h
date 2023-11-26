#ifndef DLG_LOGIN_H
#define DLG_LOGIN_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <signal.h>
#include <QtGui>
#include "Brushes.h"
class dlg_login : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_login(QWidget *parent = 0);
    QPushButton *BtnUPLogin;
    QPushButton *BtnUPReg;
    QLineEdit *EdUsername;
    QLineEdit *EdPassword;
    QVBoxLayout *lgLT;
    QLabel *LbStatus;

public slots:
    void StartLogin();
    void StartReg();
    void RequestLog(QString msg);

signals:
    void SendLogin(QString user,QString pass);
    void SendReg(QString user,QString pass);

};

#endif // DLG_LOGIN_H

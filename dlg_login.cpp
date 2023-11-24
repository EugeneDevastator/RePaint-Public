#include "dlg_login.h"
#include <QtWidgets/QApplication>

dlg_login::dlg_login(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowIcon(QIcon(RESPATH+"/res/icon16.png"));
    this->setWindowTitle("Autenthification");
    BtnUPLogin=new QPushButton("Log In");
    BtnUPReg=new QPushButton("Register");

    EdUsername=new QLineEdit;
    EdPassword=new QLineEdit;

    EdUsername->setText("Username");
    EdUsername->selectAll();

    EdPassword->setText("Password");
    EdPassword->selectAll();
    //EdPassword.displayText()="Password";
    lgLT=new QVBoxLayout;
    LbStatus=new QLabel("zero");
QRegExpValidator *NickVld=new QRegExpValidator;
QRegExp *VStr=new QRegExp("[A-Z,a-z,0-9]{3,12}");
//VStr->setPattern("[A-Z]{1,12}");


NickVld->setRegExp(*VStr);

//EdUsername->setValidator(NickVld);
//EdPassword->setValidator(NickVld);
EdPassword->setEchoMode(QLineEdit::Password);

    lgLT->addWidget(EdUsername);
    lgLT->addWidget(EdPassword);
    lgLT->addWidget(BtnUPLogin);
    //lgLT->addWidget(BtnUPReg);
    lgLT->addWidget(LbStatus);

    setLayout(lgLT);
    connect(this->BtnUPLogin,SIGNAL(clicked()),this,SLOT(StartLogin()));
  //  connect(this->BtnUPReg,SIGNAL(clicked()),this,SLOT(StartReg()));

}
void dlg_login::StartLogin(){
    if (EdUsername->text().length()<2){
        EdUsername->setFocus();
        LbStatus->setText("Name > 2");
    }
    else if (EdPassword->text().length()<3) {
        EdPassword->setFocus();
        LbStatus->setText("Password > 2");
    }
    else{
    LbStatus->setText(EdUsername->text());
    this->accept();
    emit SendLogin(EdUsername->text(),EdPassword->text());
    }

}
void dlg_login::StartReg(){
    /*
    if (EdUsername->text().length()<2){
        EdUsername->setFocus();
        LbStatus->setText("Name > 1");
    }
    else if (EdPassword->text().length()<3) {
        EdPassword->setFocus();
        LbStatus->setText("Password > 2");
    }
    else{
    LbStatus->setText(EdUsername->text());
    this->accept();
    emit SendReg(EdUsername->text(),EdPassword->text());
    }
    */

}
void dlg_login::RequestLog(QString msg){
    this->exec();
   // this->show();
    LbStatus->setText(msg);
}

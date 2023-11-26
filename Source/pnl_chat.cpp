#include "pnl_chat.h"
#include <QTime>

pnl_Chat::pnl_Chat(QWidget *parent)
{

    ChatLine = new QLineEdit;
    Chat = new QTextEdit;

        Chat->insertPlainText("asdasda\n");

        Lt=new QVBoxLayout;


        Lt->addWidget(ChatLine,1);
        Lt->addWidget(Chat,900);
        Lt->setSpacing(0);
        Lt->setMargin(0);

            Chat->setReadOnly(true);
          setLayout2(Lt);

}
void pnl_Chat::GetChatMsg(QString Msg){
    //Msg.append("\n");
    //Msg.split(":");
    QString resmsg;
    //resmsg.prepend(Msg.split(":").at(0));
    resmsg.prepend(QTime::currentTime().toString("HH:mm:ss")+"> ");
    //resmsg.append(":"+Msg.split(":").at(1));
    resmsg.append(Msg);

 Chat->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor);
 Chat->insertPlainText(resmsg);
 Chat->ensureCursorVisible();
}

void pnl_Chat::GetIntchatMsg(QString imsg){
    QString resmsg;
    //resmsg.prepend(Msg.split(":").at(0));
    resmsg.prepend(QTime::currentTime().toString("HH:mm:ss")+"> ");
    //resmsg.append(":"+Msg.split(":").at(1));
    resmsg.append(imsg);
resmsg.append("\n");
    Chat->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor);
   Chat->insertPlainText(resmsg);
    Chat->ensureCursorVisible();

}

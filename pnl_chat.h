#ifndef PNL_CHAT_H
#define PNL_CHAT_H
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include "bctl_widgetholder.h"
class pnl_Chat : public bctl_WidgetHolder
{
    Q_OBJECT
public:
    explicit pnl_Chat(QWidget *parent = 0);
    QLineEdit *ChatLine;
    QTextEdit *Chat;
    QVBoxLayout *Lt;
signals:
    
public slots:
        void GetChatMsg(QString imsg);
        void GetIntchatMsg(QString imsg);
};

#endif // PNL_CHAT_H

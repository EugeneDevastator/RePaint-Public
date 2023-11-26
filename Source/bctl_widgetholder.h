#ifndef BCTL_WIDGETHOLDER_H
#define BCTL_WIDGETHOLDER_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QMoveEvent>
#include <QMouseEvent>

class bctl_WidgetHolder : public QWidget
{
    Q_OBJECT
public:
    explicit bctl_WidgetHolder(QWidget *parent = 0);
    QBoxLayout *LT;
     QLabel *Title;
     QWidget *PARENT;
     QPoint MPosOrigin;

QList <QWidget *> Siblings;
signals:
    void SendMove(QMoveEvent* event);
public slots:
    QRect AlignWidgets(QWidget *actor,QWidget *stator);
    QRect AlignWidgets(QRect actor, QWidget *stator);

protected:
    void mousePressEvent(QMouseEvent *event);
   void moveEvent(QMoveEvent *event);
    void	mouseMoveEvent( QMouseEvent * event );
    void paintEvent(QPaintEvent *);
//    bool event(QEvent *event);
    void  setLayout2(QLayout *lt);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    QPoint dragPosition;
};

#endif // BCTL_WIDGETHOLDER_H

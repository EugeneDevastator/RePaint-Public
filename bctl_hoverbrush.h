#ifndef BCTL_HOVERBRUSH_H
#define BCTL_HOVERBRUSH_H

#include <QWidget>
#include <QPainter>

class bctl_HoverBrush : public QWidget
{
    Q_OBJECT
public:
    explicit bctl_HoverBrush(QWidget *parent = 0);
    
signals:
    
public slots:
    void SetBSize(QSize sz);
    void GetNewB(QPoint centralpos, int rad,qreal tiltx, qreal tilty);
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // BCTL_HOVERBRUSH_H

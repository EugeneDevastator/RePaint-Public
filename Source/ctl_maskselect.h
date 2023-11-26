#ifndef DLG_MASKSELECT_H
#define DLG_MASKSELECT_H

#include <QtWidgets/QWidget>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <math.h>


class ctl_MaskSelect : public QWidget
{
    Q_OBJECT
public:
    explicit ctl_MaskSelect(QWidget *parent = 0);
    QImage *ViewCanvas;
    int cellsize;
    int clip;
    QPoint scrollpos;
    QPoint ppos;
    int selidx;
    QList<QImage *> *MaskList;

signals:
    void ItemSelected(int idx);
public slots:
    
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

};

#endif // DLG_MASKSELECT_H

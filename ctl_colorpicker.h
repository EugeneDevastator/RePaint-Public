#ifndef CTL_COLORPICKER_H
#define CTL_COLORPICKER_H

#include <QtWidgets/QWidget>
#include <QImage>
#include "ctl_bparam.h"
#include "Brushes.h"

class ctl_ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ctl_ColorPicker(QWidget *parent = 0);
    QImage *SquareArea;
    QImage *SavedGrad;
    qreal Hue;
    qreal Lit;
    qreal Sat;

signals:
    

public slots:
    void RecalcMainGrad();
    void paintEvent(QPaintEvent *event);

protected:
void mouseMoveEvent(QMouseEvent *event);
void resizeEvent(QResizeEvent *event);
void mousePressEvent(QMouseEvent *event);
};

#endif // CTL_COLORPICKER_H

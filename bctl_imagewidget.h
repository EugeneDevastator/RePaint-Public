#ifndef BCTL_IMAGEWIDGET_H
#define BCTL_IMAGEWIDGET_H

#include <QtWidgets/QWidget>

class bctl_ImageWidget : public QWidget
{
    Q_OBJECT

public:
    QImage ViewImage;

    explicit bctl_ImageWidget(QWidget *parent = 0);

signals:

public slots:
protected:
   virtual void paintEvent(QPaintEvent *event);


};

#endif // BCTL_IMAGEWIDGET_H

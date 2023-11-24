#ifndef IMAGEBOX_H
#define IMAGEBOX_H

#include <QWidget>
#include <QImage>

class ImageBox : public QWidget
{
    Q_OBJECT
public:
    explicit ImageBox(QWidget *parent = 0);
    QImage Image;
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // IMAGEBOX_H

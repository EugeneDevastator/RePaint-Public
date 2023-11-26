#ifndef BRUSHPARAM_H
#define BRUSHPARAM_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class BrushParam : public QWidget
{
    Q_OBJECT
public:
    explicit BrushParam(QWidget *parent = 0);

signals:

public slots:

private:
    QSlider *slider;
    QPushButton *BtnTabmode;

};

#endif // BRUSHPARAM_H

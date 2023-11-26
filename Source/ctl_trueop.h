#ifndef CTL_TRUEOP_H
#define CTL_TRUEOP_H

#include <QtWidgets/QWidget>

class ctl_trueop : public QWidget
{
    Q_OBJECT
public:
    explicit ctl_trueop(QWidget *parent = 0);
    float op;
    QColor col;
    float spc;
    double *rrel;
    double *crv;
    double *hue;
    double *sat;
    double *lit;
    bool repainted;
QImage *BG;
signals:
    
public slots:
    void setop(float op);
    void setspc(float spc);
protected:
    void paintEvent(QPaintEvent *event);

};


#endif // CTL_TRUEOP_H

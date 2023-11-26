#ifndef CTL_BMSELECTOR_H
#define CTL_BMSELECTOR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QPainter>
#include <QtWidgets/QHBoxLayout>
#include "Brushes.h"

class ctl_BMselector : public QWidget
{
    Q_OBJECT
public:
    explicit ctl_BMselector(QWidget *parent = 0);
    QList <QString> BMnames;
    QList <QPainter::CompositionMode> BMmodes;

    QComboBox *combo;


public slots:
    void setBMbyint(int);
    //void setBMbycm
    QPainter::CompositionMode GetCMfromIndex(int idx);
    int GetIdx();
    void SendChangedIdx(int idx);
signals:
    void BMchanged(int);

    
};

#endif // CTL_BMSELECTOR_H

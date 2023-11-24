#ifndef PNL_FILEMENU_H
#define PNL_FILEMENU_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>
#include "bctl_widgetholder.h"
#include "Brushes.h"
class pnl_FileMenu : public bctl_WidgetHolder
{
    Q_OBJECT
public:
    explicit pnl_FileMenu(QWidget *parent = 0);
    QPushButton *BtnNew;
    QPushButton *BtnSave;
    QPushButton *BtnSaveAs;
    QPushButton *BtnOpen;
    QPushButton *BtnReload;
    QPushButton *BtnSnap;
    QPushButton *BtnPaste;
    QPushButton *BtnPinPanels;
    QBoxLayout *Lt;
signals:
    
public slots:
    
};

#endif // PNL_FILEMENU_H

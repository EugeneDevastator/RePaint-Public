#include "pnl_filemenu.h"
#include <QtWidgets/QApplication>

pnl_FileMenu::pnl_FileMenu(QWidget *parent)
{
    Title->setText("Painting Operations");
    BtnNew=new QPushButton();
    BtnSave=new QPushButton();
    BtnSaveAs=new QPushButton();
    BtnOpen=new QPushButton();
    BtnReload=new QPushButton();
    BtnSnap=new QPushButton();
    BtnPaste=new QPushButton();

    BtnPinPanels=new QPushButton();



    BtnSave->setIcon(QIcon(RESPATH+"/res/btnsave.png"));
    BtnSaveAs->setIcon(QIcon(RESPATH+"/res/btnsaveas.png"));

    BtnOpen->setIcon(QIcon(RESPATH+"/res/btnopen.png"));
    BtnReload->setIcon(QIcon(RESPATH+"/res/btnreload.png"));
    BtnSnap->setIcon(QIcon(RESPATH+"/res/btnsnap.png"));
    BtnPaste->setIcon(QIcon(RESPATH+"/res/btnpaste.png"));
    BtnPinPanels->setIcon(QIcon(RESPATH+"/res/btnpin.png"));

    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    BtnNew->setSizePolicy(sizePolicy);
    BtnSaveAs->setSizePolicy(sizePolicy);
    BtnOpen->setSizePolicy(sizePolicy);
    BtnSave->setSizePolicy(sizePolicy);
    BtnSnap->setSizePolicy(sizePolicy);
    BtnReload->setSizePolicy(sizePolicy);
    BtnPaste->setSizePolicy(sizePolicy);
BtnPinPanels->setSizePolicy(sizePolicy);



    BtnNew->setIcon(QIcon(RESPATH+"/res/btnnew.png"));
    //BtnNew->setIconSize(QSize(54,54));


    Lt=new QBoxLayout(QBoxLayout::LeftToRight);
    Lt->setDirection(QBoxLayout::LeftToRight);
    Lt->setSpacing(1);
    //Lt->setSpacing(0);
    Lt->addWidget(BtnNew);
    Lt->addWidget(BtnOpen);
    Lt->addWidget(BtnSaveAs);
    Lt->addWidget(BtnSave);
    Lt->addWidget(BtnReload);
    Lt->addWidget(BtnSnap);
        Lt->addWidget(BtnPinPanels);
//    Lt->addWidget(BtnPaste);

    this->setLayout2(Lt);
}

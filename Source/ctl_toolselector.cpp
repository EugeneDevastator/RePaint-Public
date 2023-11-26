#include "ctl_toolselector.h"
#include <QResource>
#include <QtWidgets/QApplication>

ctl_ToolSelector::ctl_ToolSelector(QWidget *parent) : QWidget(parent)
{
    QList<QToolButton *> ListBtns;
    QList<QString> BNames;
    for (int i = 0; i < eEOE; i++)
    {
        BNames.append("tNONE");
    }
    BNames[eBrush] = "tBrush";
    BNames[eSmudge] = "tSmudge";
    BNames[eDisp] = "tDisp";
    BNames[eLine] = "tLine";
    BNames[eCont] = "tCont";
    // BNames[eFbrush]="tFbrush";

    for (int i = 0; i < eSTOP; i++)
    {
        // Tfuncs[i]=this->SelectTool;
        Btns[i] = new QToolButton;
        Btns[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
        Btns[i]->setFixedWidth(64);
        Btns[i]->setFixedHeight(64);
        Btns[i]->setIconSize(QSize(48, 48));
        Btns[i]->setDown(false);
        Btns[i]->setIcon(QIcon(RESPATH + "/res/" + BNames[i] + ".png"));
        Btns[i]->setCheckable(true);
        Btns[i]->setAutoExclusive(true);
        // Btns[i]->setAutoRaise(false);

        connect(Btns[i], SIGNAL(clicked()), this, SLOT(SelectTool()));
    }

    ToolID = 0;
    Btns[0]->click();

    QGridLayout *lt = new QGridLayout;
    TB = new QToolBar;
    int cnt = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (cnt == eSTOP)
                break;

            lt->addWidget(Btns[cnt], i, j);
            cnt++;
        }
    }

    //    lt->setRowStretch(0,1);
    //  lt->setRowStretch(1,1);
    lt->setSpacing(0);
    lt->setMargin(0);
    setLayout(lt);
}
void ctl_ToolSelector::Release()
{
    for (int i = 0; i < eSTOP; i++)
    {
        Btns[i]->setDown(false);
    }
}
void ctl_ToolSelector::SelectTool()
{
    for (int i = 0; i < eSTOP; i++)
    {
        if (Btns[i]->isChecked())
            ToolID = i;
    }

    emit ToolSelected(ToolID);
    // ToolID=i;
    // Release();
    // Btns[i]->setDown(true);
}
void ctl_ToolSelector::AssignTool(int id)
{
    Btns[ToolID]->setDown(false);
    Btns[ToolID]->setChecked(false);
    ToolID = id;
    Btns[ToolID]->setChecked(true);
    // Btns[id]->click();

    //    emit ToolSelected(ToolID);
}

void ctl_ToolSelector::SelectBrush()
{
    ToolID = 1;
    Release();
    // BtnBrush->setDown(true);
}
void ctl_ToolSelector::SelectSmudge()
{
    ToolID = 2;
    Release();
    // BtnSmudge->setDown(true);
}

void ctl_ToolSelector::SelectDisp()
{
    ToolID = 3;
    Release();
    // BtnDisp->setDown(true);
}
void ctl_ToolSelector::SelectLine()
{
    ToolID = 4;
    Release();
    // BtnLine->setDown(true);
}
/*
void ctl_ToolSelector::SelectCbr(){
ToolID=4;
Release();
//BtnLine->setDown(true);
}
*/

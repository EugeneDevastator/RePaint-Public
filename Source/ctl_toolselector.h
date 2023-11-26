#ifndef CTL_TOOLSELECTOR_H
#define CTL_TOOLSELECTOR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QToolBar>
#include "Brushes.h"
class ctl_ToolSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ctl_ToolSelector(QWidget *parent = 0);
    int ToolID;
        QToolButton *Btns[9];
signals:
        void ToolSelected(int);
public slots:
    void SelectBrush();
    void SelectSmudge();
    void SelectDisp();
    void SelectLine();
    void Release();
    void SelectTool();
    void AssignTool(int id);
private:
    typedef void (Fptr)(int);

    QToolBar *TB;
   // Fptr Tfuncs[9];




};

#endif // CTL_TOOLSELECTOR_H

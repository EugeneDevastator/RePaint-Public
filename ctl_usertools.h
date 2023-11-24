#ifndef CTL_USERTOOLS_H
#define CTL_USERTOOLS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QtWidgets/QListView>
#include <QtWidgets/QLineEdit>
#include "Brushes.h"
#include "pnl_bcontorls.h"
#include "bctl_widgetholder.h"
#include <signal.h>
#include <QFile>
#include <QSettings>

class ctl_UserTools : public bctl_WidgetHolder
{
    Q_OBJECT
public:
    explicit ctl_UserTools(pnl_bcontorls * bc,QWidget *parent = 0);

    pnl_bcontorls *BC;
    QPushButton BtnJoin;
    QPushButton BtnCreate;
    QGridLayout *LT;
    QStandardItemModel *itModel;
    QItemSelectionModel *selModel;

    QListView *View;
    QList <d_ToolPreset> ToolsList;
    QFile ToolsFile;
    QDataStream ToolStream;
    qint8 CtlNum;

    QLineEdit *EdName;
    QPushButton *BtnSave;
    QPushButton *BtnDel;
    QPushButton *BtnSaveAll;
    QSettings ToolsINI;
    d_ToolPreset CurrPreset;



signals:
    void SetPreset(d_ToolPreset preset);


public slots:
    void GetPreset(d_ToolPreset preset);
    void SaveTools();
    void LoadTools();

    void LoadToolsINI();
    void SaveToolsINI();

   void ApplyTool(int idx);
    void SaveTool();
    void Remodel();
    void ClickTool(QModelIndex midx);
    void DelTool();
};

#endif // CTL_USERTOOLS_H

#ifndef CTL_LAYERSPANEL_H
#define CTL_LAYERSPANEL_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolButton>
#include <QStandardItemModel>
#include <QtWidgets/QTreeView>
#include <QItemSelectionModel>
#include "bctl_dblslider.h"
#include "brushes.h"
#include "ctl_BMselector.h"
#include "bctl_widgetholder.h"


class pnl_Layers : public bctl_WidgetHolder

{
      Q_OBJECT
public:

    int ActiveLayer;
    qint8 locked;
    quint8 movinglayer;
    int Lmovelock;

    ctl_BMselector *BMsel;
    QVBoxLayout *LT;
    bctl_DblSlider *OpSlider;
    QComboBox *CbBmode;
    QList <sLayerProps> *LayerProps;

    QListWidget *LayerList;

    QPushButton *BtnAdd;
    QPushButton *BtnDel;
    QPushButton *BtnDrop;
    QPushButton *BtnDup;
    QToolButton *BtnPresOp;

    QLabel *LbIndex;

    //QTreeModel *treemodel;
    //QList <QIcon> sIcons;
    ///QString *LocalName;

    explicit pnl_Layers(QWidget *parent=0);

public slots:
//    void AddLayer(QImage thumb);
    void AddLayerAt(int before);
    void UpdateLayerInfo(QModelIndex newid,QModelIndex oldid);
    void UpdateLayerInfo(QListWidgetItem *itm);
    void DelLayer(int userrow);
    void DelActiveLayer();
    void SetThumb(int userlayer, QImage thumb);
    void SetActiveOp(float op);
    void SetActiveBm(int bm);
    void ItemHasChanged(QListWidgetItem *itm);
    void DropLayerCmd();
    quint16 CheckIndex(quint16 idx);

     void ListItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
     void ItemMoved (int ilist);
     void ItemPicked(QListWidgetItem* itm);
     void ItemRenamed(QString newName);
     void ListWidgetEditEnd(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
//     void ItemAltered(QListWidgetItem *itm);
     void SelChanged ();

    void AddLayerCmd();
    void DelLayerCmd();
    void DupLayerCmd();
    //void DupLayer(int before);
    void ExecLAction(d_LAction lact);
    void SetLock(qint8 lk);
    int GetActiveLayer();

signals:
    void SendActiveLayer(int);
    void MouseOut();
    void MouseIn();
    void SendLayerVis(int,bool);
    void SendDelLayer(int);
    void SendDropLayer(int);
    void SendLayerOp(int,float);
    void SendLayerBm(int,int); // layer, bm
    void SendConfirmedLayerOp(int,float);

    void SendLayerAction(d_LAction lact);
    void ActionDone();

};

#endif // CTL_LAYERSPANEL_H

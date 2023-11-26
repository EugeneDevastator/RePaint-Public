#include "ctl_friendlist.h"
#include <QAbstractItemModel>
#include <QtGui>
#include <QtWidgets/QMenu>

ctl_friendlist::ctl_friendlist(NetClient *net, QWidget *parent)

{
    NET = net;
    /*    iOn=QIcon(RESPATH+"/res/uOnline.png");
        iOff=QIcon(RESPATH+"/res/uOffline.png");
        iBusy=QIcon(RESPATH+"/res/uBusy.png");
        iIris=QIcon(RESPATH+"/res/uIris.png");
        iRoomFree=QIcon(RESPATH+"/res/uIris.png");
        iRoomFree=QIcon(RESPATH+"/res/uIris.png");*/
    for (int i = 0; i < 10; i++)
    {
        sIcons.append(QIcon(RESPATH + "/res/uOnline.png"));
    }

    // QContextMenuEvent *ctxEvent=new QContextMenuEvent();
    DlgFind = new dlg_UserSearch;

    sIcons.replace(suOn, QIcon(RESPATH + "/res/uOnline.png"));
    sIcons.replace(suOff, QIcon(RESPATH + "/res/uOffline.png"));
    sIcons.replace(suRoomFree, QIcon(RESPATH + "/res/uRfree.png"));
    sIcons.replace(suRoomClosed, QIcon(RESPATH + "/res/uRclosed.png"));
    sIcons.replace(suMemberFree, QIcon(RESPATH + "/res/uMfree.png"));
    sIcons.replace(suMemberClosed, QIcon(RESPATH + "/res/uMclosed.png"));

    // FLIST = new QListView();
    FLIST = new QTreeView();
    LT = new QVBoxLayout();
    LT->addWidget(FLIST);
    setLayout2(LT);
    stimodel = new QStandardItemModel;

    friends = new QStandardItemModel;
    roommates = new QStandardItemModel;
    spectators = new QStandardItemModel;

    FriendRoot = new QStandardItem("FRIENDS");
    RoomRoot = new QStandardItem("PAINTERS");
    SpectRoot = new QStandardItem("EYES");

    stimodel->appendRow(RoomRoot);
    stimodel->appendRow(FriendRoot);
    stimodel->appendRow(SpectRoot);

    selmodel = new QItemSelectionModel(stimodel);
    // FLIST->setItemsExpandable(false);
    // FLIST->setExpanded();

    FLIST->setModel(stimodel);
    FLIST->setSelectionModel(selmodel);
    FLIST->setHeaderHidden(true);
    FLIST->setIndentation(8);
    // FLIST->setModel(treemodel);

    FLIST->setEditTriggers(QAbstractItemView::NoEditTriggers);
    FLIST->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(FLIST, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(JoinRoom(QModelIndex)));

    //-------------- tests
    //    ListView->setDropIndicatorShown(true);
    FLIST->setSelectionBehavior(QAbstractItemView::SelectRows);
    FLIST->setDragEnabled(true);
    FLIST->setAcceptDrops(true);
    //     ListView->setDropIndicatorShown(true);
    FLIST->setDefaultDropAction(Qt::MoveAction);
    FLIST->setDragDropMode(QAbstractItemView::InternalMove);
    // ListView->setDragDropOverwriteMode(false);

    //--------------- EOT

    MnuPopup = new QMenu;

    MnuPopup->addAction(QIcon(RESPATH + "/res/fJoin.png"), "Join");     // 0
    MnuPopup->addAction(QIcon(RESPATH + "/res/fCreate.png"), "Create"); // 1
    MnuPopup->addAction(QIcon(RESPATH + "/res/fPart.png"), "Leave");    // 2
    MnuPopup->addAction(QIcon(RESPATH + "/res/fFind.png"), "Find");     // 3
    MnuPopup->addAction(QIcon(RESPATH + "/res/fDel.png"), "Forget");    // 4

    connect(MnuPopup->actions().value(0), SIGNAL(triggered()), this, SLOT(JoinRoom()));
    connect(MnuPopup->actions().value(1), SIGNAL(triggered()), this, SLOT(StartRoom()));
    connect(MnuPopup->actions().value(2), SIGNAL(triggered()), this, SLOT(LeaveRoom()));
    connect(MnuPopup->actions().value(3), SIGNAL(triggered()), this, SLOT(SearchFriend()));
    connect(MnuPopup->actions().value(4), SIGNAL(triggered()), this, SLOT(DelFriend()));

    DlgFind->setModal(true);

    connect(DlgFind, SIGNAL(AskName(QString)), NET, SLOT(C_FindFriend(QString)));
    connect(DlgFind, SIGNAL(AskAdd(QString)), NET, SLOT(C_AddFriend(QString)));
    connect(NET, SIGNAL(SendPeopleList(QStringList)), DlgFind, SLOT(GetPeopleList(QStringList)));

    // make loof for lists of friends. connect(NET,SIGNAL()

    //   MnuPopup->pos().setX(300);
    //    MnuPopup->pos().setY(300);
    // MnuPopup->setParent(FLIST);
    // MnuPopup->show();

    FLIST->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(FLIST, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowPopup(QPoint)));
}

void ctl_friendlist::ShowPopup(QPoint center)
{
    center = mapToGlobal(center);
    if (RoomRoot->rowCount() == 0)
        MnuPopup->actions()[2]->setVisible(false);
    else
        MnuPopup->actions()[2]->setVisible(true);
    if (stimodel->itemFromIndex(selmodel->selectedIndexes().at(0))->parent() != NULL)
    {
        MnuPopup->actions()[4]->setVisible(true);
        MnuPopup->actions()[0]->setVisible(true);
    }
    else
    {
        MnuPopup->actions()[4]->setVisible(false);
        MnuPopup->actions()[0]->setVisible(false);
    }

    MnuPopup->popup(center);
}

void ctl_friendlist::JoinRoom(QModelIndex idx)
{
    if (stimodel->itemFromIndex(idx)->parent() != NULL)
    {
        QString jname = stimodel->itemFromIndex(idx)->text();
        emit SendJoin(jname);
    }
}

void ctl_friendlist::JoinRoom()
{
    QModelIndex idx = selmodel->selectedIndexes().at(0);
    if (stimodel->itemFromIndex(idx)->parent() != NULL)
    {
        QString jname = stimodel->itemFromIndex(idx)->text();
        emit SendJoin(jname);
    }
}

void ctl_friendlist::Refresh()
{
    //   stimodel->clear();
    /*roommates.sort(0);
    friends.sort(0);
    spectators.sort(0);*/
    //   for(int i=0;i<roommates->rowCount();i++)      stimodel->appendRow(roommates->item(i));
    //  for (int i=0;i<friends->rowCount();i++)       stimodel->appendRow(friends->itemFromIndex(friends->item(i)->index()));
    //  for (int i=0;i<spectators->rowCount();i++)    stimodel->appendRow(spectators->item(i));
}

void ctl_friendlist::AddUser(QString uName)
{
    QStandardItem *newItm = new QStandardItem(sIcons.at(suOn), uName);
    FriendRoot->appendRow(newItm);
    emit SendIntMsg("[" + uName + " connected]");
}
void ctl_friendlist::LeaveRoom()
{
    NET->C_PartRoom();
    while (RoomRoot->child(0) != NULL)
    {
        RoomRoot->removeRow(0);
    }
}

void ctl_friendlist::StartRoom()
{
    NET->C_StartRoom();
}

void ctl_friendlist::SetUserStatus(QString uName, int status)
{
    int i = 0;
    while ((FriendRoot->child(i) != NULL))
    {
        if (FriendRoot->child(i)->text().compare(uName) == 0)
            break;
        i++;
    }

    if (FriendRoot->child(i) != NULL)
    {
        FriendRoot->child(i)->setIcon(sIcons.at(status));
    }
    else
    {
        QStandardItem *newItm = new QStandardItem(sIcons.at(status), uName);
        FriendRoot->appendRow(newItm);
    }
    FLIST->expandAll();
};

void ctl_friendlist::DelFromBranch(QStandardItem *root, QString uname)
{
    /*
    while ((root->child(i)->text().compare(uname))!=0 & (root->child(i)!=NULL)) { i=i+1; }

        if (root->child(i)!=NULL) { root->removeRow(i); }*/
    for (int i = 0; i < root->rowCount(); i++)
    {
        if ((root->child(i)->text().compare(uname)) == 0)
        {
            root->removeRow(i);
            break;
        }
    }
}

void ctl_friendlist::DelUser(QString uName)
{
    DelFromBranch(RoomRoot, uName);
    DelFromBranch(SpectRoot, uName);
    DelFromBranch(FriendRoot, uName);
}
void ctl_friendlist::Purge()
{
    while (FriendRoot->child(0) != NULL)
    {
        FriendRoot->removeRow(0);
    }
    while (RoomRoot->child(0) != NULL)
    {
        RoomRoot->removeRow(0);
    }
    while (SpectRoot->child(0) != NULL)
    {
        SpectRoot->removeRow(0);
    }
}
void ctl_friendlist::AddToRoom(QString uName, int status)
{

    QStandardItem *newItm = new QStandardItem(sIcons.at(status), uName);
    RoomRoot->appendRow(newItm);
    FLIST->expandAll();
    emit SendIntMsg("[" + uName + " joins room]");
}
void ctl_friendlist::DelFromRoom(QString uName)
{
    DelFromBranch(RoomRoot, uName);
    FLIST->expandAll();
    emit SendIntMsg("[" + uName + " leaves room]");
}

void ctl_friendlist::SearchFriend()
{
    /*bool ok;
   QString iname=QInputDialog::getText(this, tr("Search for a hand"),
                                             tr("Name:"), QLineEdit::Normal,
                         "InputName", &ok);
   if (ok && !iname.isEmpty() && iname.length()>1){
       NET->C_FindFriend(iname);
   }*/
    DlgFind->show();
}
void ctl_friendlist::DelFriend()
{
    QModelIndex idx = selmodel->selectedIndexes().at(0);
    if (stimodel->itemFromIndex(idx)->parent() != NULL)
    {
        NET->C_DelFriend(stimodel->itemFromIndex(idx)->text());
        DelFromBranch(FriendRoot, stimodel->itemFromIndex(idx)->text());
    }
}
int ctl_friendlist::GetLayer()
{
    if (selmodel->selectedIndexes().count() > 0)
    {
        QModelIndex idx = selmodel->selectedIndexes().at(0);
        if (stimodel->itemFromIndex(idx)->parent() != NULL)
        {
            if (stimodel->itemFromIndex(idx)->parent()->row() == 0)
            {
                // return 7;//stimodel->itemFromIndex(idx)->row();
                return (stimodel->itemFromIndex(idx)->row()) + 1;
            }
        }
    }
    return 0;
}

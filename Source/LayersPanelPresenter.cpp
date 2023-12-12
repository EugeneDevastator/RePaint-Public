#include "LayersPanelPresenter.h"
#include "Brushes.h"

LayersPanelPresenter::LayersPanelPresenter(QWidget *parent)

{
    setAccessibleName("LayersPanel");
    setWindowTitle("Layers");
    Title->setText("Layers");
    qDebug() << ("layer panel init BEGIN");
    locked = 0;
    // LayerProps=new QList <LayerData>;
    LbIndex = new QLabel("Layer index");
    qDebug() << ("layer panel created objs.");

    //   LstLayers->setDragEnabled(true);

    //   LstLayers->setDropIndicatorShown(true);

    // MdlLayers->setSupportedDragActions(Qt::MoveAction);

    qDebug() << ("layer panel init part1");

    OpSlider = new bctl_DblSlider;
    OpSlider->setFixedHeight(32);

    LayerList = new QListWidget();
    LayerList->setIconSize(QSize(64, 64));
    LayerList->setDragDropMode(QAbstractItemView::InternalMove);
    LayerList->setFocusPolicy(Qt::NoFocus);
    LayerList->setFocusProxy(this);

    BtnAdd = new QPushButton();
    BtnAdd->setIconSize(QSize(32, 32));
    BtnAdd->setFixedHeight(48);
    BtnAdd->setToolTip("Add New Layer");
    BtnDrop = new QPushButton();
    BtnDrop->setFixedHeight(48);
    BtnDrop->setIconSize(QSize(32, 32));
    BtnDrop->setToolTip("Drop Layer on the one below");
    BtnDel = new QPushButton();
    BtnDel->setFixedHeight(48);
    BtnDel->setIconSize(QSize(32, 32));
    BtnDel->setToolTip("Delete Layer");
    BtnDup = new QPushButton();
    BtnDup->setFixedHeight(48);
    BtnDup->setIconSize(QSize(32, 32));
    BtnDup->setToolTip("Duplicate Layer");
    BMsel = new ctl_BMselector;
    BMsel->setFocusProxy(this);
    BtnPresOp = new QToolButton;
    BtnPresOp->setText("Preserve opacity");
    BtnPresOp->setAutoRaise(false);
    BtnPresOp->setCheckable(true);

    BtnAdd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    BtnDrop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    BtnDel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    BtnDup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // BtnAdd->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    qDebug() << ("layer panel init part2");
    LT = new QVBoxLayout;

    QHBoxLayout *LTbtns = new QHBoxLayout;
    LTbtns->addWidget(BtnAdd);
    LTbtns->addWidget(BtnDup);
    LTbtns->addWidget(BtnDrop);
    LTbtns->addWidget(BtnDel);
    // LTbtns->addWidget(BMsel);

    // LTbtns->addWidget();
    // QHBoxLayout *LTctls=new QHBoxLayout;

    LT->addLayout(LTbtns);
    LT->addWidget(BMsel);
    LT->addWidget(OpSlider);
    LT->addWidget(LbIndex);
    //
    LT->addWidget(BtnPresOp);

    LT->addWidget(LayerList);
    // LT->addWidget(LstLayers);

    Lmovelock = 0;
    ActiveLayer = 0;
    qDebug() << ("layer panel init part3");
    this->setLayout2(LT);

    // connect(BtnDel,SIGNAL(clicked()),this,SLOT(DelActiveLayer()));
    connect(BtnDel, SIGNAL(clicked()), this, SLOT(DelLayerCmd()));
    connect(BtnAdd, SIGNAL(clicked()), this, SLOT(AddLayerCmd()));
    connect(BtnDup, SIGNAL(clicked()), this, SLOT(DupLayerCmd()));

    connect(OpSlider, SIGNAL(ValChange(float)), this, SLOT(SetActiveOp(float)));

    qDebug() << ("layer panel init part3a");
    // connect(MdlLayers,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(ItemChange(QStandardItem*)));
    connect(BtnDrop, SIGNAL(clicked()), this, SLOT(DropLayerCmd()));
    connect(BMsel, SIGNAL(BMchanged(int)), this, SLOT(SetActiveBm(int)));

    BtnAdd->setIcon(QIcon(RESPATH + "/res/btnlnew.png"));
    BtnDup->setIcon(QIcon(RESPATH + "/res/btnldup.png"));
    BtnDel->setIcon(QIcon(RESPATH + "/res/btnldel.png"));
    BtnDrop->setIcon(QIcon(RESPATH + "/res/btnldrop.png"));

    // QStandardItem *itm=new QStandardItem(QIcon(QPixmap::fromImage(thumb)),"New Layer");
    // MdlLayers->insertRow(-1,itm);
    // selmodel->select(MdlLayers->indexFromItem(itm),QItemSelectionModel::ClearAndSelect);
    // OpSlider->clipmaxF=LayerProps->at(ActiveLayer).Opacity;
    // OpSlider->Redraw();
    // BMsel->setBMbyint(LayerProps->at(ActiveLayer).blendmode);
    ActiveLayer = 0;
    //  selmodel->select(MdlLayers->item(ActiveLayer)->index(),QItemSelectionModel::ClearAndSelect);

    qDebug() << ("layer panel init end");

    QListWidgetItem *itmw = new QListWidgetItem("new layer");
    itmw->setCheckState(Qt::Checked);

    /*
    LayerList->addItem("hui1");
    LayerList->addItem("pizda2");
    LayerList->addItem(itmw);
    LayerList->addItem("zhopa3");

    itmw->setSelected(true);
    */
    // LayerList->setMouseTracking(true);
    connect(LayerList->model(), SIGNAL(layoutChanged()), this, SLOT(SelChanged()));

    // connect(LayerList,SIGNAL(QAbstractItemView::dataChanged(QModelIndex,QModelIndex)),this,SLOT(ItemChange(QModelIndex,QModelIndex)));
    // indecurrentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(ListItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(LayerList, SIGNAL(indexesMoved(QModelIndexList)), this, SLOT(ItemMoved(QModelIndexList)));
    connect(LayerList, SIGNAL(itemEntered(QListWidgetItem *)), this, SLOT(ItemPicked(QListWidgetItem *)));
    connect(LayerList, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(UpdateLayerInfo(QListWidgetItem *)));
    connect(LayerList, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(ItemHasChanged(QListWidgetItem *)));
    // connect(LayerList,SIGNAL(currentTextChanged(QString)),this,SLOT(ItemRenamed(QString)));
    connect(LayerList->itemDelegate(), SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)), this, SLOT(ListWidgetEditEnd(QWidget *, QAbstractItemDelegate::EndEditHint)), Qt::DirectConnection);
}

void LayersPanelPresenter::ListWidgetEditEnd(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    quint16 lyr = GetActiveLayer();
    (LayerProps->operator[](lyr)).layerName = "Fixlayernaming"; // reinterpret_cast<QLineEdit*>(editor)->text();
    LayerList->releaseKeyboard();
    //    qDebug() << reinterpret_cast<QLineEdit*>(editor)->text() << "at layer:" << GetActiveLayer();
}

quint16 LayersPanelPresenter::CheckIndex(quint16 idx)
{

    // if (MdlLayers->rowCount()<(idx+1)) idx=MdlLayers->rowCount()-1;
    if (LayerProps->count() < (idx + 1))
        idx = LayerProps->count() - 1;
    return idx;
}

int LayersPanelPresenter::GetActiveLayer()
{

    quint16 Lyr = 0;
    //  if (!selmodel->selectedRows().isEmpty())
    //  Lyr=selmodel->selectedRows().at(0).row();
    if (LayerList->selectedItems().count() > 0)
    {
        Lyr = LayerList->row(LayerList->selectedItems().at(0));
    }
    else
        LayerList->item(0)->setSelected(true);
    return Lyr;
}

void LayersPanelPresenter::AddLayerAt(int before)
{

    QListWidgetItem *itmw = new QListWidgetItem("new layer");

    itmw->setCheckState(Qt::Checked);
    itmw->setFlags(itmw->flags() | Qt::ItemIsEditable);

    LayerList->addItem(itmw);
    if (LayerList->selectedItems().count() < 1)
        LayerList->itemAt(0, 0)->setSelected(true);

    ActiveLayer = GetActiveLayer();
}

void LayersPanelPresenter::DelLayer(int userrow)
{
    ActiveLayer = GetActiveLayer();
    if (LayerList->count() > 1)
    {

        LayerList->takeItem(userrow);
    }
    ActiveLayer = GetActiveLayer();
}

void LayersPanelPresenter::DelActiveLayer()
{
    // DelLayer(ActiveLayer);
    LayerAction lact;
    lact.ActID = laDel;
    lact.layer = ActiveLayer;
    emit SendLayerAction(lact);
}
void LayersPanelPresenter::SetThumb(int userlayer, QImage thumb)
{
    // Lmovelock+=1;

    if (userlayer < LayerList->count())
    {
        LayerList->item(userlayer)->setIcon(QIcon(QPixmap::fromImage(thumb)));
        //  MdlLayers->item(userlayer)->setIcon(QIcon(QPixmap::fromImage(thumb)));
    }
}

void LayersPanelPresenter::DropLayerCmd()
{
    //    emit SendDropLayer(ActiveLayer);

    if (GetActiveLayer() < LayerProps->count() - 1)
    {
        LayerAction lact;
        lact.ActID = laDrop;
        lact.layer = GetActiveLayer();
        emit SendLayerAction(lact);
    }

    //    DelLayer(ActiveLayer);
}

void LayersPanelPresenter::SetActiveOp(float op)
{
    // LayerProps[ActiveLayer].Opacity=Opacity;
    LayerAction lact;
    lact.ActID = laOp;
    lact.layer = GetActiveLayer();
    lact.op = op;
    emit SendLayerAction(lact);
    // emit SendLayerOp(ActiveLayer,Opacity);
}
void LayersPanelPresenter::SetActiveBm(int bm)
{
    // LayerProps[ActiveLayer].blendmode=bm;
    LayerAction lact;
    lact.ActID = laBm;
    lact.layer = GetActiveLayer();
    lact.bm = bm;
    emit SendLayerAction(lact);

    // emit SendLayerBm(ActiveLayer,bm);
}

void LayersPanelPresenter::ItemHasChanged(QListWidgetItem *itm)
{
    int layer = LayerList->row(itm);
    // layer=CheckIndex(layer);
    bool itemvis = itm->checkState();
    bool innervis = LayerProps->at(layer).IsVisible;
    if ((itemvis) != (innervis))
    {
        emit SendLayerVis(layer, itm->checkState());

        qDebug() << "name is:" << itm->text();

        qDebug() << "set vis for layer " << layer;
    }
}

void LayersPanelPresenter::UpdateLayerInfo(QModelIndex newid, QModelIndex oldid)
{

    ActiveLayer = GetActiveLayer(); // MdlLayers->itemFromIndex(newid)->row();
    OpSlider->clipmaxF = LayerProps->at(ActiveLayer).Opacity;
    OpSlider->Redraw();
    BMsel->setBMbyint(LayerProps->at(ActiveLayer).blendmode);

    // emit SendActiveLayer(ActiveLayer);
    /*   if (selmodel->selectedIndexes().count()>0){
           ActiveLayer=MdlLayers->itemFromIndex(newid)->row();
           OpSlider->clipmaxF=LayerProps->at(ActiveLayer).Opacity;
           OpSlider->Redraw();
           BMsel->setBMbyint(LayerProps->at(ActiveLayer).blendmode);
           }

      emit SendActiveLayer(ActiveLayer); // obsolete was used to send AL to
   */
}

void LayersPanelPresenter::UpdateLayerInfo(QListWidgetItem *itm)
{

    ActiveLayer = GetActiveLayer(); // MdlLayers->itemFromIndex(newid)->row();
    OpSlider->clipmaxF = LayerProps->at(ActiveLayer).Opacity;
    OpSlider->Redraw();
    BMsel->setBMbyint(LayerProps->at(ActiveLayer).blendmode);
    LbIndex->setText(QString::number(ActiveLayer));
}

void LayersPanelPresenter::ListItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    int c = LayerList->row(current); // this is for changing selection;
    int p = LayerList->row(previous);
    qDebug() << c << " previous:" << p;
}

void LayersPanelPresenter::ItemPicked(QListWidgetItem *itm)
{
    movinglayer = LayerList->row(itm);
    qDebug() << movinglayer << "dragged item";
}

void LayersPanelPresenter::ItemMoved(int ilist)
{
    int c = 1;
    int p = ActiveLayer;
    qDebug() << c << " previous:" << p;
}
void LayersPanelPresenter::ItemRenamed(QString newName)
{
    qDebug() << "rename New name:" << newName;
}

void LayersPanelPresenter::SelChanged()
{

    int layerto = GetActiveLayer();
    int layerfrom = movinglayer;

    LayerAction lact;
    lact.ActID = laMove;
    lact.layerto = layerto;
    lact.layer = layerfrom;
    emit SendLayerAction(lact);
    qDebug() << layerto << " - layerto   layerfrom - :" << movinglayer;
}

void LayersPanelPresenter::AddLayerCmd()
{

    //  ActiveLayer=MdlLayers->itemFromIndex(selmodel->selectedIndexes().at(0))->row();
    LayerAction lact;
    lact.ActID = laAdd;
    lact.layer = GetActiveLayer();
    emit SendLayerAction(lact);
}
void LayersPanelPresenter::DupLayerCmd()
{
    LayerAction lact;
    lact.ActID = laDup;
    lact.layer = GetActiveLayer();
    emit SendLayerAction(lact);
}
void LayersPanelPresenter::DelLayerCmd()
{
    // DelLayer(ActiveLayer);
    //     ActiveLayer=MdlLayers->itemFromIndex(selmodel->selectedIndexes().at(0))->row();
    LayerAction lact;
    lact.ActID = laDel;
    lact.layer = GetActiveLayer();
    qDebug() << (GetActiveLayer());
    emit SendLayerAction(lact);
}
void LayersPanelPresenter::ExecLAction(LayerAction lact)
{

    if (lact.ActID == laAdd)
    {
        AddLayerAt(lact.layer);
    }
    else if (lact.ActID == laDel)
    {
        DelLayer(lact.layer);
    }
    else if (lact.ActID == laDup)
    {
        AddLayerAt(lact.layer);
    }
    else if (lact.ActID == laDrop)
    {
        DelLayer(lact.layer);
    }
    else if (lact.ActID == laOp)
    {
        // LayerProps[lact.layer].Opacity=lact.Opacity;
    }
    else if (lact.ActID == laBm)
    {
        // LayerProps[lact.layer].blendmode=lact.bm;
    }
    else if (lact.ActID == laVis)
    {
        if (lact.vis == true)
            LayerList->item(lact.layer)->setCheckState(Qt::Checked);
        else
            LayerList->item(lact.layer)->setCheckState(Qt::Unchecked);
        // LayerProps[lact.layer].visible=lact.vis;
        // MdlLayers->item(LayerToMdl(lact.layer),0)

        LayerList->item(lact.layer)->setText((*LayerProps)[lact.layer].layerName);
    }
    else if (lact.ActID == laDropall)
    {
        LayerList->clear();
        ActiveLayer = 0;
    }
    else if (lact.ActID == laMove)
    {

        for (int i = 0; i < LayerProps->count(); i++)
            LayerList->item(i)->setText((*LayerProps)[i].layerName);
        /*
        if ((lact.layerto<ActiveLayer) & (lact.layer<ActiveLayer)) { }
        else if ((lact.layerto>ActiveLayer) & (lact.layer>ActiveLayer)) { }
        else if (lact.layer==ActiveLayer) ActiveLayer=lact.layerto;
        else if ((lact.layerto<=ActiveLayer) & (lact.layer>ActiveLayer)) ActiveLayer++; // something moved from below
        else if ((lact.layer<ActiveLayer) & (lact.layerto>=ActiveLayer)) ActiveLayer--;
    */

        // lact.layerto=CheckIndex(lact.layerto);
        // lact.layer=CheckIndex(lact.layer);
        ActiveLayer = GetActiveLayer();

        // selmodel->select(MdlLayers->item(ActiveLayer)->index(),QItemSelectionModel::ClearAndSelect);
    }
    else if (lact.ActID == laNewCanvas)
    {
        LayerList->clear();
        ActiveLayer = 0;
        for (int i = 0; i < lact.layer; i++)
        {
            AddLayerAt(0);
        }
    }
    LbIndex->setText(QString::number(ActiveLayer));
    emit ActionDone();
}

void LayersPanelPresenter::SetLock(qint8 lk)
{
    locked += lk;
    if (locked > 0)
        this->setEnabled(false);
    else
    {
        this->setEnabled(true);
        locked = 0;
    }
}

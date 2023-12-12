#include "ctl_usertools.h"
#include "BrushEditorPresenter.h"

ctl_UserTools::ctl_UserTools(BrushEditorPresenter *bc, QWidget *parent)
{
  Title->setText("Brush Presets");
  ToolsFile.setFileName(RESPATH + "/sets/Presets.dat");
  BC = bc;
  qDebug() << "user tools init begin";
  View = new QListView();
  itModel = new QStandardItemModel;
  qDebug() << "user tools init 1";
  selModel = new QItemSelectionModel(itModel);
  qDebug() << "user tools init 2";
  CtlNum = 20; // 18 now

  View->setModel(itModel);
  View->setSelectionModel(selModel);

  qDebug() << "user tools views inited";

  BtnSave = new QPushButton(this);
  BtnSave->setText("S");
  BtnSave->setCheckable(false);
  BtnDel = new QPushButton(this);
  BtnDel->setText("D");
  BtnDel->setCheckable(false);
  EdName = new QLineEdit(this);
  // EdName->setText("New Preset");

  qDebug() << ("user tools btns inited");

  LT = new QGridLayout(this);
  LT->addWidget(EdName, 0, 0, 1, 2);
  LT->addWidget(BtnSave, 1, 0);
  LT->addWidget(BtnDel, 1, 1);
  LT->addWidget(View, 2, 0, 1, 2);

  this->setLayout2(LT);
  // this->setModal(true);
  //  this->setParent(parent);
  qDebug() << "user tools layout inited";
  connect(BtnSave, SIGNAL(clicked()), this, SLOT(SaveTool()));
  connect(BtnDel, SIGNAL(clicked()), this, SLOT(DelTool()));
  connect(View, SIGNAL(clicked(QModelIndex)), this, SLOT(ClickTool(QModelIndex)));
  qDebug() << "user tools signals inited";
  LoadTools();
}

void ctl_UserTools::GetPreset(d_ToolPreset preset)
{
  CurrPreset = preset;
}

void ctl_UserTools::SaveTools()
{
  ToolsFile.open(QIODevice::ReadWrite | QIODevice::Truncate);
  ToolStream.setDevice(&ToolsFile);
  ToolStream.setFloatingPointPrecision(QDataStream::DoublePrecision);
  // ToolsFile.seek(0);

  ToolStream << (qint16)ToolsList.count();

  for (int i = 0; i < ToolsList.count(); i++)
  {
    // ToolsList.append(defTool);
    ToolStream << QString(ToolsList[i].name);
    ToolStream << (qint8)ToolsList[i].compmode;
    ToolStream << (qint8)ToolsList[i].toolid;
    ToolStream << ToolsList[i].usecolor;
    for (int k = 0; k < CtlNum; k++)
    {
      ToolStream << (qreal)ToolsList[i].pars[k].fjit;
      ToolStream << (qreal)ToolsList[i].pars[k].fmax;
      ToolStream << (qreal)ToolsList[i].pars[k].fmin;
      ToolStream << (qint8)ToolsList[i].pars[k].tmode;
    }
  }
  ToolsFile.close();
}

void ctl_UserTools::LoadTools()
{
  ToolsFile.open(QIODevice::ReadWrite);
  if (ToolsFile.exists())
  {
    ToolStream.setDevice(&ToolsFile);
    ToolStream.setFloatingPointPrecision(QDataStream::DoublePrecision);
    qDebug() << "user tools LOADING";
    qint16 count;
    d_ToolPreset defTool;
    ToolsList.clear();
    ToolsFile.seek(0);
    if (ToolsFile.size() > 0)
    {
      ToolStream >> count;
      qDebug() << ("user tools read count:" + QString::number(count));
      for (int i = 0; i < count; i++)
      {
        ToolsList.append(defTool);
        ToolStream >> ToolsList[i].name;
        ToolStream >> ToolsList[i].compmode;
        ToolStream >> ToolsList[i].toolid;
        ToolStream >> ToolsList[i].usecolor;
        for (int k = 0; k < CtlNum; k++)
        {
          ToolStream >> ToolsList[i].pars[k].fjit;
          ToolStream >> ToolsList[i].pars[k].fmax;
          ToolStream >> ToolsList[i].pars[k].fmin;
          ToolStream >> ToolsList[i].pars[k].tmode;
        }
      }
    }
    ToolsFile.close();
  }

  Remodel();
}

void ctl_UserTools::LoadToolsINI()
{
}
void ctl_UserTools::SaveToolsINI()
{
  /*
    QSettings Presets("Presets.ini",QSettings::IniFormat);

    for (int i=0;i<ToolsList.count();i++){
         //ToolsList.append(defTool);
       Presets.beginGroup(QString::number(i));
       Presets.setValue("name",QString(ToolsList[i].name);
       Presets.setValue("blendmode",(qint8)ToolsList[i].compmode);
       Presets.setValue("toolid",(qint8)ToolsList[i].toolid);
      Presets.setValue("usecol",ToolsList[i].usecolor);
      //
      for (int k=0;k<CtlNum;k++){
             ToolStream << (qreal)ToolsList[i].pars[k].fjit;
             ToolStream << (qreal)ToolsList[i].pars[k].fmax;
             ToolStream << (qreal)ToolsList[i].pars[k].fmin;
             ToolStream << (qint8)ToolsList[i].pars[k].tmode;
         }

     }
 */
}

void ctl_UserTools::ApplyTool(int idx)
{
}
void ctl_UserTools::SaveTool()
{

  CurrPreset = BC->GenPreset();
  CurrPreset.name = EdName->text();
  qint16 idx = itModel->itemFromIndex(selModel->selectedIndexes().at(0))->row();
  if (EdName->text() != ToolsList.at(idx).name)
  {
    ToolsList.append(CurrPreset);

    QStandardItem *itm = new QStandardItem((QString)CurrPreset.name);
    itModel->appendRow(itm);
    SaveTools();
  }
  else
  { // overwrite existing one

    ToolsList[idx] = CurrPreset;
    SaveTools();
  }
}

void ctl_UserTools::Remodel()
{
  itModel->clear();
  QStandardItem *itm;
  for (int i = 0; i < ToolsList.count(); i++)
  {
    itm = new QStandardItem(ToolsList.at(i).name);
    itModel->appendRow(itm);
  }
}
void ctl_UserTools::ClickTool(QModelIndex midx)
{
  int idx = itModel->itemFromIndex(midx)->row();

  BC->ApplyPreset(ToolsList.at(idx));
  EdName->setText(ToolsList.at(idx).name);
}
void ctl_UserTools::DelTool()
{
  if (selModel->selectedIndexes().count() > 0)
  {
    QModelIndex midx = selModel->selectedIndexes().at(0);

    int ridx = itModel->itemFromIndex(midx)->row();
    itModel->removeRow(ridx);
    ToolsList.removeAt(ridx);
    SaveTools();
  }
}

#include "dlg_usersearch.h"
#include <QtWidgets/QLabel>
dlg_UserSearch::dlg_UserSearch(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("User Search");
    BtnSearch=new QPushButton("Search");
    BtnMore=new QPushButton("MORE");
    BtnAdd=new QPushButton("ADD");

    ResModel=new QStandardItemModel;
    SelModel=new QItemSelectionModel(ResModel);
    EdResults=new QListView;
    EdResults->setModel(ResModel);
    EdResults->setSelectionModel(SelModel);

    EdSearch=new QLineEdit;

    QLabel *lbName=new QLabel("Name:");

    LT=new QGridLayout;
    LT->addWidget(BtnSearch,0,0);
    LT->addWidget(BtnAdd,0,2);
    LT->addWidget(BtnMore,0,3);

    QVBoxLayout *SearchParsLT=new QVBoxLayout;
    SearchParsLT->addWidget(lbName);
    SearchParsLT->addWidget(EdSearch);
    SearchParsLT->addStretch(1222);
    LT->addLayout(SearchParsLT,1,0,2,1);
    LT->addWidget(EdResults,1,1,3,3);



    lbName->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
this->setLayout(LT);

    connect(BtnSearch,SIGNAL(clicked()),this,SLOT(SearchClick()));
    connect(BtnAdd,SIGNAL(clicked()),this,SLOT(AddClick()));

 //for text database (obsolete)
    //   StStar="[A-Z,a-z,0-9]{0,12}";
  //  StChar="[A-Z,a-z,0-9]";

       StStar="%";
       StChar="_";


}
void dlg_UserSearch::SearchClick(){
    if (EdSearch->text().length()>1) {
    QString sstr=EdSearch->text();
    sstr.replace("*",StStar);
    sstr.replace("?",StChar);

        emit AskName(sstr);


    }
}
void dlg_UserSearch::GetPeopleList(QStringList stl){

    ResModel->clear();
    foreach (QString str,stl){
        QStandardItem *itm=new QStandardItem(str);
        ResModel->appendRow(itm);
    }


}
void dlg_UserSearch::AddClick(){

this->setWindowTitle("hui");
    if (SelModel->selectedIndexes().count()>0) {
        QString srch=ResModel->itemFromIndex(SelModel->selectedIndexes().at(0))->text();
  if (!srch.isNull()){

        this->setWindowTitle(srch);
    emit AskAdd(srch);
    }
    }
}

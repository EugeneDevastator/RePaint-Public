#include "dlg_newcanvas.h"
#include <QFile>
#include <QTextStream>
#include "Brushes.h"
dlg_NewCanvas::dlg_NewCanvas(ImageArray *lnk, QWidget *parent) : QDialog(parent)
{
    qDebug() << ("NewCanvas_Dialog init BEGIN");
    LinkedImage = lnk;
    this->setWindowIcon(QIcon(RESPATH + "/res/icon16.png"));
    this->setWindowTitle("Canvas Operations");
    BtnNew = new QPushButton;
    BtnCancel = new QPushButton;
    BtnRot = new QPushButton;
    BtnLock = new QToolButton;
    BtnResize = new QPushButton;
    BtnReframe = new QPushButton;

    BtnPickSize = new QPushButton;
    BtnPickProps = new QPushButton;

    BtnLock->setCheckable(true);
    BtnLock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    EdWidth = new QLineEdit;
    EdHeight = new QLineEdit;
    EdRatioW = new QLineEdit;
    EdRatioH = new QLineEdit;
    CbPresets = new QComboBox;
    QLabel *IcW = new QLabel("W");
    QLabel *IcH = new QLabel("H");

    BtnNew->setIcon(QIcon(RESPATH + "/res/btnnew.png"));
    BtnCancel->setIcon(QIcon(RESPATH + "/res/fdel.png"));
    BtnLock->setIcon(QIcon(RESPATH + "/res/btnlock.png"));
    BtnRot->setIcon(QIcon(RESPATH + "/res/btnRot.png"));
    BtnResize->setIcon(QIcon(RESPATH + "/res/btnrescale.png"));
    BtnReframe->setIcon(QIcon(RESPATH + "/res/btnreframe.png"));
    BtnPickSize->setIcon(QIcon(RESPATH + "/res/BtnPick.png"));
    BtnPickProps->setIcon(QIcon(RESPATH + "/res/BtnPick.png"));
    QPixmap picW(RESPATH + "/res/nWidth.png");
    IcW->setPixmap(picW);
    IcW->setAlignment(Qt::AlignCenter);
    // IcW->setFixedSize(24,24);
    QPixmap picH(RESPATH + "/res/nHeight.png");
    IcH->setPixmap(picH);
    IcH->setAlignment(Qt::AlignCenter);
    // IcH->setFixedSize(24,24);

    QIntValidator *vld = new QIntValidator(0, 32767, this);
    QIntValidator *vldr = new QIntValidator(0, 999, this);

    EdWidth->setValidator(vld);
    EdHeight->setValidator(vld);
    // EdWidth->setFixedWidth(92);
    // EdHeight->setFixedWidth(92);
    EdRatioW->setValidator(vldr);
    EdRatioH->setValidator(vldr);

    // EdRatioW->setFixedWidth(48);
    // EdRatioH->setFixedWidth(48);

    LT = new QGridLayout;

    LT->addWidget(BtnRot, 2, 0, 2, 1);
    LT->addWidget(CbPresets, 0, 1, 1, 3);
    LT->addWidget(IcW, 2, 1);
    LT->addWidget(IcH, 3, 1);
    LT->addWidget(BtnPickSize, 1, 2);
    LT->addWidget(BtnPickProps, 1, 3);

    LT->addWidget(EdWidth, 2, 2);
    LT->addWidget(EdHeight, 3, 2);

    LT->addWidget(EdRatioW, 2, 3);
    LT->addWidget(EdRatioH, 3, 3);
    LT->addWidget(BtnCancel, 4, 3);
    LT->addWidget(BtnNew, 4, 2);
    LT->addWidget(BtnResize, 4, 1);
    //   LT->addWidget(BtnReframe,4,0);
    LT->addWidget(BtnLock, 2, 4, 2, 1);
    setLayout(LT);
    LT->setAlignment(Qt::AlignCenter);

    LT->setColumnStretch(0, 24); // flip
    LT->setColumnStretch(1, 24); // icons
    LT->setColumnStretch(2, 92); // width
    LT->setColumnStretch(3, 48); // ratio
    LT->setColumnStretch(4, 24); // lock

    LT->setRowStretch(0, 10); // sets
    LT->setRowStretch(1, 10); // pick
    LT->setRowStretch(2, 10); // w
    LT->setRowStretch(3, 10); // h
    LT->setRowStretch(4, 10); // new
    //  LT->setRowStretch(5,10);
    // BtnNew->setFixedSize(48,48/1.66666);
    // BtnReframe->setFixedSize(48,48/1.66666);
    // BtnResize->setFixedSize(48,48/1.66666);
    // BtnCancel->setFixedSize(48,48/1.66666);

    //   foreach (QObject *w,LT->children() ){
    //     QWidget *ob=(QWidget*)w;
    //      ob->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //  }

    QList<QWidget *> myList;
    for (int i = 0; i < LT->count(); i++)
    {
        if (QWidgetItem *myItem = dynamic_cast<QWidgetItem *>(LT->itemAt(i)))
        {
            myList << myItem->widget();
        }
    }
    for (int i = 0; i < myList.count(); i++)
    {
        QWidget *wid = qobject_cast<QWidget *>(myList.at(i));
        if (wid)
            wid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //    qDebug() << wid->objectName();
    }

    BtnNew->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(BtnNew, SIGNAL(clicked()), this, SLOT(NewClick()));
    connect(BtnResize, SIGNAL(clicked()), this, SLOT(RescaleClick()));
    connect(BtnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(CbPresets, SIGNAL(currentIndexChanged(int)), this, SLOT(SetPresetRes(int)));

    connect(BtnPickSize, SIGNAL(clicked()), this, SLOT(PickSize()));
    connect(BtnPickProps, SIGNAL(clicked()), this, SLOT(PickProps()));

    QFile savedset(RESPATH + "/sets/sizes.dat");
    QTextStream preslines;

    // savedset.open(QIODevice::ReadWrite|QIODevice::Truncate);
    savedset.open(QIODevice::ReadOnly);
    preslines.setDevice(&savedset);
    QString ReadLine;
    ReadLine = preslines.readLine();
    while (!ReadLine.isEmpty())
    {
        CbPresets->addItem(ReadLine);
        ReadLine = preslines.readLine();
    }
    connect(EdWidth, SIGNAL(textEdited(QString)), this, SLOT(RecalcH()));
    connect(EdRatioH, SIGNAL(textEdited(QString)), this, SLOT(RecalcH()));
    connect(EdHeight, SIGNAL(textEdited(QString)), this, SLOT(RecalcW()));
    connect(EdRatioW, SIGNAL(textEdited(QString)), this, SLOT(RecalcW()));
    connect(BtnRot, SIGNAL(clicked()), this, SLOT(Rot90()));
}

void dlg_NewCanvas::NewClick()
{
    int wd = EdWidth->text().toInt();
    int hg = EdHeight->text().toInt();

    if ((wd > 0) & (hg > 0))
    {
        QSize sz;
        sz.setWidth(EdWidth->text().toInt());
        sz.setHeight(EdHeight->text().toInt());
        emit SendSize(sz);
        this->accept();
    }
}

void dlg_NewCanvas::SetPresetRes(int idx)
{

    QString WHstr = CbPresets->currentText().split(" ").at(0);
    QString Wstr = WHstr.split('x').at(0);
    QString Hstr = WHstr.split('x').at(1);

    EdWidth->setText(Wstr);
    EdHeight->setText(Hstr);
    int w = Wstr.toInt();
    int h = Hstr.toInt();
    QPoint whr;
    whr = WHratio(QPoint(w, h));

    EdRatioW->setText(QString::number(whr.x()));
    EdRatioH->setText(QString::number(whr.y()));
}

void dlg_NewCanvas::RecalcW()
{
    if (BtnLock->isChecked())
    {
        int neww = EdHeight->text().toInt() * EdRatioW->text().toInt() / EdRatioH->text().toInt();
        EdWidth->setText(QString::number(neww));
    }
    else
    {
        int w = EdWidth->text().toInt();
        int h = EdHeight->text().toInt();
        QPoint whr;
        whr = WHratio(QPoint(w, h));
        EdRatioW->setText(QString::number(whr.x()));
        EdRatioH->setText(QString::number(whr.y()));
    }
}
void dlg_NewCanvas::RecalcH()
{
    if (BtnLock->isChecked())
    {

        int neww = EdWidth->text().toInt() * EdRatioH->text().toInt() / EdRatioW->text().toInt();
        EdHeight->setText(QString::number(neww));
    }
    else
    {
        int w = EdWidth->text().toInt();
        int h = EdHeight->text().toInt();
        QPoint whr;
        whr = WHratio(QPoint(w, h));
        EdRatioW->setText(QString::number(whr.x()));
        EdRatioH->setText(QString::number(whr.y()));
    }
}
void dlg_NewCanvas::Rot90()
{
    QString tmpw = EdWidth->text();
    QString tmpwr = EdRatioW->text();

    EdWidth->setText(EdHeight->text());
    EdRatioW->setText(EdRatioH->text());
    EdHeight->setText(tmpw);
    EdRatioH->setText(tmpwr);
}
void dlg_NewCanvas::PickProps()
{
    QPoint whr = WHratio(QPoint(LinkedImage->MasterSize.width(), LinkedImage->MasterSize.height()));
    EdRatioW->setText(QString::number(whr.x()));
    EdRatioH->setText(QString::number(whr.y()));
}

void dlg_NewCanvas::PickSize()
{
    EdWidth->setText(QString::number(LinkedImage->MasterSize.width()));
    EdHeight->setText(QString::number(LinkedImage->MasterSize.height()));
}

void dlg_NewCanvas::RescaleClick()
{
    QSize sz;
    sz.setWidth(EdWidth->text().toInt());
    sz.setHeight(EdHeight->text().toInt());
    emit SendRescale(sz, 0);
    this->accept();
}

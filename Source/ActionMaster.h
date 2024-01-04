#ifndef ACTIONMASTER_H
#define ACTIONMASTER_H

#include <QThread>
#include "Brushes.h"
#include "ArtMaster.h"
#include "strokemaster.h"
#include "imagearray.h"
#include "layerstack.h"


enum eActionTypes{
    atStroke,
    atAction,
    atLAction,
    atSect,
    atCanvasAction
};

struct logpair{
public:
    quint8 logid;
    QByteArray szdAction;
};

struct FullStroke{
    QList <StrokeSection> Sections;
};




class ActionMaster : public QThread
{
    Q_OBJECT
public:
    explicit ActionMaster(ImageArray *iar,bool forcesinglecore=false,QObject *parent = 0);
    QList <ArtThread*> ARTList;
    StrokeMaster *STM;
    ImageArray *MainImage;
    bool isLogging;
    bool executing;
    bool GlobalLock;
    bool LActionBusy;
    QTimer *uptimer;


   QList <logpair> LOG;
   QList <LayerStack*> *LStacks;
   QList <dotList> LDotLists;
   QList <LayerAction> LAStack;

    QSize baseSize;
        quint8 baselcount;
        quint8 atcount;  //artthreads count
        quint8 maxthreads;

signals:
    void SendLAction(LayerAction);
    void SendSection(StrokeSection);
public slots:

        void ExecOperation(quint8 OpType,QByteArray Data);
        void ExecSection(StrokeSection Sect);
        void ExecNetSection(StrokeSection Sect);
        void ExecLayerAction(LayerAction lact);
        //      void ExecSection
    void NewImg(QSize sz,int lcount=1);


 //   void OpenImg();
    void OpenImg(QString fname);
    void OpenImg(QIODevice *iodev);
    void OpenImgBa(QByteArray ba);
    void ImportImg(QString fname);

    void ParseSections();
    void ParseLActions();

    void NewLog();
    void LogAct(ActionData act);
    void LogLAct(LayerAction act);
    void LogSect(StrokeSection sect);
    void OpenLog(QString Fname);
    void OpenLog(QIODevice *iodev);

    void SaveLog(QString Fname);
    void SaveLog(QIODevice *iodev);

    void ConfirmLAction();
    void UnfoldStrokeToDots(StrokeSection Sect);
};

#endif // ACTIONMASTER_H

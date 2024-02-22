#ifndef ACTIONMASTER_H
#define ACTIONMASTER_H

#include <QThread>
#include "Brushes.h"
#include "ArtMaster.h"
#include "strokemaster.h"
#include "imagearray.h"
#include "LayerActionQueue.h"


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
    QList <ArtThread*> ArtThreads;
    StrokeMaster *STM;
    ImageArray *MainImage;
    bool isLogging;
    bool executing;
    bool GlobalLock;
    bool LActionBusy;
    QTimer *uptimer;


   QList <logpair> LOG;
   QList <LayerActionQueue*> *ActionsPerLayer;
   QList <dotList> LDotLists;
   QList <LayerOperation> LAStack;

    QSize baseSize;
        quint8 baselcount;
        quint8 atcount;  //artthreads count
        quint8 maxthreads;

signals:
    void SendLAction(LayerOperation);
    void SendSection(StrokeSection);
public slots:

        void ExecOperation(quint8 OpType,QByteArray Data);
        void ExecSection(StrokeSection Sect);
        void ExecNetSection(StrokeSection Sect);
        void ExecLayerAction(LayerOperation lact);
        //      void ExecSection
    void NewImg(QSize sz,int lcount=1);


 //   void OpenImg();
    void OpenImg(QString fname);
    void OpenImg(QIODevice *iodev);
    void OpenImgBa(QByteArray ba);
    void ImportImg(QString fname);

    void ParseSections();
    void ParseLayerActions();

    void NewLog();
    void LogAct(ActionData act);
    void LogLAct(LayerOperation act);
    void LogSect(StrokeSection sect);
    void OpenLog(QString Fname);
    void OpenLog(QIODevice *iodev);

    void SaveLog(QString Fname);
    void SaveLog(QIODevice *iodev);

    void ConfirmLAction();
    void UnfoldStrokeToDots(StrokeSection Sect);
};

#endif // ACTIONMASTER_H

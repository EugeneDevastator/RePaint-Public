#ifndef ARTTHREAD_H
#define ARTTHREAD_H

#include <QThread>
#include "ArtMaster.h"
#include "imagearray.h"
#include "layerstack.h"


class ArtThread : public QThread
{
    Q_OBJECT
public:

    ArtMaster *ARTM;

    explicit ArtThread(ImageArray *mimg,QObject *parent = 0);
    void run();

    int corecount;
    ImageArray * MainImage;


bool isdone;
    bool *GlobalLock;
    bool DrawLocked;
    bool locked;
    bool singlecore;
    bool executing;
    bool suspended;
    quint8 *tcount;
    quint8 threadID; //thread id;
    QTimer *uptimer;

    QList <actionpair> ActionList;
    QList <actionpair> ActionNetList;
    QList <LayerStack*> *LStacks;



public slots:
    void ForceSingleCore();
    void callupdate();
    void ExecAction(QImage *img, ActionData Act, bool local=true);
    void ExecActionList(QList <actionpair> APlist);

signals:
    
public slots:
    
};

#endif // ARTTHREAD_H

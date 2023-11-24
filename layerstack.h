#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "Brushes.h"


typedef QList <d_Section> sectionList;
typedef QList <d_Action> dotList;

class LayerStack : public QObject
{
Q_OBJECT
public:
    explicit LayerStack();
    bool isLocked;
    bool isForDel;
    bool canDel;


signals:
    
public slots:
    dotList* GetLocalDots();
    sectionList GetLocalSects();
    void addLocalDot(d_Action act);
    void addLocalDotList(dotList dtLst);
    void addLocalSect(d_Section sect);

private:
    sectionList *SectsLocal;
    sectionList *SectsNet;
    dotList *DotsLocal;
    dotList *DotsNet;

    
};

#endif // LAYERSTACK_H

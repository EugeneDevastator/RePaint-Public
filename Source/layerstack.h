#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "Brushes.h"


typedef QList <StrokeSection> sectionList;
typedef QList <ActionData> dotList;

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
    void addLocalDot(ActionData act);
    void addLocalDotList(dotList dtLst);
    void addLocalSect(StrokeSection sect);

private:
    sectionList *SectsLocal;
    sectionList *SectsNet;
    dotList *DotsLocal;
    dotList *DotsNet;

      void addNetSect(StrokeSection sect);
};

#endif // LAYERSTACK_H

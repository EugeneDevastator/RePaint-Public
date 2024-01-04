#include "layerstack.h"

LayerStack::LayerStack() {
    isForDel = false;
    canDel = false;
    SectsLocal = new sectionList;
    SectsNet = new sectionList;
    isLocked = false;
    DotsLocal = new dotList;
    DotsNet = new dotList;
}

dotList *LayerStack::GetLocalDots() {
    while (isLocked) {
        QApplication::processEvents();
    }
    {
        isLocked = true;
        dotList *Dotscopy = new dotList(*DotsLocal);
        DotsLocal->clear();
        isLocked = false;
        return Dotscopy;
    }

}

sectionList LayerStack::GetLocalSects() {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    sectionList Sectscopy = *SectsLocal;
    SectsLocal->clear();
    isLocked = false;
    return Sectscopy;

}

void LayerStack::addLocalDot(ActionData act) {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    DotsLocal->append(act);
    isLocked = false;
}

void LayerStack::addLocalDotList(dotList dtLst) {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    DotsLocal->append(dtLst);
    isLocked = false;
}

void LayerStack::addLocalSect(StrokeSection sect) {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    SectsLocal->append(sect);
    isLocked = false;
}

void LayerStack::addNetSect(StrokeSection sect) {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    SectsNet->append(sect);
    isLocked = false;
}
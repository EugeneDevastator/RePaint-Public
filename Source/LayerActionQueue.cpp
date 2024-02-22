#include "LayerActionQueue.h"

LayerActionQueue::LayerActionQueue() {
    isForDel = false;
    canDel = false;
    SectsLocal = new sectionList;
    SectsNet = new sectionList;
    isLocked = false;
    DotsLocal = new dotList;
    DotsNet = new dotList;
}

dotList *LayerActionQueue::GetLocalDots() {
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

sectionList LayerActionQueue::GetLocalSects() {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    sectionList Sectscopy = *SectsLocal;
    SectsLocal->clear();
    isLocked = false;
    return Sectscopy;

}

void LayerActionQueue::addLocalDot(ActionData act) {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    DotsLocal->append(act);
    isLocked = false;
}

void LayerActionQueue::addLocalDotList(dotList dtLst) {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    DotsLocal->append(dtLst);
    isLocked = false;
}

void LayerActionQueue::addLocalSect(StrokeSection sect) {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    SectsLocal->append(sect);
    isLocked = false;
}

void LayerActionQueue::addNetSect(StrokeSection sect) {
    while (isLocked) {
        QApplication::processEvents();
    }
    isLocked = true;
    SectsNet->append(sect);
    isLocked = false;
}
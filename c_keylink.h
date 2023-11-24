#ifndef C_KEYLINK_H
#define C_KEYLINK_H

#include <QObject>
#include <QHash>
#include <QKeyEvent>
#include <QFlags>
#include <QtGlobal>
#include <QDebug>

enum enKeyevs{
    ekCPICK,
    ekBrushSizeP,
    ekBrushSizeM,
    ekCrvP,
    ekCrvM,
    ekHueP,
    ekHueM,
    ekSatP,
    ekSatM,
    ekLitP,
    ekLitM,

    ekCanvasMove,
    ekCanvasPick,
    ekCanvasZoomIn,
    ekCanvasZoomOut,
    ekFastBrush,
    ekTPresets,
    ekPanelhide,

    ekFitScreen,
    ekTouchScreen,
    ekActualPixels,

    ekSTOP
    };

struct KMpair{
public:
   KMpair(int m,int k);
   int mods;
   int key;
};

class c_KeyLink : public QObject
{

    Q_OBJECT
public:
    explicit c_KeyLink(QObject *parent = 0);
    QHash<qint64,int> KKlinks; // user defined key links to the key-event value. first hash val is identifier(string forex)

    QHash<qint64,int> KElinks; // event value links t1
    bool KBstate[40];


signals:
    
public slots:
    void PressKey(QKeyEvent *event);
    void UnPressKey(QKeyEvent *event);
    void PressText(QString txt);
    void UnPressText(QString txt);
    void UnPressAll();
 //   void ParseKeyEvent(QKeyEvent *event);
    qint64 Compose(qint32 mods,qint32 key);
};

#endif // C_KEYLINK_H

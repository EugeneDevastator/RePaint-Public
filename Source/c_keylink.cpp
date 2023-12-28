#include "c_keylink.h"


KMpair::KMpair(int m, int k){
    mods=m;
    key=k;
}

c_KeyLink::c_KeyLink(QObject *parent) :
    QObject(parent)
{

    qDebug()<<("KEYM:begin init");


    /*
    Qt::NoModifier	0x00000000	No modifier key is pressed.
    Qt::ShiftModifier	0x02000000	A Shift key on the keyboard is pressed.
    Qt::ControlModifier	0x04000000	A Ctrl key on the keyboard is pressed.
    Qt::AltModifier	0x08000000	An Alt key on the keyboard is pressed.
    Qt::MetaModifier	0x10000000	A Meta key on the keyboard is pressed.
    Qt::KeypadModifier	0x20000000	A keypad button is pressed.
    Qt::GroupSwitchModifier	0x40000000	X11 only. A Mode_switch key on the keyboard is pressed.
    */
    //convert item event into qint32


    for (int i=0;i<ekSTOP;i++) KBstate[i]=false;
qDebug()<<("KEYM:zeroed states");

//QFlag(Qt::AltModifier).operator int()
    //KKlinks[Compose(Qt::AltModifier,0)]=ekCanvasPick;

    KKlinks.insert(Compose(Qt::ShiftModifier,Qt::Key_Shift),ekFastBrush);
    KKlinks.insert(Compose(0,Qt::Key_CapsLock),ekBrushRegulation);
    KKlinks.insert(Compose(0,Qt::Key_QuoteLeft),ekCPICK);
    KKlinks.insert(Compose(0,Qt::Key_BracketLeft),ekBrushSizeM);
    KKlinks.insert(Compose(0,Qt::Key_BracketRight),ekBrushSizeP);
    KKlinks.insert(Compose(0,Qt::Key_Q),ekHueP);
    KKlinks.insert(Compose(0,Qt::Key_A),ekHueM);
    KKlinks.insert(Compose(0,Qt::Key_W),ekSatP);
    KKlinks.insert(Compose(0,Qt::Key_S),ekSatM);
    KKlinks.insert(Compose(0,Qt::Key_E),ekLitP);
    KKlinks.insert(Compose(0,Qt::Key_D),ekLitM);

    KKlinks.insert(Compose(0,Qt::Key_Z),ekTPresets);
    KKlinks.insert(Compose(0,Qt::Key_X),ekPanelhide);

    KKlinks.insert(Compose(Qt::AltModifier,Qt::Key_Alt),ekCanvasPick);
    KKlinks.insert(Compose(Qt::AltModifier,Qt::Key_AltGr),ekCanvasPick);
    KKlinks.insert(Compose(0,Qt::Key_Space),ekCanvasMove);
    KKlinks.insert(Compose(Qt::ControlModifier,Qt::Key_Control),ekCanvasZoomIn);
    KKlinks.insert(Compose(Qt::ControlModifier|Qt::ShiftModifier,Qt::Key_Space),ekCanvasZoomOut);
  //  KKlinks.insert(Compose(Qt::ShiftModifier,Qt::Key_Shift),ekFastBrush);
    KKlinks.insert(Compose(Qt::ControlModifier,Qt::Key_0),ekFitScreen);
    KKlinks.insert(Compose(Qt::ControlModifier|Qt::AltModifier,Qt::Key_0),ekActualPixels);
    KKlinks.insert(Compose(Qt::AltModifier,Qt::Key_0),ekTouchScreen);

qDebug()<<("KEYM:end init");

}
void c_KeyLink::PressKey(QKeyEvent *event){
    qint32 k=event->key();
    qint32 m=event->modifiers();
    qint64 c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=true;
}

void c_KeyLink::UnPressAll(){
    for (int i=0;i<ekSTOP;i++) KBstate[i]=false;

}

void c_KeyLink::UnPressKey(QKeyEvent *event){
    qint32 k=event->key();
    qint32 m=event->modifiers();
    qint64 c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;

    m=Qt::AltModifier;
    c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;
    m=Qt::ShiftModifier;
    c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;
    m=Qt::ControlModifier;
     c=Compose(m,k);

    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;
    m=Qt::ControlModifier|Qt::ShiftModifier;
     c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;
    m=Qt::ControlModifier|Qt::AltModifier;
     c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;

    m=Qt::ShiftModifier|Qt::AltModifier;
     c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;


    m=Qt::ShiftModifier|Qt::ControlModifier|Qt::AltModifier;
     c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;


    m=Qt::NoModifier;
     c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;
    m=0;
     c=Compose(m,k);
    if (KKlinks.contains(c)) KBstate[KKlinks.value(c)]=false;


}
void c_KeyLink::PressText(QString txt){

   // if (KKlinks.contains(k)) KBstate[KKlinks.value(k)]=true;


}
void c_KeyLink::UnPressText(QString txt){
  //  if (KKlinks.contains(k)) KBstate[KKlinks.value(k)]=false;
}
qint64 c_KeyLink::Compose(qint32 mods, qint32 key){

    qint64 ret=0;
    //ret=key;
    //ret=ret+(mods>>8);
    //((qint32*)&ret)[0]=key;
    //((qint32*)&ret)[1]=mods;
    ret=key;
    qint64 modsx=mods;
    modsx*=256;
    ret=ret+modsx;
return ret;
}


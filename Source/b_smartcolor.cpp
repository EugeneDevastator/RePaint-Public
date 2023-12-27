#include "b_smartcolor.h"

b_SmartColor::b_SmartColor(DialModel *hueDial, DialModel *satDial, DialModel *litDial, QObject *parent):
HueDial(hueDial), SatDial(satDial), LitDial(litDial)
{
    savehue = 0;
    savesat = 0;
    savelit = 0;
    UseCol = Qt::black;
    connect(HueDial,SIGNAL(ChangedSignal()),this,SLOT(RefreshColor()));
}
void b_SmartColor::RefreshColor(){
    savesat = SatDial->GetValueInRange(1);
    savehue = HueDial->GetValueInRange(1);
    savelit = LitDial->GetValueInRange(1);
    UseCol = QColor::fromHslF(savehue, savesat, savelit);
    emit NewColor(UseCol);
}

void b_SmartColor::LoadAll()
{
    UseCol = QColor::fromHslF(savehue, savesat, savelit);
    emit NewColor(UseCol);
}
void b_SmartColor::SetSatF(float inp)
{
    savesat = inp;
    UseCol = QColor::fromHslF(savehue, savesat, savelit);
    emit NewColor(UseCol);
}
void b_SmartColor::SetHueF(float inp)
{
    savehue = inp;
    UseCol = QColor::fromHslF(savehue, savesat, savelit);
    emit NewColor(UseCol);
}
void b_SmartColor::SetLitF(float inp)
{
    savelit = inp;
    UseCol = QColor::fromHslF(savehue, savesat, savelit);
    emit NewColor(UseCol);
}
void b_SmartColor::SetCol(QColor col)
{
    if ((col.lightnessF() == 0) | (col.lightnessF() == 1))
    {
        savelit = col.lightnessF();
    }
    else if ((col.saturationF() == 0))
    {
        savesat = col.hslSaturationF();
        savelit = col.hslSaturationF();
    }
    else
    {
        savehue = col.hueF();
        savesat = col.hslSaturationF();
        savelit = col.lightnessF();
    }
    UseCol = QColor::fromHslF(savehue, savesat, savelit);
    emit NewColor(UseCol);
}



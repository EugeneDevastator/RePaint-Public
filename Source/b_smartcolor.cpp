#include "b_smartcolor.h"

b_SmartColor::b_SmartColor(QObject *parent) : QObject(parent)
{
    savehue = 0;
    savesat = 0;
    savelit = 0;
    UseCol = Qt::black;
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

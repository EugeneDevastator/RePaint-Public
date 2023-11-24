#ifndef B_SMARTCOLOR_H
#define B_SMARTCOLOR_H

#include <QObject>
#include <QColor>

class b_SmartColor : public QObject
{
    Q_OBJECT
public:
    explicit b_SmartColor(QObject *parent = 0);
    QColor UseCol;
    float savehue;
    float savesat;
    float savelit;
private:

public slots:
    void SetHueF(float inp);
    void SetSatF(float inp);
    void SetLitF(float inp);
    void LoadAll();
    //void SaveAll();
    void SetCol(QColor col);
protected:
signals:
    void NewColor(QColor col);
    
};

#endif // B_SMARTCOLOR_H

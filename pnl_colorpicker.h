#ifndef PNL_COLORPICKER_H
#define PNL_COLORPICKER_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
class pnl_ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit pnl_ColorPicker(QWidget *parent = 0);
   QGridLayout *LT;
signals:
    
public slots:
    
};

#endif // PNL_COLORPICKER_H

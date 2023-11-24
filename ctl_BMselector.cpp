#include "ctl_BMselector.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QApplication>
ctl_BMselector::ctl_BMselector(QWidget *parent) :
    QWidget(parent)
{
    BMnames.append("Normal");         BMmodes.append(QPainter::CompositionMode_SourceOver);
    BMnames.append("Erase");          BMmodes.append(QPainter::CompositionMode_DestinationOut);
    BMnames.append("Over");           BMmodes.append(QPainter::CompositionMode_SourceAtop);
    BMnames.append("Under");          BMmodes.append(QPainter::CompositionMode_DestinationOver);
    BMnames.append("Plus");           BMmodes.append(QPainter::CompositionMode_Plus);
    BMnames.append("ColorDodge");     BMmodes.append(QPainter::CompositionMode_ColorDodge);
    BMnames.append("Screen");         BMmodes.append(QPainter::CompositionMode_Screen);
    BMnames.append("Lighten");        BMmodes.append(QPainter::CompositionMode_Lighten);
    BMnames.append("ColorBurn");      BMmodes.append(QPainter::CompositionMode_ColorBurn);
    BMnames.append("Multiply");       BMmodes.append(QPainter::CompositionMode_Multiply);
    BMnames.append("Darken");         BMmodes.append(QPainter::CompositionMode_Darken);
    BMnames.append("Overlay");        BMmodes.append(QPainter::CompositionMode_Overlay);
    BMnames.append("HardLight");      BMmodes.append(QPainter::CompositionMode_HardLight);
    BMnames.append("SoftLight");      BMmodes.append(QPainter::CompositionMode_SoftLight);
    BMnames.append("Xor");            BMmodes.append(QPainter::CompositionMode_Xor);
    BMnames.append("Difference");     BMmodes.append(QPainter::CompositionMode_Difference);
    BMnames.append("Exclusion");      BMmodes.append(QPainter::CompositionMode_Exclusion);


    combo=new QComboBox;
    foreach(QString str,BMnames){
        combo->addItem(str);
    }

    QPixmap *pic=new QPixmap;

    pic->load(RESPATH+"/res/ctlbm.png");
    QLabel *icn=new QLabel;
    icn->setScaledContents(true);
    icn->setPixmap(*pic);
    icn->setFixedSize(32,32);

    QHBoxLayout *lt=new QHBoxLayout;
    lt->addWidget(icn);
    lt->addWidget(combo);
    lt->setMargin(0);
    lt->setSpacing(0);

    combo->setMaxVisibleItems(999);
    setLayout(lt);
    connect (combo,SIGNAL(activated(int)),this,SLOT(SendChangedIdx(int)));
combo->setFocusProxy(this);

}
void ctl_BMselector::setBMbyint(int m){
    if (combo->currentIndex()!= m)
        combo->setCurrentIndex(m);
}

int ctl_BMselector::GetIdx(){
    return combo->currentIndex();
}

void ctl_BMselector::SendChangedIdx(int idx){
    emit BMchanged(idx);
}

QPainter::CompositionMode ctl_BMselector::GetCMfromIndex(int idx){
    return BMmodes.at(idx);
}

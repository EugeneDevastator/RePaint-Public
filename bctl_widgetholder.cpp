#include "bctl_widgetholder.h"
#include <QtWidgets/QDesktopWidget>
//<qdesktopwidget.h>

bctl_WidgetHolder::bctl_WidgetHolder(QWidget *parent) :
    QWidget(parent)
{
    Title=new QLabel("widgetholder yay!");
    Title->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    Title->setFrameShape(QFrame::StyledPanel);
    Title->setFrameShadow(QFrame::Plain);
    Title->setFixedHeight(32);
    LT=new QBoxLayout(QBoxLayout::TopToBottom);
    this->setParent(parent);
    this->setWindowFlags(Qt::SubWindow|Qt::CustomizeWindowHint|Qt::WindowMinimizeButtonHint);
  //  this->show();
    this->setWindowTitle("WIDGETHOLDER!");
 //   Title.setText("TITLE OF WIDGET");
    LT->setSpacing(0);
    LT->setMargin(6);
  //  LT->addWidget(&Title);
this->setLayout(LT);

  // PARENT=(QWidget *)parent;
}

QRect bctl_WidgetHolder::AlignWidgets(QWidget *actor, QWidget *stator){

    int snapw=actor->width()/2;
    int snaph=actor->height()/2;
    snapw=qMin(snapw,20);
    snaph=qMin(snaph,20);

      QRect newgeo(actor->geometry());
      //left to left;
      int dll=abs(actor->geometry().x()-stator->geometry().x());
      if (dll<snapw) newgeo.moveLeft(stator->x());

      //right to left
      int drl=abs(actor->geometry().topRight().x()-stator->geometry().x());
      if (drl<snapw) newgeo.moveLeft(stator->geometry().x()-actor->width());

      // right to right
      int drr=abs(stator->geometry().right()-actor->geometry().right());
      if (drr<snapw) newgeo.moveRight(stator->geometry().right());

      //left to right

      int dlr=abs(stator->geometry().right()-actor->geometry().left());
      if (dlr<snapw) newgeo.moveLeft(stator->geometry().right());

//-------------------- ygreks
int titleh=actor->frameGeometry().height()-actor->geometry().height();
int stitleh=stator->frameGeometry().height()-stator->geometry().height();

      //top to top
      int dtt=abs(actor->geometry().top()-titleh-stator->geometry().top());
      if (dtt<snaph) newgeo.moveTop(stator->geometry().top()+titleh);

      //top to bott
      int dtb=abs(actor->geometry().top()-titleh-stator->geometry().bottom());
      if (dtb<snaph) newgeo.moveTop(stator->geometry().bottom()+titleh);

      //bott to bott

      int dbb=abs(actor->geometry().bottom()-stator->geometry().bottom());
      if(dbb<snaph) newgeo.moveBottom(stator->geometry().bottom());

      // bott to top
      int dbt=abs(actor->geometry().bottom()-stitleh-stator->geometry().top());
      if(dbt<snaph) newgeo.moveBottom(stator->geometry().top()-stitleh);
//if (newgeo!=stator->geometry())
    //  actor->setGeometry(newgeo);
      return newgeo;
}

QRect bctl_WidgetHolder::AlignWidgets(QRect actor, QWidget *stator){

    int snapw=actor.width()/2;
    int snaph=actor.height()/2;
    snapw=qMin(snapw,20);
    snaph=qMin(snaph,20);

      QRect newgeo(actor);
      //left to left;
      int dll=abs(actor.left()-stator->geometry().x());
      if (dll<snapw) newgeo.moveLeft(stator->x());

      //right to left
      int drl=abs(actor.right()-stator->geometry().x());
      if (drl<snapw) newgeo.moveLeft(stator->geometry().x()-actor.width());

      // right to right
      int drr=abs(stator->geometry().right()-actor.right());
      if (drr<snapw) newgeo.moveRight(stator->geometry().right());

      //left to right

      int dlr=abs(stator->geometry().right()-actor.left());
      if (dlr<snapw) newgeo.moveLeft(stator->geometry().right());

//-------------------- ygreks
    //  int titleh=actor->frameGeometry().height()-actor->geometry().height();
    //  int stitleh=stator->frameGeometry().height()-stator->geometry().height();

      //top to top
      int dtt=abs(actor.top()-stator->geometry().top());
      if (dtt<snaph) newgeo.moveTop(stator->geometry().top());

      //top to bott
      int dtb=abs(actor.top()-stator->geometry().bottom());
      if (dtb<snaph) newgeo.moveTop(stator->geometry().bottom());

      //bott to bott

      int dbb=abs(actor.bottom()-stator->geometry().bottom());
      if(dbb<snaph) newgeo.moveBottom(stator->geometry().bottom());

      // bott to top
      int dbt=abs(actor.bottom()-stator->geometry().top());
      if(dbt<snaph) newgeo.moveBottom(stator->geometry().top());
//if (newgeo!=stator->geometry())
return newgeo;
      //return 0;
}


void bctl_WidgetHolder::moveEvent(QMoveEvent *event){
/*
    if (parent()!=0){
        PARENT=(QWidget *)this->parent();
        QWidget *ch;
        //this->geometry();
        QRect tmpgeo=AlignWidgets(this,PARENT);
}
//------------------
        for (int i=0;i<PARENT->children().count()-1;i++){
            ch=(QWidget*)PARENT->children().at(i);

        tmpgeo=AlignWidgets(tmpgeo,ch);
       }*/

      // if (this->geometry()!=tmpgeo) this->setGeometry(tmpgeo);

       // this->setWindowTitle(QString::number(QDesktopWidget().width()));





}
void bctl_WidgetHolder::mousePressEvent(QMouseEvent *event){
//   MPosOrigin=event->pos();
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void bctl_WidgetHolder::mouseMoveEvent(QMouseEvent * event){
 if (this->windowFlags() == (this->windowFlags()|Qt::Tool))
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
    /*
    if (parent()!=0){
//    PARENT=(QWidget *)this->parent();
//    QPoint ppnt=PARENT->pos();
    QPoint tpnt=this->pos();
    QPoint Delta=event->pos()-MPosOrigin;
    QPoint TargetPos(tpnt.x()+Delta.x(),tpnt.y()+Delta.y());

    this->move(TargetPos);
    MPosOrigin=event->pos();
    this->setWindowTitle(QString::number(PARENT->geometry().x()));

    }
*/

}
void bctl_WidgetHolder::setLayout2(QLayout *lt){
    QWidget *Container=new QWidget;
    Container->setLayout(lt);
    LT->addWidget(Title);
    LT->addWidget(Container);

}


void bctl_WidgetHolder::paintEvent(QPaintEvent *){

}

void bctl_WidgetHolder::keyPressEvent(QKeyEvent *event){
   // PARENT->keyPressEvent(event);
}
void bctl_WidgetHolder::keyReleaseEvent(QKeyEvent *event){
   // PARENT->keyReleaseEvent(event);
}

/*
    RePaint, the network painting tool
    Copyright (C) 2013-2023 Yauheni Bolshakov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
  */

#include "ArtThread.h"

ArtThread::ArtThread(ImageArray *mimg, QObject *parent) :
    QThread(parent)
{
    ARTM=new ArtMaster();
MainImage=mimg;
threadID=0;
    singlecore=false;
    executing=false;
    suspended=false;
    DrawLocked=false;
    locked=false;
    isdone=true;
    ActionList.clear();
    ActionNetList.clear();


    corecount= QThread::idealThreadCount ();
    if (corecount <2)  {
       ForceSingleCore();
        }


    connect(ARTM,SIGNAL(SendDirtyRect(QRect)),MainImage,SLOT(RepaintImgRect(QRect)));



    qDebug() <<("ARTT:cinited");
}
void ArtThread::run(){
/*
    if (!locked) {
        if (!executing){
            executing=true;
            if (!suspended) {
                while ((!ActionList.isEmpty())|(!ActionNetList.isEmpty())) {
                    if (!ActionList.isEmpty()){
                             actionpair ap=ActionList.first();
                             ARTM->DoAction(ap.img,ap.Act,ap.islocal);
                             ActionList.removeFirst();
                        }
                    if (!ActionNetList.isEmpty()){
                        actionpair ap=ActionNetList.first();
                        ARTM->DoAction(ap.img,ap.Act,ap.islocal);
                        ActionNetList.removeFirst();
                        }

                    }
                }
            }
        executing=false;
         }

*/
   if (!locked) {
        if (!executing){
            executing=true;

            if (!suspended) {

                for (int i=threadID;i<LStacks->count();i=i+(*tcount))
                {

                    dotList* LocalDots;
                    if ((*LStacks)[i]->isForDel){
                    }
                     LocalDots=(*LStacks)[i]->GetLocalDots();

                     while (!LocalDots->isEmpty())

                     {


                   // if (!LocalDots->isEmpty())
                        ARTM->DoActionList(&MainImage->ViewCanvas[i],LocalDots,true);
                        LocalDots=(*LStacks)[i]->GetLocalDots();
                        }

/*
                    while (!(*LStacks)[i]->DotsNet->isEmpty()){
                     //   if (*GlobalLock) return;
                        d_Action act =*(*LStacks)[i]->DotsNet->takeFirst();//LStacks->operator [](i).DotsNet.takeFirst();
                        ARTM->DoAction(&MainImage->ViewCanvas[i],act,true); //not layer in action because they can be added and removed w/o change.

                    }*/
                }
            }
            executing=false;
      }
   }

}
void ArtThread::callupdate(){
  //  emit AskRepaint();

    if (!executing){
        executing=true;
        if (!suspended) {
            while ((ActionList.count()>0)|(ActionNetList.count()>0)) {


                if ( (ActionList.count()>0) | (ActionNetList.count()>0) ) {
                    if (ActionList.count()>0){
                        actionpair ap=ActionList.takeAt(0);
                        if (!ap.img->isNull()){
                        ARTM->DoAction(ap.img,ap.Act,true);
                        QApplication::processEvents(QEventLoop::AllEvents,2);
                        }
                        }
                    if (ActionNetList.count()>0){
                        actionpair apnet=ActionNetList.takeAt(0);
                        ARTM->DoAction(apnet.img,apnet.Act,false);
                     //   QApplication::processEvents(QEventLoop::AllEvents,2);
                        }
                    }
                QApplication::processEvents(QEventLoop::AllEvents,6);
                }

            }

    executing=false;
//    uptimer->stop();
    }

}

void ArtThread::ForceSingleCore(){
   //this->setPriority(QThread::HighPriority);
   singlecore=true;
this->terminate();
   uptimer =new QTimer;
   connect(uptimer,SIGNAL(timeout()),this,SLOT(callupdate()),Qt::DirectConnection);
   uptimer->setInterval(40);
   uptimer->start();
   executing=false;


}

void ArtThread::ExecAction(QImage *img, d_Action Act,bool local){
    //converting parameters


    if  ((Act.ToolID == eSmudge)|(Act.ToolID == eDisp)|(Act.ToolID == eCont)) {
    // //clamp position please move outside
        int twd=Act.Brush.Realb.rad_out*Act.Brush.Realb.scale;
        float cx=qMin(Act.Stroke.pos2.x(),(qreal)img->width()-twd);
        cx=qMax(cx,(float)twd);
        float cy=qMin(Act.Stroke.pos2.y(),(qreal)img->height()-twd);
        cy=qMax(cy,(float)twd);
        Act.Stroke.pos2.setX(cx);
        Act.Stroke.pos2.setY(cy);
    }


        actionpair ap;
        ap.Act= Act;
        ap.img=img;
        ap.islocal=local;
        if (!local) ActionNetList.append(ap);
        else ActionList.append(ap);


        //if (!executing) this->start(QThread::LowestPriority);
        if (!executing) {

            if (singlecore) {
                QApplication::processEvents(QEventLoop::AllEvents,2);
            }
            else this->start();
        }


}


void ArtThread::ExecActionList(QList<actionpair> APlist){
    //converting parameters

/*
    if  ((Act.ToolID == eSmudge)|(Act.ToolID == eDisp)|(Act.ToolID == eCont)) {
     //clamp position please move outside
        int twd=Act.Brush.rad_out*((Act.Brush.scale*5.0)/255.0);
        float cx=qMin(Act.Stroke.pos2.x(),(qreal)img->width()-twd);
        cx=qMax(cx,(float)twd);
        float cy=qMin(Act.Stroke.pos2.y(),(qreal)img->height()-twd);
        cy=qMax(cy,(float)twd);
        Act.Stroke.pos2.setX(cx);
        Act.Stroke.pos2.setY(cy);
    }
*/

     //   actionpair ap;
      //  ap.Act= Act;
      //  ap.img=img;
      //  ap.islocal=local;
    if (!APlist.at(0).islocal) ActionNetList.append(APlist);
        else ActionList.append(APlist);
        //if (!executing) this->start(QThread::LowestPriority);
        if (!executing) {

            if (singlecore) {
                QApplication::processEvents(QEventLoop::AllEvents,2);
            }
            else this->start();
        }
}

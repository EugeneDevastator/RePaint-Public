#include "ActionMaster.h"
#include <QtWidgets/QFileDialog>

ActionMaster::ActionMaster(ImageArray *iar,bool forcesinglecore,QObject *parent) :
    QThread(parent)
{
    GlobalLock=false;
    MainImage=iar;
    uptimer =new QTimer;
    connect(uptimer,SIGNAL(timeout()),this,SLOT(ParseSections()),Qt::DirectConnection);
    uptimer->setInterval(40);
    uptimer->start();
    executing=false;
    LActionBusy=false;
//atcount = new quint8;
    LStacks=new QList<LayerStack*>;
LStacks->clear();
LayerStack *ls=new LayerStack();
LayerStack *ls1=new LayerStack();
LayerStack *ls2=new LayerStack();

LStacks->append(ls);
LStacks->append(ls1);
LStacks->append(ls2);
//LStacks.append(ls);
//LStacks.append(ls);

if ((QThread::idealThreadCount()>1) & !forcesinglecore ){
        ARTList.clear();
        atcount=QThread::idealThreadCount()-1;

     for (int i=1;i<=QThread::idealThreadCount()-1;i++){
         ArtThread *at = new ArtThread(iar);
         at->LStacks=LStacks;
         at->tcount=&atcount;
         at->threadID=i-1;
         at->GlobalLock=&GlobalLock;
         ARTList.append(at);
         at->start();
     }

    }
    else {
       atcount=1;
       ARTList.clear();
       ArtThread *at = new ArtThread(iar);
       at->ForceSingleCore();
       at->LStacks=LStacks;
       at->tcount=&atcount;
       at->GlobalLock=&GlobalLock;

        ARTList.append(at);

    }

maxthreads=atcount;
}

// -----------------------  threaded procedures
void ActionMaster::ParseSections(){
  //  emit AskRepaint();
// PLEASE tune timer for aviable updates and sim painting!

    if (!executing){
        executing=true;
        int i=0;
        int tn; //thread number to give job to;
        for (int i=0;i<MainImage->ViewCanvas.count();i++){

          //  (currlayer % corecount) +1
          //  tn=(i % atcount);  //0=0 1=1 n-1%n=n-1
          //  ARTList[tn]->ExecAction(LStacks[i].LocalDots.takeAt(0));
            sectionList sList=(*LStacks)[i]->GetLocalSects();
    while (!sList.isEmpty()){
            while (sList.count()>0){
                this->UnpackSection(sList.takeAt(0),true);
                foreach (ArtThread *at,ARTList){
                  if (!at->executing) at->start();
                }
            }
    sList=(*LStacks)[i]->GetLocalSects();
    }
            /*
            while ((*LStacks)[i].SectsNet.count()>0){
                this->UnpackSection(*(*LStacks)[i].SectsNet.takeAt(0),false);
                foreach (ArtThread *at,ARTList){
                   if (!at->executing) at->start();
                    }
                }
                */

      //      QApplication::processEvents(QEventLoop::AllEvents,6);
            }
        }

         //
   executing=false;
  // uptimer->stop();
   //QApplication::processEvents(QEventLoop::AllEvents,6);
   }





// --------------------- execution methids-----------
void ActionMaster::ExecOperation(quint8 OpType, QByteArray Data){

}


void ActionMaster::ExecNetSection(d_Section Sect){
 if (LStacks->count()-1>=Sect.layer){
     d_Section *pSect = new d_Section;
     *pSect=Sect;
   //  (*LStacks)[Sect.layer]->addNetSect(Sect);
 }
}

void ActionMaster::ExecSection(d_Section Sect){
 if (LStacks->count()-1>=Sect.layer){

     d_Section *pSect = new d_Section;
     *pSect=Sect;

     (*LStacks)[Sect.layer]->addLocalSect(Sect);
 }
}


void ActionMaster::ExecLayerAction(d_LAction lact){
    LAStack.append(lact);
    if (!LActionBusy && LAStack.count()>0) {
int k=1;
k=k+1;
        ParseLActions();
    }
    return;
    }

void ActionMaster::ConfirmLAction(){
     LActionBusy=false;
     if (LAStack.count()>0){
        ParseLActions();
     }
    }


void ActionMaster::ParseLActions(){

    if (LActionBusy | LAStack.isEmpty()) {
        int k=1;
        return;
    }

    LActionBusy=true;
    d_LAction lact=LAStack.takeFirst();

    if (lact.ActID==laBm){
        MainImage->SetLBm(lact.layer,lact.bm);
    }
    else if (lact.ActID==laOp){
        MainImage->SetLOp(lact.layer,lact.op);
    }
    else if (lact.ActID==laDup){
        MainImage->DupLayer(lact.layer);

        LayerStack *LS=new LayerStack();
        LStacks->insert(lact.layer,LS);
    }
    else if (lact.ActID==laDel){
GlobalLock=true;
         if (MainImage->ViewCanvas.count()>1){

// SUSPEND ALL THREADS FIRST!
        LStacks->removeAt(lact.layer);
        MainImage->DelLayer(lact.layer);

       }
                  //    GlobalLock=false;
   //      for (int k=0;k<ARTList.count();k++){
  //         ARTList[k]->start();
  //       }
    }
    else if (lact.ActID==laAdd){
        LayerStack *LS=new LayerStack;
        LStacks->insert(lact.layer,LS);
        MainImage->AddLayerAt(lact.layer);
    }
    else if (lact.ActID==laDrop){
        if (lact.layer<MainImage->ViewCanvas.count()-1){
            LStacks->removeAt(lact.layer);
            MainImage->DropLayer(lact.layer);

            }
    }
    else if (lact.ActID==laMove){

        if (lact.layer>=LStacks->count()) lact.layer=LStacks->count()-1;
        if (lact.layerto>=LStacks->count()) lact.layerto=LStacks->count()-1;

        LStacks->move(lact.layer,lact.layerto);
        MainImage->MoveLayer(lact.layer,lact.layerto);

    }
    else if (lact.ActID==laNewCanvas){

        LStacks->clear();
        NewImg(lact.rect.size(),lact.layer);
        for (int i=0;i<lact.layer;i++) {
            LayerStack *LS=new LayerStack;
            LStacks->append(LS);
        }

    }
    else if (lact.ActID==laResizeCanvas){
        MainImage->ResizeScene(lact.rect.size(),1);
    }
    else if (lact.ActID==laCropCanvas){
        MainImage->ResizeScene(lact.rect.size(),0);
    } // else return;


    emit SendLAction(lact);
    MainImage->GenAllThumbs();
return;

}

//----------------------- data conversion procedures

void ActionMaster::UnpackSection(d_Section Sect, bool local){
// please perhaps Backward stroke resolve! bug with rnd colors

    dotList tmpList;
    tmpList.clear();
      if (local)  emit SendSection(Sect);

       qreal stdist = Dist2D(Sect.Stroke.pos1,Sect.Stroke.pos2);


      int rad = Sect.BrushFrom.Realb.rad_out*Sect.BrushFrom.Realb.scale;
      int endradius = Sect.Brush.Realb.rad_out*Sect.BrushFrom.Realb.scale;

       qreal dx=Sect.Stroke.pos1.x()-Sect.Stroke.pos2.x();
       qreal dy=Sect.Stroke.pos1.y()-Sect.Stroke.pos2.y();
       qreal x2r=dx/stdist;
       qreal y2r=dy/stdist;



           //qreal curlen = Sect.spacing_offset; //last point of border, curlen is SP, curlen is the edge of joint for new brush
           qreal curlen = 0;//offset; // initial offset must be = -rad*sect.spacing
           qreal nextrad=rad+(curlen*(endradius-rad)/stdist);//((stdist*rad)+(endradius-rad)*curlen)/(stdist+rad-endradius); // calculating next radius`
           qreal nextlen=curlen+nextrad*Sect.spacing; // nextlen is location of next centre measured from stroke start

           qreal rnflw=0.0;//(qrand()/32767.0*rrang*2)-rrang;
           qreal rnside=0.0;//(qrand()/32767.0*rrang*2)-rrang;


           QPointF dotpos1;
         //  dotpos1.setX(Sect.Stroke.pos1.x()+nextlen*x2r-rnflw*y2r+rnside*x2r);
         //  dotpos1.setY(Sect.Stroke.pos1.y()+nextlen*y2r+rnflw*x2r+rnside*y2r);

           QPointF dotpos2;
           dotpos2=Sect.Stroke.pos2;
           /*dotpos2.setX(Sect.Stroke.pos2.x()+curlen*x2r-rnflw*y2r+rnside*x2r);
           dotpos2.setY(Sect.Stroke.pos2.y()+curlen*y2r+rnflw*x2r+rnside*y2r);*/


           //what if length < than rads?
 // qreal rrang=Sect.spacing*Sect.Brush.rad_out*Sect.Brush.scale;
   qreal rrang=Sect.Brush.Realb.rad_out*Sect.Brush.Realb.scale*(Sect.scatter/51.0);
//rrang=0;

               quint16 n=0;
        if (nextlen < stdist) {
           while (nextlen < stdist) {
                n=n+1;
                ;
               rnflw=RawRnd(Sect.BrushFrom.Realb.seed+n*2,1024)*rrang*2-rrang;//(qrand()/32767.0*rrang*2)-rrang;
               rnside=0;//0.0;//(qrand()/32767.0*rrang*2)-rrang;


          //original equo
             //  dotpos1.setX(Sect.Stroke.pos2.x()+nextlen*x2r-rnflw*y2r+rnside*x2r);
            //   dotpos1.setY(Sect.Stroke.pos2.y()+nextlen*y2r+rnflw*x2r+rnside*y2r);
               dotpos1.setX(Sect.Stroke.pos2.x()+((nextlen*dx)/stdist)-rnflw*y2r+rnside*x2r);
               dotpos1.setY(Sect.Stroke.pos2.y()+((nextlen*dy)/stdist)+rnflw*x2r+rnside*y2r);

d_Action *newact = new d_Action;//=Sect;

               //------- send actions to thread
               qreal k = nextlen/stdist;
               d_Brush Cbrush=Sect.BrushFrom;
               Cbrush.Realb=BlendBrushes(Sect.BrushFrom.Realb,Sect.Brush.Realb,k);

               newact->layer=Sect.layer;
               newact->startseed=Sect.startseed;
               newact->Stroke.pos1=dotpos1;
               newact->Stroke.pos2=dotpos2;
               newact->Brush=Cbrush;
               newact->ToolID=Sect.ToolID;



               if (Sect.Noisemode==0) //random
               {

                   newact->Brush.Realb.noisex=RawRnd(Sect.BrushFrom.Realb.seed+n*3,1024)*1024;
                   newact->Brush.Realb.noisey=RawRnd(Sect.BrushFrom.Realb.seed+n+21,1024)*1024;

               }
               else if (Sect.Noisemode==1) { //constant
                  //  act.Brush.Realb.seed=45698;
                   newact->Brush.Realb.noisex=34;
                   newact->Brush.Realb.noisey=76;
               }
               else if (Sect.Noisemode==2) { //stencil
                   newact->Brush.Realb.noisex=newact->Stroke.pos1.x();
                   newact->Brush.Realb.noisey=newact->Stroke.pos1.y();
               }

               newact->Brush.Realb.noisex-=1024*floor(newact->Brush.Realb.noisex/1024);
               newact->Brush.Realb.noisey-=1024*floor(newact->Brush.Realb.noisey/1024);
               newact->Brush.Realb.noisex=abs(newact->Brush.Realb.noisex);
               newact->Brush.Realb.noisey=abs(newact->Brush.Realb.noisey);

                newact->layer=Sect.layer;
             /*  actionpair ap;
               ap.Act=newact;
               ap.islocal=local;
               ap.img=&MainImage->ViewCanvas[Sect.layer];
                APlist.append(ap);*/
               if (local){
                   d_Action rawact=*newact;
                   //(*LStacks)[Sect.layer]->addLocalDot(rawact);
                   tmpList.append(rawact);
                   delete newact;
               }
               else
                 {

 //                  (*LStacks)[Sect.layer].DotsNet->append(newact);
}
            //   QApplication::processEvents(QEventLoop::AllEvents,2);
               //
               dotpos2=dotpos1;
           //    if (local) offset=-stdist+nextlen;
               nextlen=nextlen+nextrad*Sect.spacing;

           }
          //      ARTM->ExecActionList(APlist);

        }

   //     else
     //      if (local)offset-=stdist;

(*LStacks)[Sect.layer]->addLocalDotList(tmpList);

}



// ------------------------------
// FILE OPERATIONS:
//------------------------------


void ActionMaster::NewImg(QSize sz, int lcount){

    MainImage->MasterSize=sz;

    MainImage->ViewCanvas.clear();
    MainImage->LayerProps.clear();
    MainImage->DropLayers.clear();
    MainImage->AddZeroLayer(sz,Qt::blue);
    resizeImage(&MainImage->ViewCanvas[0], sz);

    qDebug() <<("Imar vc resized;");



    if (lcount > 1)
        for (int i=1;i<lcount;i++){
        /*d_LAction lact;
        lact.ActID=laAdd;
        lact.layer=1;
        ExecLayerAction(lact);
        */
            MainImage->AddLayerAt(1);
        }
    QPainter painter(&MainImage->ViewCanvas[MainImage->ViewCanvas.count()-1]);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRect(MainImage->ViewCanvas[0].rect());
    //ViewCanvas[0].fill(g_Brush->col);
    MainImage->RepaintWidgetRect(MainImage->rect());

}

void ActionMaster::OpenImgBa(QByteArray ba){
    QBuffer *buf=new QBuffer(&ba);
    buf->open(QBuffer::ReadWrite);
    OpenImg(buf);
    delete buf;
    MainImage->RepaintWidgetRect(MainImage->rect());
}

/*
void ActionMaster::OpenImg(){
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this->parent(),
                                tr("Open Image"),
                                                    "",
                                                    tr("JPG Files (*.ELI);;PNG files (*.PNG);;JPG Files (*.JPG)"),
                                &selectedFilter,
                                options);

if (!fileName.isEmpty()) OpenImg(fileName);
}
*/
 void ActionMaster::OpenImg(QString fname){
   if (!(fname.right(3).contains("ELI"))){
             ImportImg(fname);
             return;
         }

   QString cname=fname;
   cname.chop(4);
//OpenLog(fname);
        QFile openfile(fname);
        openfile.open(QIODevice::ReadOnly);
        OpenImg(&openfile);
       openfile.close();
       MainImage->wasmerged=false;
        //RepaintWidgetRect(this->rect());
       MainImage->RepaintImgRect(MainImage->ViewCanvas[0].rect());
       MainImage->GenAllThumbs();
}

 void ActionMaster::OpenImg(QIODevice *iodev){
     QDataStream dstream;//(iodev,QIODevice::ReadWrite);
     dstream.setDevice(iodev);
     qint32 lcount;
     qint32 width;
     qint32 height;
     dstream >> lcount;
     dstream >> width;
     dstream >> height;
     //NewImg(QSize(width,height),lcount);

     d_LAction nia;
     nia.layer=lcount;
     nia.rect.setWidth(width);
     nia.rect.setHeight(height);
     nia.ActID=laNewCanvas;
     ExecLayerAction(nia);

     d_LAction visact;


     for (int i=0;i<lcount;i++){
         QByteArray ba;
         qint32 lsz;
         dstream >> lsz;
         ba.resize(lsz);
         dstream >> ba;
         QBuffer buf(&ba);
         buf.open(QIODevice::ReadOnly);

         QImage tmpimg;
         tmpimg.load(&buf,"PNG");
         //resizeImage(&ViewCanvas[i],tmpimg.size());
         QPainter pnt(&MainImage->ViewCanvas[i]);
         pnt.setPen(Qt::NoPen);
         pnt.setBrush(Qt::transparent);
         pnt.setCompositionMode(QPainter::CompositionMode_Source);
         pnt.drawRect(tmpimg.rect());
         pnt.setCompositionMode(QPainter::CompositionMode_SourceOver);
         pnt.drawImage(0,0,tmpimg);

         MainImage->LayerProps[i].DeSerialize(&dstream);

         visact.layer=i;
         visact.vis=MainImage->LayerProps[i].visible;
         visact.ActID=laVis;
         ExecLayerAction(visact);

        }
     MainImage->RepaintWidgetRect(MainImage->rect());
   }

void ActionMaster::ImportImg(QString fname){


    QImage tmpimg=QImage(QSize(50,50),QImage::Format_ARGB32_Premultiplied);
    tmpimg.load(fname,"PNG");

    d_LAction nia;
    nia.layer=1;
    nia.rect.setWidth(tmpimg.width());
    nia.rect.setHeight(tmpimg.height());
    nia.ActID=laNewCanvas;
    ExecLayerAction(nia);
    QPainter pnt(&MainImage->ViewCanvas[0]);
 //   pnt.setPen(Qt::NoPen);
//    pnt.setBrush(QColor::fromRgbF(0,0,0,0));

//    pnt.drawRect(tmpimg.rect());
    pnt.setCompositionMode(QPainter::CompositionMode_Source);
    pnt.drawImage(0,0,tmpimg);
    //ViewCanvas[0].load(fname,"PNG");
MainImage->RepaintWidgetRect(MainImage->rect());
}

// ---------------------------------------------
//----------------------- LOGGING OPERATIONS!
// --------------------------------------------


void ActionMaster::NewLog(){
    LOG.clear();
    baseSize=MainImage->ViewCanvas[0].size();
    baselcount=MainImage->ViewCanvas.count();
    isLogging=true;
}
void ActionMaster::LogAct(d_Action act){
    if (isLogging) {
    logpair lp;
    lp.logid=atAction;
    lp.szdAction=act.Serialize();
    LOG.append(lp);
    }
}
void ActionMaster::LogLAct(d_LAction act){
    if (isLogging) {
    logpair lp;
    lp.logid=atLAction;
    lp.szdAction=act.Serialize();
    LOG.append(lp);
    }
}
void ActionMaster::LogSect(d_Section sect){
    if (isLogging) {
    logpair lp;
    lp.logid=atSect;
    lp.szdAction=sect.Serialize();
    LOG.append(lp);
    }
}

void ActionMaster::OpenLog(QString Fname){

   // if ((Fname.right(3).contains("RDL")))
      Fname.append(".RIL");
            {
            QFile openfile(Fname);
            openfile.open(QIODevice::ReadOnly);
            OpenLog(&openfile);
            openfile.close();
    }
}

void ActionMaster::OpenLog(QIODevice *iodev){
    LOG.clear();
    QDataStream dstream;//(iodev,QIODevice::ReadWrite);
    dstream.setDevice(iodev);
    dstream >> baseSize;
    dstream >> baselcount;
    quint64 lpcount;
    dstream >> lpcount;
    logpair lp;
    for (quint64 i=0;i<lpcount;i++){
        dstream >> lp.logid;
        dstream >> lp.szdAction;
        LOG.append(lp);
    }
  //uptimer->stop();
    for (quint64 i=0;i<lpcount;i++){
        if (LOG[i].logid==atSect) {
            d_Section sect;
            sect.DeSerialize(LOG[i].szdAction);


            sect.Stroke.pos1.setX(((qreal)sect.Stroke.pos1.x()*MainImage->ViewCanvas[0].width())/baseSize.width());
            sect.Stroke.pos1.setY(((qreal)sect.Stroke.pos1.y()*MainImage->ViewCanvas[0].height())/baseSize.height());
            sect.Stroke.pos2.setX(((qreal)sect.Stroke.pos2.x()*MainImage->ViewCanvas[0].width())/baseSize.width());
            sect.Stroke.pos2.setY(((qreal)sect.Stroke.pos2.y()*MainImage->ViewCanvas[0].height())/baseSize.height());
          sect.BrushFrom.Realb.rad_out=((qreal)sect.BrushFrom.Realb.rad_out*MainImage->ViewCanvas[0].width())/baseSize.width();
           sect.BrushFrom.Realb.rad_in=((qreal)sect.BrushFrom.Realb.rad_in*MainImage->ViewCanvas[0].width())/baseSize.width();
           sect.Brush.Realb.rad_out=((qreal)sect.Brush.Realb.rad_out*MainImage->ViewCanvas[0].width())/baseSize.width();
            sect.Brush.Realb.rad_in=((qreal)sect.Brush.Realb.rad_in*MainImage->ViewCanvas[0].width())/baseSize.width();
           // sect.scatter=((qreal)sect.scatter*MainImage->ViewCanvas[0].width())/baseSize.width();
            qreal minrad=3.0;
            if (sect.Brush.Realb.rad_out<minrad) {
                sect.Brush.Realb.opacity*=(sect.Brush.Realb.rad_out)/minrad;
                sect.Brush.Realb.rad_out=(minrad+sect.Brush.Realb.rad_out)*0.5;
                }
            if (sect.BrushFrom.Realb.rad_out<minrad) {
                sect.BrushFrom.Realb.opacity*=(sect.BrushFrom.Realb.rad_out)/minrad;
                sect.BrushFrom.Realb.rad_out=(minrad+sect.BrushFrom.Realb.rad_out)*0.5;
                }
            //LocalSects.append(sect);
           this->ExecSection(sect);
            QApplication::processEvents(QEventLoop::AllEvents,6);
        }

    }
   // uptimer->start();
//executing=false;
}


void ActionMaster::SaveLog(QString Fname){

 //   if ((Fname.right(3).contains("RPL")))
    Fname.append(".RIL");  // repaint image lof
            {
            QFile savefile(Fname);
            savefile.open(QIODevice::ReadWrite|QIODevice::Truncate);
            SaveLog(&savefile);
            savefile.close();
    }
}

void ActionMaster::SaveLog(QIODevice *iodev){
    QDataStream dstream;//(iodev,QIODevice::ReadWrite);
    dstream.setDevice(iodev);
    dstream << baseSize;
    dstream << baselcount;
    quint64 lpcount=LOG.count();
    dstream << lpcount;
    logpair lp;
    for (quint64 i=0;i<lpcount;i++){
        dstream << LOG.at(i).logid;
        dstream << LOG.at(i).szdAction;
    }
}

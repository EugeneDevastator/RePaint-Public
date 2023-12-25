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

#include "strokemaster.h"

StrokeMaster::StrokeMaster(BrushEditorPresenter *bcont, LayersPanelPresenter *lyrp, ImageArray *iar, QWidget *parent) : QWidget(parent)
{
    isLogging = false;
    // ARTM=artm;
    BC = bcont;
    LP = lyrp;
    MainImage = iar;
    baseSize = QSize(1920, 1080);
    baselcount = 3;

    offset = 0;
    ResetLocalPos();
    /*
        uptimer =new QTimer;
        connect(uptimer,SIGNAL(timeout()),this,SLOT(callupdate()),Qt::DirectConnection);
        uptimer->setInterval(40);
        uptimer->start();
        executing=false;
    */
}

void StrokeMaster::ResetLocalPos()
{
    isNew = true;
}
void StrokeMaster::GetRawStroke(d_Stroke Strk, d_StrokePars currpars, d_StrokePars lastpars)
{

    Strk.packpos1.SetByQPointF(Strk.pos1);
    Strk.packpos2.SetByQPointF(Strk.pos2);

    Strk.pos1 = Strk.packpos1.ToPointF();
    Strk.pos2 = Strk.packpos2.ToPointF();

    ActionData nact;
    nact = BC->ParseBrush(Strk, currpars);
    StrokeSection Sect;
    Sect.Stroke = Strk;
    Sect.Noisemode = nact.Noisemode;
    Sect.Brush = nact.Brush; // BC->ParseBrush(Strk,currpars).Brush;
    if (isNew)
        Sect.BrushFrom = BC->ParseBrush(Strk, lastpars).Brush;
    else
        Sect.BrushFrom = prevBrush;
    prevBrush = Sect.Brush;

    Sect.layer = LP->GetActiveLayer();
    Sect.spacing = BC->CtlSpc->GetModValue(currpars);
    Sect.scatter = BC->CtlScatter->GetModValue(currpars) * (51);
    Sect.ToolID = nact.ToolID;
    if (currpars.Pars[csERASER] == 1)
    {
        Sect.Brush.ClientStamp.bmidx = 1;
        Sect.BrushFrom.ClientStamp.bmidx = 1;
    }
    if (LP->BtnPresOp->isChecked())
    {
        Sect.Brush.ClientStamp.preserveop = 1;
        Sect.BrushFrom.ClientStamp.preserveop = 1;
    }

    if (isNew)
    {
        isNew = false;
        lastpos = CalcLastPos(Sect);
    }
    else
    {
        Sect.Stroke.pos2 = lastpos;
        lastpos = CalcLastPos(Sect);
    }

    // this->ExecSection(Sect,true);
    LocalSects.append(Sect);
    QApplication::processEvents(QEventLoop::AllEvents, 7);
    emit SendReadySect(Sect);
}

void StrokeMaster::ExecSectionNet(StrokeSection Sect)
{
    // DO FUCKING NOTHING
    //   NetSects.append(Sect);
}
/*
void StrokeMaster::callupdate(){
  //  emit AskRepaint();

    if (!executing){
        executing=true;
      //  if (!suspended) {
            while ((LocalSects.count()>0)|(NetSects.count()>0)) {


                if ( (LocalSects.count()>0) | (NetSects.count()>0) ) {
                    if (LocalSects.count()>0){
                        ExecSection(LocalSects.takeAt(0),true);
                      //  QApplication::processEvents(QEventLoop::AllEvents,2);
                        }
                    if (NetSects.count()>0){
                         ExecSection(NetSects.takeAt(0),false);
                      //  DoAction(ap.img,ap.Act);
                     //   QApplication::processEvents(QEventLoop::AllEvents,2);
                        }
                    }
                QApplication::processEvents(QEventLoop::AllEvents,6);
                }

            }

   executing=false;
  // uptimer->stop();
   }

*/
